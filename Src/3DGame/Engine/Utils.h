#pragma once

#ifndef ENABLE
#define ENABLE					1
#endif
#ifndef DISABLE
#define DISABLE					0
#endif

#ifdef __cplusplus
#include <cstdlib>
#define malloc_s(t, l)			(t*)malloc(sizeof(t) * (l))
#define delete_s(t)				delete t; t = NULL				
#else
#include <stdlib.h>
#define malloc_s(t, l)			malloc(sizeof(t) * (l))
#endif

#define free_s(t)				free(t); t = NULL

#define xy2i(x, y, w)			((y) * (w) + (x))
#define xyz2i(x, y, z, w, h)	((z) * (w) * (h) + xy2i(x, y, w))

#ifdef _VECT_CONV
#include "Vect2.h"
#include "Vect3.h"
#include "Vect4.h"

inline vect3 V4ToV3(const vect4 v) restrict(cpu, amp) { return vect3(v.X, v.Y, v.Z); }
inline vect2 V3ToV2(const vect3 v) restrict(cpu, amp) { return vect2(v.X, v.Y); }
#endif

#ifdef _VECTOR_
template <typename T>
bool Contains(std::vector<T> vec, T *element)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec.at(i) == *element) return true;
	}

	return false;
}
#endif