#ifndef MAIN_H
#define MAIN_H

#pragma warning(disable : 4244) // Ignore possible loss of data warning.

#include <SDL.h>
#include <iostream>
#include <math.h>
#include "Line.h"
#include "Map.h"
#include "UtilDefs.h"

/* The aspect ratio of the screen. */
#define ASPR				(WINDOW_WIDTH / WINDOW_HEIGHT)
/* Get the interger part of x. */
#define ipart(x)			(int(x))
/* Round a float to a int value. */
#define round(x)			(ipart((x) + 0.5))
/* Get the fractional part of x. */
#define fpart(x)			(float((x) - int(x)))
#define rfpart(x)			(float(1.0 - fpart(x)))
/* Convert a RBGA value (with 0 ≤ a ≤ 1) to a packed integer color.*/
#define pack(r, g, b, a)	((int(r * a) << 16) + (int(g * a) << 8) + int(b * a))

void InitWindow();
void InitMap();
void ThrowFatalError(const char* message);
void DestroyWindow();
void RenderScreen();
void WuLine(Line& line);
void Plot(int x, int y, int color);
void PlotA(int x, int y, float alpha);
void ClearScreen(int color);

#endif