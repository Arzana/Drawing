#pragma once

/* Includes */
#include <cmath>

/* Constants */
#define M_PI								3.14159265358979323846f
#define M_TAU								6.28318530717958647692f
#define M_DEG2RAD							0.01745329251994329576f

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

/* Simple functions */
#define ipart(x)							int(x)
#define sqr(x)								((x) * (x))
#define cube(x)								(sqr(x) * (x))
#define abs(x)								((x) < 0 ? -(x) : (x))
#define min2(x, y)							((x) < (y) ? (x) : (y))
#define max2(x, y)							((x) > (y) ? (x) : (y))
#define min3(x, y, z)						min2((x), min2((y), (z)))
#define max3(x, y, z)						max2((x), max2((y), (z)))
#define clamp(mi, ma, v)					min2(max2((mi), (v)), (ma))
#define fpart(x)							((x) < 0 ? 1 - ((x) - floor(x)) : (x) - floor(x))
#define rfpart(x)							float(1.0 - fpart(x))
#define csc(x)								(1.0f / sinf(x))
#define sec(x)								(1.0f / cosf(x))
#define cot(x)								(1.0f / tanf(x))
#define lerpclamp(x)						clamp(0.0f, 1.0f, abs(x))

/* Ease of use macro's */
#define GET_MACRO(_0, _1, _2, NAME, ...)	NAME
#define EXPAND(x)							x	/* Expand macro is needed because the MSVC compiler doesn't expand variadic macro's correctly.  */
#define min(...)							EXPAND(GET_MACRO(__VA_ARGS__, min3, min2)(__VA_ARGS__))
#define max(...)							EXPAND(GET_MACRO(__VA_ARGS__, max3, max2)(__VA_ARGS__))

/* Typedefs */
typedef unsigned char octet;
typedef unsigned int uint;

/* Complex or type restricted functions */
float barycentric(float v1, float v2, float v3, float b2, float b3) restrict(cpu, amp);
float catmullRom(float v1, float v2, float v3, float v4, float a) restrict(cpu, amp);
inline float lerp(float min, float max, float a) restrict(cpu, amp) { return min + (max - min) * a; }
inline float invLerp(float min, float max, float v) restrict(cpu, amp) { return (v - min) / (max - min); }