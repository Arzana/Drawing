#pragma once

#include "Mtrx4.h"
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
void GF_SetModelMatrix(const Matrix4 *m);
void GF_SetViewMatrix(const Matrix4 *m);
void GF_SetFrustrum(const float fovY, const float aspr, const float front, const float back);
void GF_SetOrthographic(const float width, const float height, const float front, const float back);
void GF_SetViewport(const Rectangle *rect);
void GF_SetFlag_Clip(const bool value);
void GF_SetFlag_ZBuff(const bool value);
void GF_AddPoint(const Vector3 v, const Color c);
void GF_AddPoint(const float x, const float y, const float z, const Color c);
void GF_AddPoint(const Vertex vtx);

#ifdef _USE_GF_INTERNAL
typedef struct flags
{
	bool Clip : 1;
	bool ZBuff : 1;
	bool Proj : 1;
	bool Strt : 1;
	octet Prim : 3;

	flags(void) : Clip(ENABLE), Prim(GF_POINTS), Proj(ENABLE), Strt(DISABLE), ZBuff(ENABLE) { }
} Flags;

void GF_SetDepth(const float front, const float back);
void Raise(const char *msg);
void GF_Line(const int x0, const int y0, const int z0, const Color c0, const int x1, const int y1, const int z1, const Color c1);
void GF_Line(const Vertex *v0, const Vertex *v1);
void GF_Line(const Line *line);
void GF_HLine(const float x0, const float z0, const Color c0, const float x1, const float z1, const Color c1, const float y);
void GF_HLine(const Vertex *v0, const Vertex *v1);
void GF_BFTrgl(const Vertex *v0, const Vertex *v1, const Vertex *v2);
void GF_TFTrgl(const Vertex *v0, const Vertex *v1, const Vertex *v2);
void GF_FullTrgl(const Vertex *v0, const Vertex *v1, const Vertex *v2);
void GF_Points(void);
void GF_Lines(void);
void GF_LineStrip(void);
void GF_LineLoop(void);
void GF_Triangles(void);
void GF_TriangleStrip(void);
void GF_TriangleFan(void);
Vect3 GF_ToNDC(const Vector4 *v);
void GF_ToScreen(Vect3 *v);
Vect3 GF_ToScreen(Vect4 *v);
void single_line(const int i, const int j);
void single_triangle(const int i, const int j, const int k);
#endif