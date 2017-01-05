#pragma once

#include "Vertex.h"

#ifndef __GPU
#define __GPU				restrict(cpu, amp)
#endif
#ifndef __GPU_ONLY
#define __GPU_ONLY			restrict(amp)
#endif
#ifndef __CPU_ONLY
#define __CPU_ONLY
#endif

typedef struct Line
{
	vrtx v0, v1;

	Line(void) __GPU : v0(vrtx()), v1(vrtx()) { }
	Line(vrtx v0, vrtx v1) __GPU : v0(v0), v1(v1) { }

	bool Clip(ViewPort vp) __GPU
	{
		int m0 = v0.ComputeMask(vp);
		int m1 = v1.ComputeMask(vp);

		while (m0 | m1)
		{
			if (m0 & m1) return false;

			float x, y, z, a;
			int mc = m0 ? m0 : m1;

			if (mc & TOP) y = float(vp.screen.h);
			else if (mc & BOTTOM) y = float(vp.screen.y);
			else if (mc & RIGHT) x = float(vp.screen.w);
			else if (mc & LEFT) x = float(vp.screen.x);
			else if (mc & NEAR) z = vp.near;
			else if (mc & FAR) z = vp.far;

			if (mc & VERTICAL)
			{
				a = invLerp(v0.v.Y, v1.v.Y, y);
				z = lerp(v0.v.Z, v1.v.Z, a);
				x = lerp(v0.v.X, v1.v.X, a);
			}
			else if (mc & HORIZONTAL)
			{
				a = invLerp(v0.v.X, v1.v.X, x);
				z = lerp(v0.v.Z, v1.v.Z, a);
				y = lerp(v0.v.Y, v1.v.Y, a);
			}
			else if (mc & DEPTH)
			{
				a = invLerp(v0.v.Z, v1.v.Z, z);
				x = lerp(v0.v.X, v1.v.X, a);
				y = lerp(v0.v.Y, v1.v.Y, a);
			}

			if (mc == m0)
			{
				v0.v = vect3(x, y, z);
				v0.c = Color::Lerp(v0.c, v1.c, a);
				m0 = v0.ComputeMask(vp);
			}
			else
			{
				v1.v = vect3(x, y, z);
				v1.c = Color::Lerp(v0.c, v1.c, a);
				m1 = v1.ComputeMask(vp);
			}
		}

		return true;
	}

	template <typename _Func>
	void Render(const _Func plot) const __GPU
	{
		float w = v1.v.X - v0.v.X;
		float h = v1.v.Y - v0.v.Y;

		int dx0 = w < 0 ? -1 : (w > 0 ? 1 : 0);
		int dy0 = h < 0 ? -1 : (h > 0 ? 1 : 0);
		int dx1 = dx0;
		int dy1 = 0;

		float lng = abs(w);
		float shrt = abs(h);
		if (lng <= shrt)
		{
			lng = shrt;
			shrt = abs(w);
			dy1 = h < 0 ? -1 : (h > 0 ? 1 : 0);
			dx1 = 0;
		}

		float num = lng / 2.0f;
		int x = ipart(v0.v.X);
		int y = ipart(v0.v.Y);

		for (float i = 0; i < lng; i++)
		{
			float a = invLerp(0.0f, lng, i);
			plot(x, y, lerp(v0.v.Z, v1.v.Z, a), clr::Lerp(v0.c, v1.c, a));
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
} line;