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
void GF_AddPoint(const float x, const float y, const Color c);