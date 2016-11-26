#pragma once

#include <vector>
#include <thread>
#include <mutex>

using namespace std;

template <class T_Resc>
struct ParallelMath
{
public:
	const size_t *num;

	inline ParallelMath(void) :ParallelMath(thread::hardware_concurrency()) { }
	inline ParallelMath(const size_t amount)
	{
		num = new size_t(amount);
		working = new size_t(0);
		running = new bool(true);
		mtxBuff = new mutex();
		mtxW = new mutex();
		ths = new vector<thread>();
		buff = new vector<T_Resc>();
	}

	inline ~ParallelMath(void)
	{
		Join();

		delete num;
		delete working;
		delete running;
		delete mtxBuff;
		delete mtxW;
		delete ths;
		delete buff;
	}

	template<class _Fn>
	inline void Start(_Fn&& func)
	{
		if (ths->size()) throw logic_error("Threads are still running!");

		for (size_t i = 0; i < *num; i++)
		{
			ths->push_back(thread(func, i, running));
		}
	}

	template<class _Fn, class _Arg>
	inline void Start(_Fn&& func, _Arg&& arg)
	{
		if (ths->size()) throw logic_error("Threads are still running!");

		for (size_t i = 0; i < *num; i++)
		{
			ths->push_back(thread(func, i, running, arg));
		}
	}

	template<class _Fn, class _Arg0, class _Arg1>
	inline void Start(_Fn&& func, _Arg0&& arg0, _Arg1&& arg1)
	{
		if (ths->size()) throw logic_error("Threads are still running!");

		for (size_t i = 0; i < *num; i++)
		{
			ths->push_back(thread(func, i, running, arg0, arg1));
		}
	}

	inline void Join(void)
	{
		if (!ths->size()) return;
		*running = false;

		for (size_t i = 0; i < *num; i++)
		{
			ths->back().join();
			ths->pop_back();
		}
	}

	inline bool Get(T_Resc *resc)
	{
		mtxBuff->lock();
		if (buff->size() > 0)
		{
			*resc = buff->back();
			buff->pop_back();
			mtxBuff->unlock();
			return true;
		}

		mtxBuff->unlock();
		return false;
	}

	inline void Add(T_Resc resc) { mtxBuff->lock(); buff->push_back(resc); mtxBuff->unlock(); }
	inline void SetWorking(void) { mtxW->lock(); ++*working; mtxW->unlock(); }
	inline void SetDone(void) { mtxW->lock(); --*working; mtxW->unlock(); }
	inline void SetSleep(void) { this_thread::sleep_for(st); }
	inline void Wait(void) { while (*working || buff->size()) this_thread::sleep_for(st); }
private:
	bool *running;
	size_t *working;
	mutex *mtxBuff, *mtxW;
	const chrono::milliseconds st = chrono::milliseconds(1);

	vector<thread> *ths;
	vector<T_Resc> *buff;
};