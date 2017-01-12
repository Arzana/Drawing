#pragma once

#include "Triangle.h"

#ifndef NULL
	#ifdef __cplusplus
		#define NULL 0
	#else
		#define NULL ((void *)0)
	#endif
#endif

typedef struct Geon
{
public:
	const int trglCount;

	Geon(void) : trglCount(0), trgls(NULL) { }
	Geon(int len, ...);
	~Geon(void);

	inline Geon operator +(const Geon &r) { return Geon(this, &r); }

	inline const trgl* GetTriangles(void) const { return trgls; }
protected:
	trgl *trgls;
private:
	Geon(const Geon *g0, const Geon *g1);
} geon;