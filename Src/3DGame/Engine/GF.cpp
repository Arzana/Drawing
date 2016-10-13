#include "Utils.h"
#include "GF.h"
#include <cstdio>

#define vrtxAt(i)	Vertex(vbuffer[(i)], cbuffer[(i)])

void Raise(const char *msg);
void GF_Line(const int x0, const int y0, const Color c0, const int x1, const int y1, const Color c1);
void GF_Trgl(const Vertex v0, const Vertex v1, const Vertex v2);
void GF_Points(void);
void GF_Lines(void);
void GF_LineStrip(void);
void GF_LineLoop(void);
void GF_Triangles(void);
void GF_TriangleStrip(void);
void GF_TriangleFan(void);

GameWindow *w = NULL;

int type = GF_POINTS;
int startCalled = 0;

size_t bufferLength;
size_t bufferIndex;

Vect2 *vbuffer = NULL;
Color *cbuffer = NULL;

void GF_SetWindow(GameWindow * window)
{
	w = window;
}

void GF_StartRender(const int primitiveType)
{
	if (!w || startCalled)
	{
		Raise(!w ? "Window not specified!" : "GF_EndRender must be called before calling GF_StartRender again!");
		return;
	}

	type = primitiveType;
	startCalled = 1;
}

void GF_EndRender(void)
{
	if (!startCalled || bufferIndex < bufferLength)
	{
		if (!startCalled) Raise("GF_StartRender must be called before calling GF_EndRender!");
		if (bufferIndex < bufferLength) Raise("Not all points in the buffer have been set!");
		return;
	}

	startCalled = 0;
	bufferIndex = 0;

	switch (type)
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
		return;
	}

	bufferLength = 0;
	free(vbuffer);
	free(cbuffer);
}

void GF_SetBufferLength(size_t length)
{
	if (!startCalled || length < 1)
	{
		if (!startCalled) Raise("GF_StartRender must be called before calling GF_SetBufferLength!");
		if (length < 1) Raise("Length must be greater than zero!");
		return;
	}

	bufferLength = length;
	vbuffer = malloc_s(Vect2, length);
	cbuffer = malloc_s(Color, length);
}

void GF_AddPoint(const float x, const float y, const Color c)
{
	if (!startCalled || bufferIndex >= bufferLength)
	{
		if (!startCalled) Raise("GF_StartRender must be called before calling GF_AddPoint!");
		if (bufferLength < 1) Raise("GF_SetBufferLength must be called before calling GF_AddPoint!");
		if (bufferIndex >= bufferLength) Raise("Cannot add any more points to the buffer!");
	}

	vbuffer[bufferIndex] = Vect2(x, y);
	cbuffer[bufferIndex] = c;
	++bufferIndex;
}

void GF_Points(void)
{
	for (size_t i = 0; i < bufferLength; i++)
	{
		w->Plot(&vbuffer[i], cbuffer[i]);
	}
}

void GF_Lines(void)
{
	for (size_t i = 0; i < bufferLength; i += 2)
	{
		Vect2 v0 = vbuffer[i], v1 = vbuffer[i + 1];
		GF_Line(v0.X, v0.Y, cbuffer[i], v1.X, v1.Y, cbuffer[i + 1]);
	}
}

void GF_LineStrip(void)
{
	for (size_t i = 0; i < bufferLength - 1; i++)
	{
		Vect2 v0 = vbuffer[i], v1 = vbuffer[i + 1];
		GF_Line(v0.X, v0.Y, cbuffer[i], v1.X, v1.Y, cbuffer[i + 1]);
	}
}

void GF_LineLoop(void)
{
	for (size_t i = 0; i < bufferLength - 1; i++)
	{
		Vect2 v0 = vbuffer[i], v1 = vbuffer[i + 1];
		GF_Line(v0.X, v0.Y, cbuffer[i], v1.X, v1.Y, cbuffer[i + 1]);
	}

	Vect2 v0 = vbuffer[bufferLength - 1], v1 = vbuffer[0];
	GF_Line(v0.X, v0.Y, cbuffer[bufferLength - 1], v1.X, v1.Y, cbuffer[0]);
}

void GF_Triangles(void)
{
	for (size_t i = 0; i < bufferLength; i += 3)
	{
		GF_Trgl(vrtxAt(i), vrtxAt(i + 1), vrtxAt(i + 2));
	}
}

void GF_TriangleStrip(void)
{
	for (size_t i = 2; i < bufferLength; i++)
	{
		GF_Trgl(vrtxAt(i - 2), vrtxAt(i - 1), vrtxAt(i));
	}
}

void GF_TriangleFan(void)
{
	for (size_t i = 1; i < bufferLength - 1; i++)
	{
		GF_Trgl(vrtxAt(0), vrtxAt(i), vrtxAt(i + 1));
	}
}

void GF_Line(const int x0, const int y0, const Color c0, const int x1, const int y1, const Color c1)
{
	if (!w) return;

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
		dx1 = 1;
	}

	uint num = lng >> 1;
	uint x = x0;
	uint y = y0;

	for (size_t i = 0; i < lng; i++)
	{
		float a = invLerp(0, lng, i);
		w->Plot(x, y, Color::Lerp(c0, c1, a));
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

void GF_Trgl(const Vertex v0, const Vertex v1, const Vertex v2)
{
	uint maxX = max(v0.v.X, max(v1.v.X, v2.v.X));
	uint minX = min(v0.v.X, min(v1.v.X, v2.v.X));
	uint maxY = max(v0.v.Y, max(v1.v.Y, v2.v.Y));
	uint minY = min(v0.v.Y, min(v1.v.Y, v2.v.Y));

	Vect2 vs0 = Vect2(v1.v.X - v0.v.X, v1.v.Y - v0.v.Y);
	Vect2 vs1 = Vect2(v2.v.X - v0.v.X, v2.v.Y - v0.v.Y);

	for (size_t x = minX; x <= maxX; x++)
	{
		for (size_t y = minY; y <= maxY; y++)
		{
			Vect2 q = Vect2(x - v0.v.X, y - v0.v.Y);
			float s = Vect2::PrepDot(&q, &vs1) / Vect2::PrepDot(&vs0, &vs1);
			float t = Vect2::PrepDot(&vs0, &q) / Vect2::PrepDot(&vs0, &vs1);

			if (s >= 0 && t >= 0 && s + t <= 1)
			{
				Color c4 = Color::Lerp(v1.c, v2.c, invLerp(minX, maxX, x));
				Color c6 = Color::Lerp(v0.c, c4, invLerp(minY, maxY, y));

				w->Plot(x, y, c6);
			}
		}
	}
}

void Raise(const char *msg)
{
	printf(msg);
	printf("\n");
	w->Terminate();
}