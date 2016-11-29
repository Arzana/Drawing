#define _VECT_CONV

#include <cfloat>
#include <cstdio>
#include "GF_WIN_Window.h"
#include "Utils.h"

#define __GPU		restrict(cpu, amp)

using namespace concurrency;

GF_WIN_Window::GF_WIN_Window(const char * title, const uint width, const uint height)
	: WindowsWindow(title, width, height), flags(new Flags()), scrArea(width * height)
	, vp(new vPort(0, 0, width - 1, height - 1, FLT_MAX, 0)), cp(new vect4((width - 1) * 0.5f, (height - 1) * 0.5f, 0, 0))
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
	flags->proj = true;
	proj = mtrx4::CreateFrustrum(fovY, aspr, front, back);
	SetDepth(front, back);
}

void GF_WIN_Window::SetProjection_Orthographic(const float width, const float height, const float front, const float back)
{
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

void GF_WIN_Window::End(void)
{
	if (!flags->start || *buffI < *buffLen)
	{
		if (!flags->start) Raise("GF_WIN_Window::Start must be called before calling GF_WIN_Window::End!");
		if (*buffI < *buffLen) Raise("Not all vertices in the buffer have been set!");
		return;
	}

	mtrx4 mvp = proj * (model * view);
	mtrx4::Transform(&mvp, vBuff, hBuff, *buffLen);

	switch (flags->prim)
	{
	case GF_POINTS:
		GF_Points();
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
	printf("%s\n", msg);
	*isRunning = false;
}

void GF_WIN_Window::ResetZBuff(void)
{
	array_view<float, 1> arr_z(scrArea, zBuff);

	parallel_for_each(
		arr_z.extent,
		[=](index<1> i) __GPU_ONLY
	{
		arr_z[i] = FLT_MIN;
	});
}

vect3 GF_WIN_Window::ToScreen(const vect4 v, const vect4 cp, const bool proj) __GPU
{
	vect3 result = proj ? v.ToNDC() : V4ToV3(&v);
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

	parallel_for_each(
		arr_h.extent,
		[=](index<1> i) __GPU_ONLY
	{
		if (arr_h[i].Clip()) return;
		vect3 scr = GF_WIN_Window::ToScreen(arr_h[i], port, proj);
		int pI = xy2i(ipart(scr.X), ipart(scr.Y), w);

		if (arr_z[pI] > scr.Z) return;
		arr_z[pI] = scr.Z;
		arr_pix[pI] = arr_c[i];
	});
}