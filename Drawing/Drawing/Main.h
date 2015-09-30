#ifndef MAIN_H
#define MAIN_H

#pragma warning(disable : 4244) // Ignore possible loss of data warning.

#include <SDL.h>
#include <iostream>
#include <math.h>
#include <stdio.h>

#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd;
#endif

#include "Map.h"
#include "Triangle.h"

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

/* Initialize the SDL window and surface. */
void InitWindow();
/* Initialize and load the map. */
void InitMap();
/* Show an error message in a new window. */
void ThrowFatalError(const char* message);
/* Destroy the SDL window and surface. */
void DestroyWindow();
/* Render the current screen surface. */
void RenderScreen();
/* Render a triangle with a flat bottom. */
void FillBottomFlatTriangle(Triangle *t, int color);
/* Render a triangle with a flat top. */
void FillTopFlatTriangle(Triangle *t, int color);
/* Render a triangle that is visible to the camera (Not safe). */
void RenderTriangle(Triangle *t, int color);
/* Draw a line using Bresenham's algorithm (Not safe). */
void BresenhamLine(float x0, float y0, float x1, float y1, int color);
/* Draw a line using Xiaolin Wu's algorithm (Not safe) */
void WuLine(Line *line, int color);
/* Plot a pixel onto the screen. */
void Plot(int x, int y, int color);
/* Plot a pixel onto the screen with a given alpha. */
void PlotA(int x, int y, float alpha, int color);
void ClearScreen(int color);

#endif