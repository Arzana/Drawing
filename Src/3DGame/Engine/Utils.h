#pragma once

#ifdef __cplusplus
#include <cstdlib>
#define malloc_s(t, l)	((t*)malloc(sizeof(t) * (l)))
#else
#include <stdlib.h>
#define malloc_s(t, l)	(malloc(sizeof(t) * (l)))
#endif