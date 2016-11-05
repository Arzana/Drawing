#define _USE_CLIPPING

#define vect(vrtx)		l->vrtx.v
#define clr(vrtx)		l->vrtx.c
#define vx(vrtx)		vect(vrtx).X
#define vy(vrtx)		vect(vrtx).Y
#define vz(vrtx)		vect(vrtx).Z

#include "Shapes.h"

Vertex::Vertex(Vect3 v, Color c)
	: v(v), c(c)
{ }

Vertex::Vertex(float x, float y, float z, Color c)
	: v(Vect3(x, y, z)), c(c)
{ }

Line::Line(Vertex v0, Vertex v1)
	: v0(v0), v1(v1)
{ }

Line::Line(const Vertex * v0, const Vertex * v1)
	: v0(*v0), v1(*v1)
{ }

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2)
	: v0(v0), v1(v1), v2(v2)
{ }

Triangle::Triangle(const Vertex * v0, const Vertex * v1, const Vertex * v2)
	: v0(*v0), v1(*v1), v2(*v2)
{ }

Rectangle::Rectangle(void)
	: x(0), y(0), w(0), h(0)
{ }

Rectangle::Rectangle(int x, int y, int w, int h)
	: x(x), y(y), w(w), h(h)
{ }

ViewPort::ViewPort(Rectangle screen, float far, float near)
	: screen(screen), far(far), near(near)
{ }

ViewPort::ViewPort(int x, int y, int w, int h, float f, float n)
	: screen(x, y, w, h), far(f), near(n)
{ }

int ComputeMask(const Vect3 v, const ViewPort vp)
{
	int code = INSIDE;

	if (v.X < vp.screen.x) code |= LEFT;
	else if (v.X > vp.screen.w) code |= RIGHT;
	if (v.Y < vp.screen.y) code |= BOTTOM;
	else if (v.Y > vp.screen.h) code |= TOP;
	if (v.Z < vp.near) code |= NEAR;
	else if (v.Z > vp.far) code |= FAR;

	return code;
}

bool LineClip(Line * l, const ViewPort vp)
{
	int m0 = ComputeMask(vect(v0), vp);
	int m1 = ComputeMask(vect(v1), vp);

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
			a = invLerp(vy(v0), vy(v1), y);
			z = lerp(vz(v0), vz(v1), a);
			x = lerp(vx(v0), vx(v1), a);
		}
		else if (mc & HORIZONTAL)
		{
			a = invLerp(vx(v0), vx(v1), x);
			z = lerp(vz(v0), vz(v1), a);
			y = lerp(vy(v0), vy(v1), a);
		}
		else if (mc & DEPTH)
		{
			a = invLerp(vz(v0), vz(v1), z);
			x = lerp(vx(v0), vx(v1), a);
			y = lerp(vy(v0), vy(v1), a);
		}

		if (mc == m0)
		{
			vect(v0) = Vect3(x, y, z);
			clr(v0) = Color::Lerp(clr(v0), clr(v1), a);
			m0 = ComputeMask(vect(v0), vp);
		}
		else
		{
			vect(v1) = Vect3(x, y, z);
			clr(v1) = Color::Lerp(clr(v0), clr(v1), a);
			m1 = ComputeMask(vect(v1), vp);
		}
	}

	return true;
}

bool TriangleClip(Triangle * t, int * len)
{
	return false;
}

template <typename T>
void Swap(T *v0, T *v1)
{
	T temp = *v0;
	*v0 = *v1;
	*v1 = temp;
}

void SortVerticesByY(Vertex * v0, Vertex * v1, Vertex * v2)
{
	if (v0->v.Y > v1->v.Y) Swap(v0, v1);
	if (v0->v.Y > v2->v.Y) Swap(v0, v2);
	if (v1->v.Y > v2->v.Y) Swap(v1, v2);
}
