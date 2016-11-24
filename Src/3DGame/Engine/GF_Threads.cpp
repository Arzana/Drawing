#define _USE_GF_THREADS_INTERNAL
#include "GF_Threads.h"
#include "Utils.h"

#define get_pop(v)			v.back(); v.pop_back()
const chrono::microseconds st = chrono::microseconds(1);

struct indices { int i, j; };

bool *thrdsRun;
int *pRun;
mutex mtx;

vector<thread> ths;
vector<hline> hlBuff;
vector<vline> vlBuff;
vector<indices> lBuff;
vector<indices> pBuff;

inline void AddHLine(hline line) { mtx.lock(); hlBuff.push_back(line); mtx.unlock(); }
inline void AddVLine(vline line) { mtx.lock(); vlBuff.push_back(line); mtx.unlock(); }
inline void AddLine(int i, int j) { mtx.lock(); lBuff.push_back({ i, j }); mtx.unlock(); }
inline void AddPoint(int start, int end) { mtx.lock(); pBuff.push_back({ start, end }); mtx.unlock(); }

void SetHLineThreads(size_t start, size_t length)
{
	if (!pRun) pRun = new int(0);

	size_t end = start + length;
	for (size_t i = start; i < end; i++)
	{
		ths.push_back(thread(hline_func, i));
	}
}

void SetVLineThreads(size_t start, size_t length)
{
	if (!pRun) pRun = new int(0);

	size_t end = start + length;
	for (size_t i = start; i < end; i++)
	{
		ths.push_back(thread(vline_func, i));
	}
}

void SetLineThreads(size_t start, size_t length)
{
	if (!pRun) pRun = new int(0);

	size_t end = start + length;
	for (size_t i = 0; i < end; i++)
	{
		ths.push_back(thread(line_func, i));
	}
}

void SetPointThreads(size_t start, size_t length)
{
	if (!pRun) pRun = new int(0);

	size_t end = start + length;
	for (size_t i = 0; i < end; i++)
	{
		ths.push_back(thread(point_func, i));
	}
}

void WaitThreads(void)
{
	while (hlBuff.size() || vlBuff.size() || lBuff.size() || pBuff.size() || *pRun) this_thread::sleep_for(st);
}

void JoinThreads(void)
{
	for (size_t i = 0; i < n_threads; i++)
	{
		ths.back().join();
		ths.pop_back();
	}

	if (pRun)
	{
		delete pRun;
		pRun = NULL;
	}
}

void hline_func(const size_t thrdId)
{
	printf("starting hline(%d)\n", thrdId);

	while (*thrdsRun)
	{
		mtx.lock();
		if (hlBuff.size() > 0)
		{
			hline l = get_pop(hlBuff);
			mtx.unlock();

			GF_HLine(l.x0, l.z0, l.c0, l.x1, l.z1, l.c1, l.y);
		}
		else
		{
			mtx.unlock();
			this_thread::sleep_for(st);
		}
	}

	printf("stopping hline(%d)\n", thrdId);
}

void vline_func(const size_t thrdId)
{
	printf("starting vline(%d)\n", thrdId);

	while (*thrdsRun)
	{
		mtx.lock();
		if (vlBuff.size() > 0)
		{
			vline l = get_pop(vlBuff);
			mtx.unlock();

			GF_VLine(l.y0, l.z0, l.c0, l.y1, l.z1, l.c1, l.x);
		}
		else
		{
			mtx.unlock();
			this_thread::sleep_for(st);
		}
	}

	printf("stopping vline(%d)\n", thrdId);
}

void line_func(const size_t thrdId)
{
	printf("starting line(%d)\n", thrdId);

	while (*thrdsRun)
	{
		mtx.lock();
		if (lBuff.size() > 0)
		{
			indices l = get_pop(lBuff);
			mtx.unlock();

			single_line(l.i, l.j);
		}
		else
		{
			mtx.unlock();
			this_thread::sleep_for(st);
		}
	}

	printf("stopping line(%d)\n", thrdId);
}

void point_func(const size_t thrdId)
{
	printf("starting point(%d)\n", thrdId);

	while (*thrdsRun)
	{
		mtx.lock();
		if (pBuff.size() > 0)
		{
			indices p = get_pop(pBuff);
			*pRun += 1;
			mtx.unlock();

			mult_point(p.i, p.j);

			mtx.lock();
			*pRun -= 1;
			mtx.unlock();
		}
		else
		{
			mtx.unlock();
			this_thread::sleep_for(st);
		}
	}

	printf("stopping point(%d)\n", thrdId);
}