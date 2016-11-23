#pragma once

#include "Mtrx4.h"
#include "Shapes.h"
#include "GameWindow.h"

#define GF_POINTS			0
#define GF_LINES			1
#define GF_LINE_STRIP		2
#define GF_LINE_LOOP		3
#define GF_LINE_FAN			4
#define GF_TRIANGLES		5
#define GF_TRIANGLE_STRIP	6
#define GF_TRIANGLE_FAN		7

#define OPTMZ_POINTS		0
#define OPTMZ_LINES			1
#define OPTMZ_TRGLS			2

#ifndef ENABLE
#define ENABLE	1
#define DISABLE 0
#endif

void GF_Init(const int optmz);
void GF_End(void);
void GF_SetWindow(GameWindow *window);
void GF_StartRender(const int primitiveType);
int GF_EndRender(void);
void GF_SetBufferLength(const size_t length);
void GF_SetModelMatrix(const Matrix4 *m);
void GF_SetViewMatrix(const Matrix4 *m);
void GF_SetFrustrum(const float fovY, const float aspr, const float front, const float back);
void GF_SetOrthographic(const float width, const float height, const float front, const float back);
void GF_SetViewport(const Rect *rect);
void GF_SetFlag_Clip(const bool value);
void GF_SetFlag_ZBuff(const bool value);
void GF_SetFlagVBuff(const bool value);
void GF_AddPoint(const Vector3 v, const Color c);
void GF_AddPoint(const float x, const float y, const float z, const Color c);
void GF_AddPoint(const Vertex vtx);
Vect3* GF_GetVectBuffer(void);

#ifdef _USE_GF_INTERNAL
typedef struct flags
{
	bool init : 1;
	bool clip : 1;
	bool zBuff : 1;
	bool proj : 1;
	bool strt : 1;
	bool vBuff : 1;
	octet prim : 3;

	flags(void) 
		: init(false)
		, clip(ENABLE)
		, zBuff(ENABLE)
		, proj(ENABLE)
		, strt(false)
		, vBuff(false)
		, prim(GF_POINTS)
	{ }
} Flags;

void GF_SetDepth(const float front, const float back);
void Raise(const char *msg);
void GF_Line(const int x0, const int y0, const int z0, const Color c0, const int x1, const int y1, const int z1, const Color c1);
void GF_Line(const Vertex *v0, const Vertex *v1);
void GF_Line(const Line *line);
void GF_HLine(const float x0, const float z0, const Color c0, const float x1, const float z1, const Color c1, const float y);
void GF_HLine(const float x0, const float z0, const float x1, const float z1, const float y, const Color c);
void GF_VLine(const float y0, const float z0, const Color c0, const float y1, const float z1, const Color c1, const float x);
void GF_VLine(const float y0, const float z0, const float y1, const float z1, const float x, const Color c);
void GF_BFTrgl(const Vertex *v0, const Vertex *v1, const Vertex *v2);
void GF_TFTrgl(const Vertex *v0, const Vertex *v1, const Vertex *v2);
void GF_FullTrgl(const Vertex *v0, const Vertex *v1, const Vertex *v2);
void GF_Points(void);
void GF_Lines(void);
void GF_LineStrip(void);
void GF_LineLoop(void);
void GF_LineFan(void);
void GF_Triangles(void);
void GF_TriangleStrip(void);
void GF_TriangleFan(void);
Vect3 GF_ToNDC(const Vector4 *v);
void GF_ToScreen(Vect3 *v);
inline Vect3 GF_ToScreen(Vect4 *v)
{
	Vect3 r = GF_ToNDC(v);
	GF_ToScreen(&r);
	return r;
}
void mult_point(int i, const int j);
void single_line(const int i, const int j);
void single_triangle(const int i, const int j, const int k);
#endif