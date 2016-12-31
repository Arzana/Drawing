#define _VECT_CONV

#include <cfloat>
#include <cstdio>
#include <ppl.h>
#include "GF_WIN_Window.h"
#include "Line.h"
#include "Triangle.h"
#include "Shapes.h"
#include "WinLogger.h"

#define vrtxat(x)	vrtx(gfWinWnd::ToScreen(cur.vertexes[x].v, *cp, flags->proj), cur.vertexes[x].c)

using namespace concurrency;

GF_WIN_Window::GF_WIN_Window(const char * title, const uint width, const uint height)
	: WindowsWindow(title, width, height), flags(new Flags()), scrArea(width * height)
	, vp(new vPort(width - 1, height - 1, FLT_MAX, 0)), cp(new vect4((width - 1) * 0.5f, (height - 1) * 0.5f, 0, 0))
	, buffLen(new size_t(0)), buffI(new size_t(0))
	, vBuff(NULL), cBuff(NULL), hBuff(NULL), zBuff(malloc_s(float, scrArea))
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

	if (vBuff) free_s(vBuff);
	if (cBuff) free_s(cBuff);
	if (hBuff) free_s(hBuff);
}

void GF_WIN_Window::SetProjection_Frustrum(const float fovY, const float aspr, const float front, const float back)
{
	LogMsg_GF("Projection set as frustrum.");
	flags->proj = true;
	proj = mtrx4::CreateFrustrum(fovY, aspr, front, back);
	SetDepth(front, back);
}

void GF_WIN_Window::SetProjection_Orthographic(const float width, const float height, const float front, const float back)
{
	LogMsg_GF("Projection set as orthographic.");
	flags->proj = false;
	proj = mtrx4::CreateOrthographic(width, height, front, back);
	SetDepth(front, back);
}

void GF_WIN_Window::SetBufferLength(const size_t length)
{
	if (length < 0 || *buffLen > 0)
	{
		if (length < 1) Raise("Length must be greater than zero!");
		if (*buffLen != 0) Raise("EndRender must be called before respecifying the buffer length!");
		return;
	}

	*buffLen = length;
	vBuff = malloc_s(vect3, length);
	cBuff = malloc_s(Color, length);
	hBuff = malloc_s(vect4, length);
}

void GF_WIN_Window::Start(const octet primitiveType)
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
	mtrx4::Transform(&mvp, vBuff, hBuff, *buffLen);

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

	if (!flags->vBuff)
	{
		model = MTRX4_IDENTITY;
		*buffI = 0;
		*buffLen = 0;
		free_s(vBuff);
		free_s(cBuff);
		free_s(hBuff);
	}

	flags->start = false;
	return true;
}

void GF_WIN_Window::AddVertex(const vect3 v, const clr c)
{
	if (*buffI >= *buffLen)
	{
		if (*buffLen < 1) Raise("GF_WIN_Window::SetBufferLength must be called before calling GF_WIN_Window::AddVertex!");
		if (*buffI >= *buffLen) Raise("Cannot add any more vertices to the buffer!");
		return;
	}

	vBuff[*buffI] = v;
	cBuff[(*buffI)++] = c;
}

void GF_WIN_Window::Clear(const clr c)
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

void GF_WIN_Window::SetFlag_VertexBuffering(const bool value)
{
	if (value == flags->vBuff) return;
	LogMsg_GF("Flag vertex buffering turned %s.", value ? "on" : "off");

	if (flags->vBuff && !value)
	{
		*buffI = 0;
		*buffLen = 0;
		free_s(vBuff);
		free_s(cBuff);
		free_s(hBuff);
	}

	flags->vBuff = value;
}

void GF_WIN_Window::SetDepth(float front, float back)
{
	vp->far = back;
	vp->near = front;
	cp->Z = (back - front) * 0.5;
	cp->W = (back + front) * 0.5;
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

vect3 GF_WIN_Window::ToScreen(const vect4 v, const vect4 cp, const bool proj) __GPU
{
	vect3 result = proj ? v.ToNDC() : V4ToV3(v);
	result.X = cp.X * result.X + cp.X;
	result.Y = cp.Y * result.Y + cp.Y;
	result.Z = cp.Z * result.Z + cp.W;
	return result;
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
		vect3 coord = gfWinWnd::ToScreen(h, port, proj);
		index<1> pI(xy2i(ipart(coord.X), ipart(coord.Y), w));

		if (arr_z[pI] < coord.Z) return;
		arr_z[pI] = coord.Z;
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

	const vect4 h0 = hBuff[0];
	const vrtx v0 = vrtx(gfWinWnd::ToScreen(h0, port, proj), cBuff[0]);

	parallel_for_each(arr_h.extent,
		[=](index<1> i) __GPU_ONLY
	{
		vect4 h1 = arr_h[i];
		if (h0.Clip() || h1.Clip())
		{
			if (!clip) return;
			vect3 coord1 = gfWinWnd::ToScreen(h1, port, proj);
			Line l = Line(v0, vrtx(coord1, arr_c[i]));
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
			vect3 coord1 = gfWinWnd::ToScreen(h1, port, proj);
			Line(v0, vrtx(coord1, arr_c[i])).Render(
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
	size_t pLen = *buffLen / 3, tLen = 0;		// Initialize initial polygon buffer
	poly *polies = malloc_s(poly, pLen);

	for (size_t i(0), j(1), k(2), m(0); i < *buffLen; i += 3, j += 3, k += 3, m++)
	{
		polies[m] = { 3,{ { hBuff[i], cBuff[i] }, { hBuff[j], cBuff[j] }, { hBuff[k], cBuff[k] } } };
		if (hBuff[i].Clip() || hBuff[j].Clip() || hBuff[k].Clip()) ClipPoly(&polies[m]);
		tLen += polies[m].TrglLen();
	}

	if (!pLen)
	{	// If nothing needs to be drawn exit here
		free_s(polies);
		return;
	}

	// Initialize final buffer
	trgl *trgls = malloc_s(trgl, tLen);
	for (size_t i = 0, j = 0; i < pLen; i++)
	{	// Concatinate polygons into final buffer
		poly cur = polies[i];
		if (cur.vrtxCount < 3) continue;

		vrtx vrtx0 = vrtxat(0);
		for (size_t k = 2; k < cur.vrtxCount; k++)
		{
			vrtx vrtx1 = vrtxat(k - 1);
			vrtx vrtx2 = vrtxat(k);
			trgls[j++] = trgl(vrtx0, vrtx1, vrtx2);
		}
	}

	array_view<clr, 1> arr_pix(scrArea, (clr*)pixels);	// Start moving rending buffers to the GPU
	array_view<float, 1> arr_z(scrArea, zBuff);
	const uint w = width;

	parallel_for(size_t(0), tLen, size_t(1),
		[&](size_t i) __CPU_ONLY
	{	// Create bounding box for triangle
		trgl cur = trgls[i];
		int minX = ceilf(min3(cur.v0.v.X, cur.v1.v.X, cur.v2.v.X));
		int minY = ceilf(min3(cur.v0.v.Y, cur.v1.v.Y, cur.v2.v.Y));
		int maxX = ceilf(max3(cur.v0.v.X, cur.v1.v.X, cur.v2.v.X));
		int maxY = ceilf(max3(cur.v0.v.Y, cur.v1.v.Y, cur.v2.v.Y));

		int bW = max(1, maxX - minX);	// Create special buffer for rasterizing the traingle.
		array_view<clr, 1> arr_box(bW * max(1, maxY - minY), (clr*)pixels);

		parallel_for_each(arr_box.extent,
			[=](index<1> idx) __GPU_ONLY
		{	// Get the current vertex
			vrtx v = vrtx(minX + i2x(idx[0], bW), minY + i2y(idx[0], bW), 0, CLR_BLACK);
			if (cur.IsInside(&v))
			{	// If the vertex if inside the triangle check depth buffer and add to screen buffer
				index<1> pI(xy2i(ipart(v.v.X), ipart(v.v.Y), w));
				if (arr_z[pI] < v.v.Z) return;
				arr_z[pI] = v.v.Z;
				arr_pix[pI] = v.c;
			}
		});
	});

	free_s(polies);
	free_s(trgls);
}