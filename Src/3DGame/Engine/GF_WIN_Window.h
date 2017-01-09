#pragma once

#include <amp.h>
#include <vector>
#include "WindowsWindow.h"
#include "Vertex.h"
#include "Rectangle.h"
#include "Mtrx4.h"

#define GF_POINTS			0
#define GF_LINES			1
#define GF_LINE_STRIP		2
#define GF_LINE_LOOP		3
#define GF_LINE_FAN			4
#define GF_TRIANGLES		5
#define GF_TRIANGLE_STRIP	6
#define GF_TRIANGLE_FAN		7

typedef struct GF_WIN_Window
	: public winWnd
{
	friend struct Game_WIN;

public:
	GF_WIN_Window(const char *title, uint width, uint height);
	~GF_WIN_Window(void);

	void SetProjection_Frustrum(float fovY, float aspr, float front, float back);
	void SetProjection_Orthographic(float width, float height, float front, float back);
	inline void SetView(const mtrx4 *mtrx) { view = *mtrx; }
	inline void SetModel(const mtrx4 *mtrx) { model = *mtrx; }
	void SetBufferLength(size_t length);
	void Start(octet primitiveType);
	bool End(void);
	void AddVertex(vect4 v, clr c);
	void Clear(clr c);
protected:
	inline void SetFlag_ZBuffering(bool value) { flags->zBuff = value; }
	inline void SetFlag_Clipping(bool value) { flags->clip = value; }
private:
	struct Flags
	{
		bool zBuff : 1;
		bool clip : 1;
		bool proj : 1;
		bool start : 1;
		octet prim : 3;
		Flags(void) : zBuff(true), clip(true), proj(false), start(false), prim(GF_POINTS) { }
	} *flags;

	const uint scrArea;
	vPort *vp;
	vect4 *cp;
	size_t *buffLen, *buffI;

	clr *cBuff;
	vect4 *hBuff;
	float *zBuff;
	mtrx4 model, view, proj;

	void SetDepth(float front, float back);
	void Raise(const char *msg);
	void ResetZBuff(void);
	static void ToScreen(vect4 *v, vect4 cp, bool proj) __GPU;

	void GF_Points(void);
	void GF_LineFan(void);
	void GF_Triangles(void);
} gfWinWnd;

#undef __GPU