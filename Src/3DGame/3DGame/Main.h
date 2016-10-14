#pragma once

#include <Utils.h>
#include <GF.h>
#include <Mtrx4.h>
#include <cstdio>

#define HEIGHT		600.0f
#define WIDTH		800.0f
#define DEPTH_FAR	100.0f
#define DEPTH_NEAR	1.0f
#define FOV_Y		60.0f

const float aspr = WIDTH / HEIGHT;

void Render(void);
Vect3 ToScreen(const Vect3 *ndc);