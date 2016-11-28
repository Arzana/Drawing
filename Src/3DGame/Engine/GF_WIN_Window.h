#pragma once

#include <amp.h>
#include <vector>
#include "WindowsWindow.h"
#include "Shapes.h"
#include "Mtrx4.h"

#define GF_POINTS			0
#define GF_LINES			1
#define GF_LINE_STRIP		2
#define GF_LINE_LOOP		3
#define GF_LINE_FAN			4
#define GF_TRIANGLES		5
#define GF_TRIANGLE_STRIP	6
#define GF_TRIANGLE_FAN		7
#define __GPU				restrict(cpu, amp)

typedef struct GF_WIN_Window
	: public winWnd
{
	friend struct Game_WIN;

public:
	GF_WIN_Window(const char *title, const uint width, const uint height);
	~GF_WIN_Window(void);

	void SetProjection_Frustrum(const float fovY, const float aspr, const float front, const float back);
	void SetProjection_Orthographic(const float width, const float height, const float front, const float back);
	inline void SetView(const mtrx4 *mtrx) { view = *mtrx; }
	inline void SetModel(const mtrx4 *mtrx) { model = *mtrx; }
	void SetBufferLength(const size_t length);
	void Start(const octet primitiveType);
	void End(void);
	void AddVertex(const vect3 v, const clr c);
	void Clear(const clr c);
protected:
	inline void SetFlag_ZBuffering(const bool value) { flags->zBuff = value; }
	inline void SetFlag_Clipping(const bool value) { flags->clip = value; }
	void SetFlag_VertexBuffering(const bool value);
	inline vect3* GetVertexBuffer(void) { return vBuff; }
private:
	struct Flags
	{
		bool zBuff : 1;
		bool clip : 1;
		bool vBuff : 1;
		bool proj : 1;
		bool start : 1;
		octet prim : 3;
		Flags(void) : zBuff(true), clip(true), vBuff(false), proj(false), start(false), prim(GF_POINTS) { }
	} *flags;

	const float scrArea;
	vPort *vp;
	vect4 *cp;
	size_t *buffLen, *buffI;

	vect3 *vBuff;
	clr *cBuff;
	vect4 *hBuff;
	float *zBuff;
	mtrx4 model, view, proj;

	void SetDepth(float front, float back);
	void Raise(const char *msg);
	void ResetZBuff(void);
	static vect3 ToScreen(const vect4 v, const vect4 cp, const bool proj) __GPU;

	void GF_Points(void);
} gfWinWnd;

#undef __GPU