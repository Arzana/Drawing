#pragma once

#include <cmath>

#define M_PI				3.14159265358979323846
#define M_TAU				6.28318530717958647692

#define ipart(x)			(int(x))
#define square(x)			((x) * (x))
#define cube(x)				(square(x) * (x))
#define abs(x)				((x) < 0 ? (x) * -1 : (x))
#define min(x, y)			((x) < (y) ? (x) : (y))
#define max(x, y)			((x) > (y) ? (x) : (y))
#define clamp(mi, ma, v)	(min(max((mi), (v)), (ma)))
#define floor(x)			((x) < 0 ? ipart((x) - 0.5) : ipart(x))
#define ciel(x)				(ipart((x) + 0.5f))
#define round(x)			ciel(x)
#define fpart(x)			((x) < 0 ? 1 - ((x) - floor(x)) : (x) - floor(x))
#define rfpart(x)			(float(1.0 - fpart(x))
#define csc(x)				(1.0f / sinf(x))
#define sec(x)				(1.0f / cosf(x))
#define cot(x)				(1.0f / tanf(x))

typedef unsigned char octet;
typedef unsigned int uint;

const float deg2rad = float(M_PI / 180.0f);

float barycentric(float v1, float v2, float v3, float b2, float b3);
float catmullRom(float v1, float v2, float v3, float v4, float a);
inline float lerp(float min, float max, float a) { return min + (max - min) * a; }
inline float invLerp(float min, float max, float v) { return (v - min) / (max - min); }