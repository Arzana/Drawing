#pragma once

#include <cmath>

#define M_PI				3.14159265358979323846
#define M_TAU				6.28318530717958647692

#define min(x, y)			((x) < (y) ? (x) : (y))
#define max(x, y)			((x) > (y) ? (x) : (y))
#define abs(x)				((x) < 0 ? (x) * -1 : (x))
#define round(x)			(ipart((x) + 0.5f))

#define clamp(mi, ma, v)	(min(max((mi), (v)), (ma)))
#define square(x)			((x) * (x))
#define cube(x)				(square(x) * (x))
#define ipart(x)			(int(x))
#define fpart(x)			(float(x) - ipart(x))
#define rfpart(x)			(float(1.0 - fpart(x))
#define cot(x)				(1.0f - tanf(x))

typedef unsigned char octet;
typedef unsigned int uint;

const float deg2rad = float(M_PI / 180.0f);

float barycentric(float v1, float v2, float v3, float b2, float b3);
float catmullRom(float v1, float v2, float v3, float v4, float a);
float lerp(float min, float max, float a);
float invLerp(float min, float max, float v);