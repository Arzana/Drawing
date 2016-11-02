#define _USE_GF_INTERNAL
#define _USE_LINE_CLIP

#define vrtxat(x)			(&Vertex(GF_ToScreen(hbuffer + x), cbuffer[x]))
#define single_line_start	int clp = hbuffer[i].Clip() + hbuffer[j].Clip(); if (clp) {
#define single_line_end		Line *l = &Line(vrtxat(i), vrtxat(j)); if (LineClip(l, port)) { GF_Line(l); } } else { GF_Line(vrtxat(i), vrtxat(j)); }
#define single_line_for		single_line_start if (!flags.Clip || clp > 1) { continue; } single_line_end
#define single_line_nonfor	single_line_start if (!flags.Clip || clp > 1) { return; } single_line_end
#define ZVC_ARGS			(const Vertex*)
#define ZXY_ARGS			(const uint, const uint, const uint, const Color)
#define ZVC_PLT				(void(GameWindow::*)ZVC_ARGS)
#define ZXY_PLT				(void(GameWindow::*)ZXY_ARGS)
#define get_zvc_plt			void(GameWindow::*plt)ZVC_ARGS = flags.ZBuff ? ZVC_PLT &GameWindow::TryPlot : ZVC_PLT &GameWindow::Plot;
#define get_zxy_plt			void(GameWindow::*plt)ZXY_ARGS = flags.ZBuff ? ZXY_PLT &GameWindow::TryPlot : ZXY_PLT &GameWindow::Plot;
#define plot				(w->*plt)

#include "Utils.h"
#include "GF.h"
#include <cstdio>
#include <cfloat>

GameWindow *w = NULL;
Flags flags = Flags();
Rect port;

size_t bufferLength = 0;
size_t bufferIndex = 0;
Vect3 *vbuffer = NULL;
Color *cbuffer = NULL;
Vect4 *hbuffer = NULL;

Mtrx4 model = MTRX4_IDENTITY;
Mtrx4 view = MTRX4_IDENTITY;
Mtrx4 pers = MTRX4_IDENTITY;
float near = 0;
float far = FLT_MAX;

void GF_SetWindow(GameWindow * window)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetWindow(GameWindow*)");
#endif

	w = window;
	GF_SetViewport(&Rect(0, 0, w->GetWidth(), w->GetHeight()));
}

void GF_StartRender(const int primitiveType)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_StartRender(int)");
#endif

	if (!w || flags.Strt)
	{
		if (!w) Raise("GF_SetWindow must be called before calling GF_StartRender!");
		if (flags.Strt) Raise("GF_EndRender must be called before calling GF_StartRender again!");
		return;
	}

	flags.Prim = primitiveType;
	flags.Strt = 1;
}

void GF_EndRender(void)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_EndRender(void)");
#endif

	if (!flags.Strt || bufferIndex < bufferLength || !w)
	{
		if (!flags.Strt) Raise("GF_StartRender must be called before calling GF_EndRender!");
		if (bufferIndex < bufferLength) Raise("Not all points in the buffer have been set!");
		if (!w) Raise("GF_SetWindow must be called before calling GF_EndRender!");
		return;
	}

	Mtrx4 modelView = model * view;
	Mtrx4 full = pers * modelView;
	hbuffer = Mtrx4::Transform(&full, vbuffer, bufferLength);

	switch (flags.Prim)
	{
	case GF_POINTS:
		GF_Points();
		break;
	case GF_LINES:
		GF_Lines();
		break;
	case GF_LINE_STRIP:
		GF_LineStrip();
		break;
	case GF_LINE_LOOP:
		GF_LineLoop();
		break;
	case GF_TRIANGLES:
		GF_Triangles();
		break;
	case GF_TRIANGLE_STRIP:
		GF_TriangleStrip();
		break;
	case GF_TRIANGLE_FAN:
		GF_TriangleFan();
		break;
	default:
		Raise("Unknown primitive type requested!");
		break;
	}

	flags.Strt = 0;
	bufferIndex = 0;
	bufferLength = 0;
	model = MTRX4_IDENTITY;
	free(vbuffer);
	free(cbuffer);
	free(hbuffer);
}

void GF_SetBufferLength(size_t length)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetBufferLength(size_t)");
#endif

	if (length < 1)
	{
		Raise("Length must be greater than zero!");
		return;
	}

	bufferLength = length;
	vbuffer = malloc_s(Vect3, length);
	cbuffer = malloc_s(Color, length);
}

void GF_SetModelMatrix(const Matrix4 * m)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetWorldMatrix(Matrix4*)");
#endif

	model = *m;
}

void GF_SetViewMatrix(const Matrix4 * m)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetModelMatrix(Matrix4*)");
#endif

	view = *m;
}

void GF_SetFrustrum(const float fovY, const float aspr, const float front, const float back)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetFrustrum(float, float, float, float)");
#endif

	pers = Mtrx4::CreateFrustrum(fovY, aspr, front, back);
	flags.Proj = 1;
	far = back;
	near = front;
}

void GF_SetOrthographic(const float width, const float height, const float front, const float back)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetOrthographic(float, float, float, float)");
#endif

	pers = Mtrx4::CreateOrthographic(width, height, front, back);
	flags.Proj = 0;
	far = back;
	near = front;
}

void GF_SetViewport(const Rectangle * rect)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetViewport(Rectangle*)");
#endif

	port = *rect;
}

void GF_SetFlag_Clip(const bool value)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetFlag_Clip(bool)");
#endif

	flags.Clip = value;
}

void GF_SetFlag_ZBuff(const bool value)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_SetFlag_ZBuff(bool)");
#endif

	flags.ZBuff = value;
}

void GF_AddPoint(const Vector3 v, const Color c)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_AddPoint(Vector3, Color)");
#endif

	if (!flags.Strt || bufferIndex >= bufferLength)
	{
		if (!flags.Strt) Raise("GF_StartRender must be called before calling GF_AddPoint!");
		if (bufferLength < 1) Raise("GF_SetBufferLength must be called before calling GF_AddPoint!");
		if (bufferIndex >= bufferLength) Raise("Cannot add any more points to the buffer!");
		return;
	}

	vbuffer[bufferIndex] = v;
	cbuffer[bufferIndex] = c;
	++bufferIndex;
}

void GF_AddPoint(const float x, const float y, float z, const Color c)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_AddPoint(float, float, float, Color)");
#endif

	GF_AddPoint(Vect3(x, y, z), c);
}

void GF_AddPoint(const Vertex vtx)
{
	GF_AddPoint(vtx.v, vtx.c);
}

void GF_Points(void)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_Points(void)");
#endif

	get_zvc_plt

	for (size_t i = 0; i < bufferLength; i++)
	{
		if (hbuffer[i].Clip()) continue;
		plot(vrtxat(i));
	}
}

void GF_Lines(void)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_Lines(void)");
#endif

	for (size_t i = 0, j = 1; i < bufferLength; i += 2, j += 2)
	{
		single_line_for
	}
}

void GF_LineStrip(void)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_LineStrip(void)");
#endif

	for (size_t i = 0, j = 1; i < bufferLength - 1; i++, j++)
	{
		single_line_for
	}
}

void GF_LineLoop(void)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_LineLoop(void)");
#endif

	size_t i = 0, j = 1;
	for (; i < bufferLength - 1; i++, j++)
	{
		single_line_for
	}

	i = bufferLength - 1, j = 0;
	single_line_nonfor
}

void GF_Triangles(void)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_Triangles(void)");
#endif

	for (size_t i = 0; i < bufferLength; i += 3)
	{
		Vect4 ph0 = hbuffer[i], ph1 = hbuffer[i + 1], ph2 = hbuffer[i + 2];
		if (ph0.Clip() || ph1.Clip() || ph2.Clip()) continue;

		Vect3 p0 = GF_ToNDC(&ph0), p1 = GF_ToNDC(&ph1), p2 = GF_ToNDC(&ph2);
		GF_ToScreen(&p0), GF_ToScreen(&p1), GF_ToScreen(&p2);

		GF_FullTrgl(&Vertex(p0, cbuffer[i]), &Vertex(p1, cbuffer[i + 1]), &Vertex(p2, cbuffer[i + 2]));
	}
}

void GF_TriangleStrip(void)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_TriangleStrip(void)");
#endif

	for (size_t i = 2; i < bufferLength; i++)
	{
		Vect4 ph0 = hbuffer[i - 2], ph1 = hbuffer[i - 1], ph2 = hbuffer[i];
		if (ph0.Clip() || ph1.Clip() || ph2.Clip()) continue;

		Vect3 p0 = GF_ToNDC(&ph0), p1 = GF_ToNDC(&ph1), p2 = GF_ToNDC(&ph2);
		GF_ToScreen(&p0), GF_ToScreen(&p1), GF_ToScreen(&p2);

		GF_FullTrgl(&Vertex(p0, cbuffer[i - 2]), &Vertex(p1, cbuffer[i - 1]), &Vertex(p2, cbuffer[i]));
	}
}

void GF_TriangleFan(void)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_TriangleFan(void)");
#endif

	for (size_t i = 1; i < bufferLength - 1; i++)
	{
		Vect4 ph0 = hbuffer[0], ph1 = hbuffer[i], ph2 = hbuffer[i + 1];
		if (ph0.Clip() || ph1.Clip() || ph2.Clip()) continue;

		Vect3 p0 = GF_ToNDC(&ph0), p1 = GF_ToNDC(&ph1), p2 = GF_ToNDC(&ph2);
		GF_ToScreen(&p0), GF_ToScreen(&p1), GF_ToScreen(&p2);

		GF_FullTrgl(&Vertex(p0, cbuffer[0]), &Vertex(p1, cbuffer[i]), &Vertex(p2, cbuffer[i + 1]));
	}
}

Vect3 GF_ToNDC(const Vect4 * v)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_ToNDC(Vect4*)");
#endif

	return flags.Proj ? v->ToNDC() : Vect3(v->X, v->Y, v->Z);
}

void GF_ToScreen(Vect3 * v)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_ToScreen(Vect3*)");
#endif

	v->X = port.w * 0.5 * v->X + port.w * 0.5;
	v->Y = port.h * 0.5 * v->Y + port.h * 0.5;
	v->Z = (far - near) * 0.5 * v->Z + (far + near) * 0.5;
}

Vect3 GF_ToScreen(Vect4 * v)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_ToScreen(Vect4*)");
#endif

	Vect3 r = GF_ToNDC(v);
	GF_ToScreen(&r);
	return r;
}

void GF_Line(const int x0, const int y0, const int z0, const Color c0, const int x1, const int y1, const int z1, const Color c1)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_Line(int, int, int, Color, int, int, int, Color)");
#endif

	int width = x1 - x0;
	int height = y1 - y0;

	int dx0 = width < 0 ? -1 : (width > 0 ? 1 : 0);
	int dy0 = height < 0 ? -1 : (height > 0 ? 1 : 0);
	int dx1 = dx0;
	int dy1 = 0;

	uint lng = abs(width);
	uint shrt = abs(height);
	if (lng <= shrt)
	{
		lng = shrt;
		shrt = abs(width);
		dy1 = height < 0 ? -1 : (height > 0 ? 1 : 0);
		dx1 = 0;
	}

	uint num = lng >> 1;
	uint x = x0;
	uint y = y0;

	get_zxy_plt
	for (size_t i = 0; i < lng; i++)
	{
		float a = invLerp(0, lng, i);
		plot(x, y, lerp(z0, z1, a), Color::Lerp(c0, c1, a));
		num += shrt;
		if (num >= lng)
		{
			num -= lng;
			x += dx0;
			y += dy0;
		}
		else
		{
			x += dx1;
			y += dy1;
		}
	}
}

void GF_Line(const Vertex * v0, const Vertex * v1)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_Line(Vertex*, Vertex*)");
#endif

	GF_Line(v0->v.X, v0->v.Y, v0->v.Z, v0->c, v1->v.X, v1->v.Y, v1->v.Z, v1->c);
}

void GF_Line(const Line * line)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_Line(Line*)");
#endif

	GF_Line(line->v0.v.X, line->v0.v.Y, line->v0.v.Z, line->v0.c, line->v1.v.X, line->v1.v.Y, line->v1.v.Z, line->v1.c);
}

void GF_HLine(const float x0, const float z0, const Color c0, const float x1, const float z1, const Color c1, const float y)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_HLine(float, float, Color, float, float, Color, float)");
#endif

	get_zxy_plt

	if (x0 == x1)
	{
		plot(x0, y, z0, c0);
	}
	else if (x0 < x1)
	{
		for (float x = x0; x <= x1; ++x)
		{
			float a = invLerp(x0, x1, x);
			plot(x, y, lerp(z0, z1, a), Color::Lerp(c0, c1, a));
		}
	}
	else
	{
		for (float x = x1; x <= x0; ++x)
		{
			float a = invLerp(x1, x0, x);
			plot(x, y, lerp(z0, z1, a), Color::Lerp(c0, c1, a));
		}
	}
}

void GF_HLine(const Vertex * v0, const Vertex * v1)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_HLine(Vertex*, Vertex*)");
#endif

	GF_HLine(v0->v.X, v0->v.Z, v0->c, v1->v.X, v1->v.Z, v1->c, v0->v.Y);
}

void GF_BFTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_BFTrgl(Vertex*, Vertex*, Vertex*)");
#endif

	float invSlp0 = (v1->v.X - v0->v.X) / (v1->v.Y - v0->v.Y);
	float invSlp1 = (v2->v.X - v0->v.X) / (v2->v.Y - v0->v.Y);

	float x0 = v0->v.X;
	float x1 = v0->v.X;

	for (float y = v0->v.Y; y <= v1->v.Y; y++)
	{
		float a = invLerp(v0->v.Y, v1->v.Y, y);
		Color c0 = Color::Lerp(v0->c, v1->c, a);
		Color c1 = Color::Lerp(v0->c, v2->c, a);
		float z0 = lerp(v0->v.Z, v1->v.Z, a);
		float z1 = lerp(v0->v.Z, v2->v.Z, a);

		GF_HLine(x0, z0, c0, x1, z1, c1, y);
		x0 += invSlp0;
		x1 += invSlp1;
	}
}

void GF_TFTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_TFTrgl(Vertex* Vertex*, Vertex*)");
#endif

	float invSlp0 = (v2->v.X - v0->v.X) / (v2->v.Y - v0->v.Y);
	float invSlp1 = (v2->v.X - v1->v.X) / (v2->v.Y - v1->v.Y);

	float x0 = v2->v.X;
	float x1 = v2->v.X;

	for (float y = v2->v.Y; y > v0->v.Y; y--)
	{
		float a = invLerp(v2->v.Y, v0->v.Y, y);
		Color c0 = Color::Lerp(v2->c, v0->c, a);
		Color c1 = Color::Lerp(v2->c, v1->c, a);
		float z0 = lerp(v2->v.Z, v0->v.Z, a);
		float z1 = lerp(v2->v.Z, v1->v.Z, a);

		GF_HLine(x0, z0, c0, x1, z1, c1, y);
		x0 -= invSlp0;
		x1 -= invSlp1;
	}
}

void GF_FullTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
#ifdef _SHOW_GF_FUNCTIONS_USED
	PrintFunction("GF_FullTrgl(Vertex*, Vertex*, Vertex*)");
#endif

	Vertex vt0 = *v0;
	Vertex vt1 = *v1;
	Vertex vt2 = *v2;

	SortVerticesByY(&vt0, &vt1, &vt2);

	if (vt1.v.Y == vt2.v.Y) GF_BFTrgl(&vt0, &vt1, &vt2);
	else if (vt0.v.Y == vt2.v.Y) GF_TFTrgl(&vt0, &vt1, &vt2);
	else
	{
		float x = vt0.v.X + (vt1.v.Y - vt0.v.Y) / (vt2.v.Y - vt0.v.Y) * (vt2.v.X - vt0.v.X);
		float y = vt1.v.Y;
		float a = invLerp(vt0.v.Y, vt2.v.Y, y);
		float z = lerp(vt0.v.Z, vt2.v.Z, a);

		Vect3 v3 = Vect3(x, y, z);
		Color c3 = Color::Lerp(vt0.c, vt2.c, a);
		Vertex vt3 = Vertex(v3, c3);

		GF_BFTrgl(&vt0, &vt3, &vt1);
		GF_TFTrgl(&vt3, &vt1, &vt2);
	}
}

void Raise(const char *msg)
{
	printf(msg);
	printf("\n");
	w->Terminate();
}

#ifdef _SHOW_GF_FUNCTIONS_USED
void PrintFunction(const char *name)
{
	printf("-> ");
	printf(name);
	printf("\n");
}
#endif