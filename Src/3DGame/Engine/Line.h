#pragma once

#include "Vertex.h"

typedef struct Line
{
	vrtx v0, v1;

	Line(void) __GPU : v0(vrtx()), v1(vrtx()) { }
	Line(const vrtx v0, const vrtx v1) __GPU : v0(v0), v1(v1) { }
	Line(const vrtx *v0, const vrtx *v1) __GPU : v0(*v0), v1(*v1) { }

	bool Clip(const ViewPort vp) __GPU
	{
		int m0 = v0.ComputeMask(vp);
		int m1 = v1.ComputeMask(vp);

		while (m0 | m1)
		{
			if (m0 & m1) return false;

			float x, y, z, a;
			int mc = m0 ? m0 : m1;

			if (mc & TOP) y = vp.screen.h;
			else if (mc & BOTTOM) y = vp.screen.y;
			else if (mc & RIGHT) x = vp.screen.w;
			else if (mc & LEFT) x = vp.screen.x;
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
		int w = v1.v.X - v0.v.X;
		int h = v1.v.Y - v0.v.Y;

		int dx0 = w < 0 ? -1 : (w > 0 ? 1 : 0);
		int dy0 = h < 0 ? -1 : (h > 0 ? 1 : 0);
		int dx1 = dx0;
		int dy1 = 0;

		uint lng = abs(w);
		uint shrt = abs(h);
		if (lng <= shrt)
		{
			lng = shrt;
			shrt = abs(w);
			dy1 = h < 0 ? -1 : (h > 0 ? 1 : 0);
			dx1 = 0;
		}

		uint num = lng >> 1;
		uint x = v0.v.X;
		uint y = v0.v.Y;

		for (size_t i = 0; i < lng; i++)
		{
			float a = invLerp(0, lng, i);
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