#define _USE_GF_THREADS_INTERNAL
#include "GF_Threads.h"

#define get_pop(v)			v.back(); v.pop_back()
const chrono::microseconds st = chrono::microseconds(1);

bool *thrdsRun;
thread ths[N_THREADS];
mutex mtx;

vector<hline> hlBuff;
vector<vline> vlBuff;

inline void AddHLine(hline line) { mtx.lock(); hlBuff.push_back(line); mtx.unlock(); }
inline void AddVLine(vline line) { mtx.lock(); vlBuff.push_back(line); mtx.unlock(); }

void SetHLineThreads(size_t start, size_t length)
{
	size_t end = start + length;
	for (size_t i = start; i < end; i++)
	{
		ths[i] = thread(hline_func, i);
	}
}

void SetVLineThreads(size_t start, size_t length)
{
	size_t end = start + length;
	for (size_t i = start; i < end; i++)
	{
		ths[i] = thread(vline_func, i);
	}
}

void WaitThreads(void)
{
	while (hlBuff.size() || vlBuff.size()) this_thread::sleep_for(st);
}

void JoinThreads(void)
{
	for (size_t i = 0; i < N_THREADS; i++)
	{
		ths[i].join();
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
}