#include "MathEx.h"

float barycentric(float v1, float v2, float v3, float b2, float b3)
{
	return ((1 - b2 - b3) * v1) + (b2 * v2) + (b3 * v3);
}

float catmullRom(float v1, float v2, float v3, float v4, float a)
{
	float a2 = a * a;
	float a3 = a2 * a;

	return (.5f * ((2.0f * v2) + (-v1 + v3) * a + (2.0f * v1 - 5.0f * v2 + 4 * v3 - v4) * a2 + (-v1 + 3.0f * v2 - 3.0f * v3 + v4) * a3));
}

float lerp(float mi, float ma, float a)
{
	return mi + (ma - mi) * a;
}

float invLerp(float mi, float ma, float v)
{
	return (v - mi) / (ma - mi);
}
