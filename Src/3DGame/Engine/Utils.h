#pragma once

#ifndef ENABLE
#define ENABLE		1
#endif
#ifndef DISABLE
#define DISABLE	0
#endif

#ifdef __cplusplus
#include <cstdlib>
#define malloc_s(t, l)	(t*)malloc(sizeof(t) * (l))
#else
#include <stdlib.h>
#define malloc_s(t, l)	malloc(sizeof(t) * (l))
#endif

#define free_s(t)		free(t); t = NULL

#ifdef _VECT_CONV
#include "Vect2.h"
#include "Vect3.h"
#include "Vect4.h"

inline Vect3 V4ToV3(const Vect4 *v) { return Vect3(v->X, v->Y, v->Z); }
inline Vect2 V3ToV2(const Vect3 *v) { return Vect2(v->X, v->Y); }
#endif

#ifdef _VECTOR_
#include <functional>

template <typename T>
bool Contains(std::vector<T> vec, T *element);

template <typename T>
void Sort(std::vector<T> *vec, std::function<bool(const T*, const T*)> pred);
#endif