#pragma once

#define N_THREADS			8
#define _USE_GF_INTERNAL

#include <thread>
#include <mutex>
#include "GF.h"

using namespace std;

typedef struct HorizontalLine
{
	float x0, z0;
	Color c0;
	float x1, z1;
	Color c1;
	float y;

	HorizontalLine(float x0, float z0, Color c0, float x1, float z1, Color c1, float y)
		: x0(x0), z0(z0), c0(c0), x1(x1), z1(z1), c1(c1), y(y)
	{ }
} hline;

typedef struct VerticalLine
{
	float y0, z0;
	Color c0;
	float y1, z1;
	Color c1;
	float x;

	VerticalLine(float y0, float z0, Color c0, float y1, float z1, Color c1, float x)
		: y0(y0), z0(z0), c0(c0), y1(y1), z1(z1), c1(c1), x(x)
	{ }
} vline;

extern bool *thrdsRun;

extern inline void AddHLine(hline line);
extern inline void AddVLine(vline line);
extern inline void AddLine(int i, int j);
extern inline void AddPoint(int start, int end);

void SetHLineThreads(size_t start, size_t length);
void SetVLineThreads(size_t start, size_t length);
void SetLineThreads(size_t start, size_t length);
void SetPointThreads(size_t start, size_t length);
void WaitThreads(void);
void JoinThreads(void);

#ifdef _USE_GF_THREADS_INTERNAL
void hline_func(const size_t thrdId);
void vline_func(const size_t thrdId);
void line_func(const size_t thrdId);
void point_func(const size_t thrdId);
#endif