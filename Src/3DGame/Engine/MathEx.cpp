#include "MathEx.h"

float barycentric(float v1, float v2, float v3, float b2, float b3) restrict(cpu, amp)
{
	return ((1 - b2 - b3) * v1) + (b2 * v2) + (b3 * v3);
}

float catmullRom(float v1, float v2, float v3, float v4, float a) restrict(cpu, amp)
{
	float a2 = square(a);
	float a3 = cube(a);

	return (.5f * ((2.0f * v2) + (-v1 + v3) * a + (2.0f * v1 - 5.0f * v2 + 4 * v3 - v4) * a2 + (-v1 + 3.0f * v2 - 3.0f * v3 + v4) * a3));
}