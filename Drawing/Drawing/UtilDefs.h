#ifndef UTILDEFS_H
#define UTILDEFS_H

#define PLANE_NEAR		0.1f
#define PLANE_FAR		100.0f
#define WINDOW_HEIGHT	600.0f
#define WINDOW_WIDTH	800.0f

/* Calculate the cotangent of x.*/
#define cot(d)					(1 / tanf(d))
/* Clamp x between a min and max. */
#define clamp(mi, ma, x)		((x) < (mi) ? (mi) : ((x) > (ma) ? (ma) : (x)))
/* Get x sqaured. */
#define square(x)				((x) * (x))

const float DEG2RAD = 3.141593f / 180;

#endif