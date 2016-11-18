#define _USE_GF_INTERNAL
#define _USE_CLIPPING

#define N_THREADS			100

#define vrtxat(x)			(&Vertex(GF_ToScreen(hbuffer + x), cbuffer[x]))
#define ZVC_ARGS			(const Vertex*)
#define ZXY_ARGS			(const float, const float, const float, const Color)
#define ZVC_PLT				(void(GameWindow::*)ZVC_ARGS)
#define ZXY_PLT				(void(GameWindow::*)ZXY_ARGS)
#define get_zvc_plt			void(GameWindow::*plt)ZVC_ARGS = flags.zBuff ? ZVC_PLT &GameWindow::TryPlot : ZVC_PLT &GameWindow::Plot
#define get_zxy_plt			void(GameWindow::*plt)ZXY_ARGS = flags.zBuff ? ZXY_PLT &GameWindow::TryPlot : ZXY_PLT &GameWindow::Plot
#define plot				(w->*plt)
#define get_pop(v)			v.back(); v.pop_back()

#include <cstdio>
#include <cfloat>
#include <vector>
#include <thread>
#include <mutex>
#include "Utils.h"
#include "GF.h"

using namespace std;

struct hline 
{
	float x0, z0;
	Color c0;
	float x1, z1;
	Color c1;
	float y;

	hline(float x0, float z0, Color c0, float x1, float z1, Color c1, float y)
		: x0(x0), z0(z0), c0(c0), x1(x1), z1(z1), c1(c1), y(y)
	{ }
};

thread ths[N_THREADS];
mutex mtx;
vector<hline> lBuff;
bool running;

GameWindow *w = NULL;
Flags flags;
ViewPort port = ViewPort(0, 0, 0, 0, 0, FLT_MAX);
Vect4 cPort;

size_t bufferLength = 0;
size_t bufferIndex = 0;
Vect3 *vbuffer = NULL;
Color *cbuffer = NULL;
Vect4 *hbuffer = NULL;

Mtrx4 model = MTRX4_IDENTITY;
Mtrx4 view = MTRX4_IDENTITY;
Mtrx4 pers = MTRX4_IDENTITY;

void line_func(const size_t thrdId)
{
	printf("starting hline(%d)\n", thrdId);

	while (running)
	{
		mtx.lock();
		if (lBuff.size() > 0)
		{
			hline l = get_pop(lBuff);
			mtx.unlock();

			GF_HLine(l.x0, l.z0, l.c0, l.x1, l.z1, l.c1, l.y);
		}
		else
		{
			mtx.unlock();
			this_thread::sleep_for(chrono::milliseconds(1));
		}
	}

	printf("stopping hline(%d)\n", thrdId);
}

void GF_Init(void)
{
	flags.init = true;
	running = true;

	for (size_t i = 0; i < N_THREADS; i++)
	{
		ths[i] = thread(line_func, i);
	}
}

void GF_End(void)
{
	running = false;

	for (size_t i = 0; i < N_THREADS; i++)
	{
		ths[i].join();
	}
}

void GF_SetWindow(GameWindow * window)
{
	w = window;
	GF_SetViewport(&Rect(0, 0, w->GetWidth() - 1, w->GetHeight() - 1));
	w->OnTerminate = [](void) { running = false; };
}

void GF_StartRender(const int primitiveType)
{
	if (!flags.init || !w || flags.strt)
	{
		if (!flags.init) Raise("GF_Init must be called before calling any graphics functions!");
		if (!w) Raise("GF_SetWindow must be called before calling GF_StartRender!");
		if (flags.strt) Raise("GF_EndRender must be called before calling GF_StartRender again!");
		return;
	}

	flags.prim = primitiveType;
	flags.strt = 1;
}

void GF_EndRender(void)
{
	if (!flags.init || !flags.strt || bufferIndex < bufferLength || !w)
	{
		if (!flags.init) Raise("GF_Init must be called before calling any graphics functions!");
		if (!flags.strt) Raise("GF_StartRender must be called before calling GF_EndRender!");
		if (bufferIndex < bufferLength) Raise("Not all points in the buffer have been set!");
		if (!w) Raise("GF_SetWindow must be called before calling GF_EndRender!");
		return;
	}

	Mtrx4 modelView = model * view;
	Mtrx4 full = pers * modelView;
	hbuffer = Mtrx4::Transform(&full, vbuffer, bufferLength);

	switch (flags.prim)
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

	while (lBuff.size() > 0) this_thread::sleep_for(chrono::milliseconds(1));

	flags.strt = 0;
	bufferIndex = 0;
	bufferLength = 0;
	model = MTRX4_IDENTITY;
	free(vbuffer);
	free(cbuffer);
	free(hbuffer);
}

void GF_SetBufferLength(size_t length)
{
	if (!flags.init || length < 1 || bufferLength != 0)
	{
		if (!flags.init) Raise("GF_Init must be called before calling any graphics functions!");
		if (length < 1) Raise("Length must be greater than zero!");
		if (bufferLength != 0) Raise("GF_EndRender must be called before respecifying the buffer length!");
		return;
	}

	bufferLength = length;
	vbuffer = malloc_s(Vect3, length);
	cbuffer = malloc_s(Color, length);
}

void GF_SetModelMatrix(const Matrix4 * m)
{
	model = *m;
}

void GF_SetViewMatrix(const Matrix4 * m)
{
	view = *m;
}

void GF_SetFrustrum(const float fovY, const float aspr, const float front, const float back)
{
	pers = Mtrx4::CreateFrustrum(fovY, aspr, front, back);
	flags.proj = 1;
	GF_SetDepth(front, back);
}

void GF_SetOrthographic(const float width, const float height, const float front, const float back)
{
	pers = Mtrx4::CreateOrthographic(width, height, front, back);
	flags.proj = 0;
	GF_SetDepth(front, back);
}

void GF_SetViewport(const Rect * rect)
{
	port.screen = *rect;
	cPort.X = rect->w * 0.5;
	cPort.Y = rect->h * 0.5;
}

void GF_SetFlag_Clip(const bool value)
{
	flags.clip = value;
}

void GF_SetFlag_ZBuff(const bool value)
{
	flags.zBuff = value;
}

void GF_AddPoint(const Vector3 v, const Color c)
{
	if (!flags.init || !flags.strt || bufferIndex >= bufferLength)
	{
		if (!flags.init) Raise("GF_Init must be called before calling any graphics functions!");
		if (!flags.strt) Raise("GF_StartRender must be called before calling GF_AddPoint!");
		if (bufferLength < 1) Raise("GF_SetBufferLength must be called before calling GF_AddPoint!");
		if (bufferIndex >= bufferLength) Raise("Cannot add any more points to the buffer!");
		return;
	}

	vbuffer[bufferIndex] = v;
	cbuffer[bufferIndex++] = c;
}

void GF_AddPoint(const float x, const float y, float z, const Color c)
{
	GF_AddPoint(Vect3(x, y, z), c);
}

void GF_AddPoint(const Vertex vtx)
{
	GF_AddPoint(vtx.v, vtx.c);
}

void GF_Points(void)
{
	get_zvc_plt;

	for (size_t i = 0; i < bufferLength; i++)
	{
		if (hbuffer[i].Clip()) continue;
		plot(vrtxat(i));
	}
}

void GF_Lines(void)
{
	for (size_t i = 0, j = 1; i < bufferLength; i += 2, j += 2)
	{
		single_line(i, j);
	}
}

void GF_LineStrip(void)
{
	for (size_t i = 0, j = 1; i < bufferLength - 1; i++, j++)
	{
		single_line(i, j);
	}
}

void GF_LineLoop(void)
{
	for (size_t i = 0, j = 1; i < bufferLength - 1; i++, j++)
	{
		single_line(i, j);
	}

	single_line(bufferLength - 1, 0);
}

void GF_Triangles(void)
{
	for (size_t i = 0, j = 1, k = 2; i < bufferLength; i += 3, j += 3, k += 3)
	{
		single_triangle(i, j, k);
	}
}

void GF_TriangleStrip(void)
{
	for (size_t i = 2, j = 1, k = 0; i < bufferLength; i++, j++, k++)
	{
		single_triangle(i, j, k);
	}
}

void GF_TriangleFan(void)
{
	for (size_t i = 1, j = 2; i < bufferLength - 1; i++, j++)
	{
		single_triangle(0, i, j);
	}
}

Vect3 GF_ToNDC(const Vect4 * v)
{
	return flags.proj ? v->ToNDC() : Vect3(v->X, v->Y, v->Z);
}

void GF_ToScreen(Vect3 * v)
{
	v->X = cPort.X * v->X + cPort.X;
	v->Y = cPort.Y * v->Y + cPort.Y;
	v->Z = cPort.Z * v->Z + cPort.W;
}

void single_line(const int i, const int j)
{
	int clp = (hbuffer + i)->Clip() + (hbuffer + j)->Clip();
	if (clp)
	{
		if (!flags.clip || clp > 1) return;
		Line *l = &Line(vrtxat(i), vrtxat(j));
		if (LineClip(l, port)) GF_Line(l);
	}
	else GF_Line(vrtxat(i), vrtxat(j));
}

void single_triangle(const int i, const int j, const int k)
{
	if ((hbuffer + i)->Clip() + (hbuffer + j)->Clip() + (hbuffer + k)->Clip())
	{
		if (!flags.clip) return;
		Trgl *t = &Trgl(vrtxat(i), vrtxat(j), vrtxat(k));
		Poly p;

		if (p.v = TriangleClip(t, &p.polyVLen, port))
		{
			for (size_t l = 1, m = 2; l < p.polyVLen - 1; l++, m++)
			{
				GF_FullTrgl(p.v, p.v + l, p.v + m);
			}

			free(p.v);
		}
	}
	else GF_FullTrgl(vrtxat(i), vrtxat(j), vrtxat(k));
}

void GF_Line(const int x0, const int y0, const int z0, const Color c0, const int x1, const int y1, const int z1, const Color c1)
{
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

	get_zxy_plt;
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
	GF_Line(v0->v.X, v0->v.Y, v0->v.Z, v0->c, v1->v.X, v1->v.Y, v1->v.Z, v1->c);
}

void GF_Line(const Line * line)
{
	GF_Line(line->v0.v.X, line->v0.v.Y, line->v0.v.Z, line->v0.c, line->v1.v.X, line->v1.v.Y, line->v1.v.Z, line->v1.c);
}

void GF_HLine(const float x0, const float z0, const Color c0, const float x1, const float z1, const Color c1, const float y)
{
	get_zxy_plt;

	if (x0 == x1)
	{
		if (z0 > z1) plot(x0, y, z0, c0);
		else plot(x1, y, z1, c1);
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
			plot(x, y, lerp(z1, z0, a), Color::Lerp(c1, c0, a));
		}
	}
}

void GF_HLine(const float x0, const float z0, const float x1, const float z1, const float y, const Color c)
{
	get_zxy_plt;

	if (x0 == x1)
	{
		if (z0 > z1) plot(x0, y, z0, c);
		else plot(x1, y, z1, c);
	}
	else
	{
		if (x0 < x1)
		{
			float dz = (z1 - z0) / (x1 - x0);
			for (float x = x0, z = z0; x <= x1; ++x, z += dz)
			{
				plot(x, y, z, c);
			}
		}
		else
		{
			float dz = (z0 - z1) / (x0 - x1);
			for (float x = x1, z = z1; x <= x0; ++x, z += dz)
			{
				plot(x, y, z, c);
			}
		}
	}
}

void GF_VLine(const float y0, const float z0, const Color c0, const float y1, const float z1, const Color c1, const float x)
{
	get_zxy_plt;

	if (y0 == y1)
	{
		if (z0 > z1) plot(x, y0, z0, c0);
		else plot(x, y1, z1, c1);
	}
	else if (y0 < y1)
	{
		for (float y = y0; x <= y1; ++y)
		{
			float a = invLerp(y0, y1, y);
			plot(x, y, lerp(z0, z1, a), Color::Lerp(c0, c1, a));
		}
	}
	else
	{
		for (float y = y1; x <= y0; ++y)
		{
			float a = invLerp(y1, y0, y);
			plot(x, y, lerp(z1, z0, a), Color::Lerp(c1, c0, a));
		}
	}
}

void GF_VLine(const float y0, const float z0, const float y1, const float z1, const float x, const Color c)
{
	get_zxy_plt;

	if (y0 == y1)
	{
		if (z0 > z1) plot(x, y0, z0, c);
		else plot(x, y1, z1, c);
	}
	else
	{
		if (y0 < y1)
		{
			float dz = (z1 - z0) / (y1 - y0);
			for (float y = y0, z = z0; y <= y1; ++y, z += dz)
			{
				plot(x, y, z, c);
			}
		}
		else
		{
			float dz = (z0 - z1) / (y0 - y1);
			for (float y = y1, z = z1; y <= y0; ++y, z += dz)
			{
				plot(x, y, z, c);
			}
		}
	}
}

void GF_BFTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
	float invSlp0 = (v1->v.X - v0->v.X) / (v1->v.Y - v0->v.Y);
	float invSlp1 = (v2->v.X - v0->v.X) / (v2->v.Y - v0->v.Y);

	float x0 = v0->v.X;
	float x1 = v0->v.X;

	for (int y = v0->v.Y; y <= v1->v.Y; y++)
	{
		float a = invLerp(v0->v.Y, v1->v.Y, y);
		Color c0 = Color::Lerp(v0->c, v1->c, a);
		Color c1 = Color::Lerp(v0->c, v2->c, a);
		float z0 = lerp(v0->v.Z, v1->v.Z, a);
		float z1 = lerp(v0->v.Z, v2->v.Z, a);

		mtx.lock();
		lBuff.push_back(hline(x0, z0, c0, x1, z1, c1, y));
		mtx.unlock();

		x0 += invSlp0;
		x1 += invSlp1;
	}
}

void GF_TFTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
	float invSlp0 = (v2->v.X - v0->v.X) / (v2->v.Y - v0->v.Y);
	float invSlp1 = (v2->v.X - v1->v.X) / (v2->v.Y - v1->v.Y);

	float x0 = v2->v.X;
	float x1 = v2->v.X;

	for (int y = v2->v.Y; y > v0->v.Y; y--)
	{
		float a = invLerp(v2->v.Y, v0->v.Y, y);
		Color c0 = Color::Lerp(v2->c, v0->c, a);
		Color c1 = Color::Lerp(v2->c, v1->c, a);
		float z0 = lerp(v2->v.Z, v0->v.Z, a);
		float z1 = lerp(v2->v.Z, v1->v.Z, a);

		mtx.lock();
		lBuff.push_back(hline(x0, z0, c0, x1, z1, c1, y));
		mtx.unlock();

		x0 -= invSlp0;
		x1 -= invSlp1;
	}
}

void GF_FullTrgl(const Vertex * v0, const Vertex * v1, const Vertex * v2)
{
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

		Vertex vt3 = Vertex(Vect3(x, y, z), Color::Lerp(vt0.c, vt2.c, a));

		GF_BFTrgl(&vt0, &vt3, &vt1);
		GF_TFTrgl(&vt3, &vt1, &vt2);
	}
}

void GF_SetDepth(const float front, const float back)
{
	port.far = back;
	port.near = front;
	cPort.Z = (back - front) * 0.5;
	cPort.W = (back + front) * 0.5;
}

void Raise(const char *msg)
{
	printf("%s\n", msg);
	w->Terminate();
}