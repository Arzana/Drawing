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