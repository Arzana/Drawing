#pragma once

#include <cmath>

#define M_PI				3.14159265358979323846
#define M_TAU				6.28318530717958647692
#define M_DEG2RAD			0.01745329251994329576

#undef min
#undef max

#define ipart(x)			int(x)
#define square(x)			((x) * (x))
#define cube(x)				(square(x) * (x))
#define abs(x)				((x) < 0 ? -(x) : (x))
#define min(x, y)			((x) < (y) ? (x) : (y))
#define max(x, y)			((x) > (y) ? (x) : (y))
#define min3(x, y, z)		(min((x), min((y), (z))))
#define max3(x, y, z)		(max((x), max((y), (z))))
#define clamp(mi, ma, v)	(min(max((mi), (v)), (ma)))
#define fpart(x)			((x) < 0 ? 1 - ((x) - floor(x)) : (x) - floor(x))
#define rfpart(x)			(float(1.0 - fpart(x))
#define csc(x)				(1.0f / sinf(x))
#define sec(x)				(1.0f / cosf(x))
#define cot(x)				(1.0f / tanf(x))
#define lerpclamp(x)		clamp(0.0f, 1.0f, abs(x))

typedef unsigned char octet;
typedef unsigned int uint;

float barycentric(float v1, float v2, float v3, float b2, float b3) restrict(cpu, amp);
float catmullRom(float v1, float v2, float v3, float v4, float a) restrict(cpu, amp);
inline float lerp(float min, float max, float a) restrict(cpu, amp) { return min + (max - min) * a; }
inline float invLerp(float min, float max, float v) restrict(cpu, amp) { return (v - min) / (max - min); }