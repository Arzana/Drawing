#pragma once

#include "Shapes.h"
#include "Window.h"

#define GF_POINTS			0
#define GF_LINES			1
#define GF_LINE_STRIP		2
#define GF_LINE_LOOP		3
#define GF_TRIANGLES		4
#define GF_TRIANGLE_STRIP	5
#define GF_TRIANGLE_FAN		6

void GF_SetWindow(GameWindow *window);
void GF_StartRender(const int primitiveType);
void GF_EndRender(void);
void GF_SetBufferLength(const size_t length);
void GF_AddPoint(const Vector3 v, const Color c);
void GF_AddPoint(const float x, const float y, const float z, const Color c);

#ifdef _USE_GF_INTERNAL
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
#endif