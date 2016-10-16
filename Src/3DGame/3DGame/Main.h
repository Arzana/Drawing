#pragma once

#include <GF.h>
#include <Mtrx4.h>
#include <cstdio>

#define _USE_FULL_HD

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

const float aspr = WIDTH / HEIGHT;

void KeyPress(int);
void Render(void);