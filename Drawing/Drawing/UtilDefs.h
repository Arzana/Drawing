#ifndef UTILDEFS_H
#define UTILDEFS_H

/* Calculate the cotangent of x.*/
#define cot(d)					(1 / tanf(d))
/* Clamp x between a min and max. */
#define clamp(mi, ma, x)		((x) < (mi) ? (mi) : ((x) > (ma) ? (ma) : (x)))
/* Get x sqaured. */
#define square(x)				((x) * (x))

const float DEG2RAD = 3.141593f / 180;

#endif