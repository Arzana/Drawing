#define _VECT_CONV

#include <cfloat>
#include <cstdio>
#include <ppl.h>
#include "GF_WIN_Window.h"
#include "Line.h"
#include "Triangle.h"
#include "Polygon.h"
#include "WinLogger.h"

#define vrtxat(x, name)	gfWinWnd::ToScreen(&p.vertexes[x].v, *cp, flags->proj); \
						vrtx name(p.vertexes[x].v, p.vertexes[x].c)


using namespace concurrency;

GF_WIN_Window::GF_WIN_Window(const char * title, uint width, uint height)
	: WindowsWindow(title, width, height), flags(new Flags()), scrArea(width * height)
	, vp(new vPort(width - 1, height - 1, FLT_MAX, 0)), cp(new vect4((width - 1) * 0.5f, (height - 1) * 0.5f, 0, 0))
	, buffLen(new size_t(0)), buffI(new size_t(0))
	, cBuff(NULL), hBuff(NULL), zBuff(malloc_s(float, scrArea))
	, model(MTRX4_IDENTITY), view(MTRX4_IDENTITY), proj(MTRX4_IDENTITY)
{ }

GF_WIN_Window::~GF_WIN_Window(void)
{
	delete_s(flags);
	delete_s(vp);
	delete_s(cp);
	delete_s(buffLen);
	delete_s(buffI);
	free_s(zBuff);

	if (cBuff) free_s(cBuff);
	if (hBuff) free_s(hBuff);
}

void GF_WIN_Window::SetProjection_Frustrum(float fovY, float aspr, float front, float back)
{
	LogMsg_GF("Projection set as frustrum.");
	flags->proj = true;
	proj = mtrx4::CreateFrustrum(fovY, aspr, front, back);
	SetDepth(front, back);
}

void GF_WIN_Window::SetProjection_Orthographic(float width, float height, float front, float back)
{
	LogMsg_GF("Projection set as orthographic.");
	flags->proj = false;
	proj = mtrx4::CreateOrthographic(width, height, front, back);
	SetDepth(front, back);
}

void GF_WIN_Window::SetBufferLength(size_t length)
{
	if (length < 0 || *buffLen > 0)
	{
		if (length < 1) Raise("Length must be greater than zero!");
		if (*buffLen != 0) Raise("EndRender must be called before respecifying the buffer length!");
		return;
	}

	*buffLen = length;
	cBuff = malloc_s(Color, length);
	hBuff = malloc_s(vect4, length);
}

void GF_WIN_Window::Start(octet primitiveType)
{
	if (flags->start)
	{
		Raise("GF_WIN_Window::End must be called before calling GF_WIN_Window::Start again!");
		return;
	}

	flags->start = true;
	flags->prim = primitiveType;
}

bool GF_WIN_Window::End(void)
{
	if (!flags->start || *buffI < *buffLen)
	{
		if (!flags->start) Raise("GF_WIN_Window::Start must be called before calling GF_WIN_Window::End!");
		if (*buffI < *buffLen) Raise("Not all vertices in the buffer have been set!");
		return false;
	}

	mtrx4 mvp = proj * (model * view);
	mtrx4::Transform(&mvp, hBuff, *buffLen);

	switch (flags->prim)
	{
	case GF_POINTS:
		GF_Points();
		break;
	case GF_LINE_FAN:
		GF_LineFan();
		break;
	case GF_TRIANGLES:
		GF_Triangles();
		break;
	default:
		Raise("Unknown primitive type requested!");
		break;
	}

	model = MTRX4_IDENTITY;
	*buffI = 0;
	*buffLen = 0;
	free_s(cBuff);
	free_s(hBuff);

	flags->start = false;
	return true;
}

void GF_WIN_Window::AddVertex(vect4 v, clr c)
{
	if (*buffI >= *buffLen)
	{
		if (*buffLen < 1) Raise("GF_WIN_Window::SetBufferLength must be called before calling GF_WIN_Window::AddVertex!");
		if (*buffI >= *buffLen) Raise("Cannot add any more vertices to the buffer!");
		return;
	}

	hBuff[*buffI] = v;
	cBuff[(*buffI)++] = c;
}

void GF_WIN_Window::Clear(clr c)
{
	array_view<clr, 1> arr_pix(scrArea, (clr*)pixels);
	const clr clearColor = c;

	parallel_for_each(
		arr_pix.extent,
		[=](index<1> i) __GPU_ONLY
	{
		arr_pix[i] = clearColor;
	});
}

void GF_WIN_Window::SetDepth(float front, float back)
{
	vp->far = back;
	vp->near = front;
	cp->Z = (back - front) * 0.5f;
	cp->W = (back + front) * 0.5f;
}

void GF_WIN_Window::Raise(const char * msg)
{
	LogErr_GF(msg);
	*isRunning = false;
}

void GF_WIN_Window::ResetZBuff(void)
{
	array_view<float, 1> arr_z(scrArea, zBuff);

	parallel_for_each(
		arr_z.extent,
		[=](index<1> i) __GPU_ONLY
	{
		arr_z[i] = FLT_MAX;
	});
}

void GF_WIN_Window::ToScreen(vect4 *v, vect4 cp, bool proj) __GPU
{
	if (v->W == 0) return;
	if (proj) v->ToNDC();
	v->X = cp.X * v->X + cp.X;
	v->Y = cp.Y * v->Y + cp.Y;
	v->Z = cp.Z * v->Z + cp.W;
	v->W = 0;
}

void GF_WIN_Window::GF_Points(void)
{
	array_view<clr, 1> arr_pix(scrArea, (clr*)pixels);
	array_view<float, 1> arr_z(scrArea, zBuff);
	array_view<vect4, 1> arr_h(*buffLen, hBuff);
	array_view<clr, 1> arr_c(*buffLen, cBuff);

	const bool proj = flags->proj;
	const vect4 port = *cp;
	const size_t w = width;

	parallel_for_each(arr_h.extent,
		[=](index<1> i) __GPU_ONLY
	{
		vect4 h = arr_h[i];
		if (h.Clip()) return;
		gfWinWnd::ToScreen(&h, port, proj);
		index<1> pI(xy2i(ipart(h.X), ipart(h.Y), w));

		if (arr_z[pI] < h.Z) return;
		arr_z[pI] = h.Z;
		arr_pix[pI] = arr_c[i];
	});
}

void GF_WIN_Window::GF_LineFan(void)
{
	array_view<clr, 1> arr_pix(scrArea, (clr*)pixels);
	array_view<float, 1> arr_z(scrArea, zBuff);
	array_view<vect4, 1> arr_h(*buffLen - 1, hBuff + 1);
	array_view<clr, 1> arr_c(*buffLen - 1, cBuff + 1);

	const bool proj = flags->proj, clip = flags->clip;
	const vect4 port = *cp;
	const vPort vport = *vp;
	const size_t w = width;

	vect4 h0 = hBuff[0];
	gfWinWnd::ToScreen(&h0, port, proj);
	const vrtx v0 = vrtx(V4ToV3(h0), cBuff[0]);

	parallel_for_each(arr_h.extent,
		[=](index<1> i) __GPU_ONLY
	{
		vect4 h1 = arr_h[i];
		if (h0.Clip() || h1.Clip())
		{
			if (!clip) return;
			gfWinWnd::ToScreen(&h1, port, proj);
			Line l = Line(v0, vrtx(V4ToV3(h1), arr_c[i]));
			if (l.Clip(vport))
			{
				l.Render(
					[=](uint x, uint y, float z, clr c) __GPU_ONLY
				{
					index<1> pI(xy2i(ipart(x), ipart(y), w));

					if (arr_z[pI] < z) return;
					arr_z[pI] = z;
					arr_pix[pI] = c;
				});
			}
		}
		else
		{
			gfWinWnd::ToScreen(&h1, port, proj);
			Line(v0, vrtx(V4ToV3(h1), arr_c[i])).Render(
				[=](uint x, uint y, float z, clr c) __GPU_ONLY
			{
				index<1> pI(xy2i(ipart(x), ipart(y), w));

				if (arr_z[pI] < z) return;
				arr_z[pI] = z;
				arr_pix[pI] = c;
			});
		}
	});
}

void GF_WIN_Window::GF_Triangles(void)
{
	array_view<clr, 1> arr_pix(scrArea, (clr*)pixels);
	array_view<float, 1> arr_z(scrArea, zBuff);
	const uint w = width;

	parallel_for(size_t(0), *buffLen, size_t(3),
		[&](size_t i) __CPU_ONLY
	{
		size_t j = i + 1, k = i + 2;
		poly p = { 3, { { hBuff[i], cBuff[i] }, { hBuff[j], cBuff[j] }, { hBuff[k], cBuff[k] } } };
		if (hBuff[i].Clip() || hBuff[j].Clip() || hBuff[k].Clip()) ClipPoly(&p);

		if (p.vrtxCount < 3) return;
		vrtxat(0, vrtx0);

		parallel_for(size_t(2), size_t(p.vrtxCount), size_t(1),
			[&](size_t m) __CPU_ONLY
		{
			vrtxat(m - 1, vrtx1);
			vrtxat(m, vrtx2);

			trgl cur(vrtx0, vrtx1, vrtx2);
			int minX = (int)ceilf(min3(cur.v0.v.X, cur.v1.v.X, cur.v2.v.X));
			int minY = (int)ceilf(min3(cur.v0.v.Y, cur.v1.v.Y, cur.v2.v.Y));
			int maxX = (int)ceilf(max3(cur.v0.v.X, cur.v1.v.X, cur.v2.v.X));
			int maxY = (int)ceilf(max3(cur.v0.v.Y, cur.v1.v.Y, cur.v2.v.Y));

			int bW = max(1, maxX - minX);
			array_view<clr, 1> arr_box(bW * max(1, maxY - minY), (clr*)pixels);

			parallel_for_each(arr_box.extent,
				[=](index<1> idx) __GPU_ONLY
			{
				vrtx v = vrtx(float(minX + i2x(idx[0], bW)), float(minY + i2y(idx[0], bW)), 0.0f, CLR_BLACK);
				if (cur.IsInside(&v))
				{
					index<1> pI(xy2i(ipart(v.v.X), ipart(v.v.Y), w));
					if (arr_z[pI] < v.v.Z) return;
					arr_z[pI] = v.v.Z;
					arr_pix[pI] = v.c;
				}
			});
		});
	});
}