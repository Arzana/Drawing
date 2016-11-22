#pragma once

#include <cstdio>
#include <Utils.h>
#include <GF.h>
#include "Camera.h"

//#define _USE_FULL_HD

#ifdef _USE_FULL_HD
#define HEIGHT		1080.0f
#define WIDTH		1920.0f
#else
#define HEIGHT		600.0f
#define WIDTH		800.0f
#endif

#define DEPTH_FAR	100.0f
#define DEPTH_NEAR	0.1f
#define FOV_Y		45.0f
#define Z_DIST(r)	((r) / sinf(FOV_Y / 2 * deg2rad))

const float aspr = WIDTH / HEIGHT;

void MouseMove(int, int, int, int);
void KeyPress(int);
void Update(void);
void Render(void);