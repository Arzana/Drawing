#pragma once

#include <math.h>

#define abs(x)				((x) < 0 ? (x) * -1 : (x))
#define max(x, y)			((x) < (y) ? (y) : (x))
#define min(x, y)			((x) < (y) ? (x) : (y))
#define clamp(mi, ma, v)	(min(max((mi), (v)), (ma)))
#define square(x)			((x) * (x))
#define cube(x)				(square(x) * (x))
#define ipart(x)			(int(x))
#define round(x)			(ipart((x) + .5f))
#define fpart(x)			(float(x) - ipart(x))
#define rfpart(x)			(float(1.0 - fpart(x))
#define cot(x)				(1.0f - tanf(x))

typedef unsigned char octet;
typedef unsigned int uint;

inline float barycentric(float v1, float v2, float v3, float b2, float b3);
inline float catmullRom(float v1, float v2, float v3, float v4, float a);
inline float lerp(float min, float max, float a);
inline float invLerp(float min, float max, float v);