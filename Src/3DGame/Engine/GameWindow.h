#pragma once

#ifdef _USE_WINDOW_INTERNAL
#define _USE_WINDOW_BASE_INTERNAL
#endif

#include <ctime>
#include <queue>
#include "WindowBase.h"
#include "Shapes.h"
#include "Color.h"

#ifndef EXIT_FAILURE
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1
#endif

#define FPS_LOCK	60.0f

#ifdef _USE_WINDOW_INTERNAL
bool *isRunning;
LRESULT CALLBACK WndProc(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam);
#endif

class GameWindow
{
public:
	void(*Update)(void);
	void(*Draw)(void);
	void(*KeyDown)(int);
	void(*MouseMove)(int, int, int, int);
	void(*OnTerminate)(void);

	bool *IsFixedTimeStep;
	bool *Lag;

	GameWindow(const char *title, const uint width, const uint height);
	~GameWindow();

	void Plot(const Vertex *vrtx);
	void Plot(const Vect3 *v, const Color c);
	void Plot(const float x, const float y, const float z, const Color c);

	void TryPlot(const Vertex *vrtx);
	void TryPlot(const Vect3 *v, const Color c);
	void TryPlot(const float  x, const float y, const float z, const Color c);

	void Clear(const Color c);
	void Run(void);
	void Terminate(void);
	void Show(void) const;
	void Hide(void) const;
	uint GetWidth(void) const;
	uint GetHeight(void) const;
	float GetFps(void) const;
	float GetAvarageFPS(void) const;
private:
	const clock_t mili2clocks = CLOCKS_PER_SEC / 1000;

	WindowBase *window;
	float *zBuffer;

	const clock_t MAX_ELAPSED_TIME = 500 * mili2clocks;
	const clock_t TARGET_ELAPSED_TIME = 1.0f / FPS_LOCK * CLOCKS_PER_SEC;

	clock_t inactiveSleepTime;
	clock_t accumelatedElapsedTime;
	clock_t previousTicks;

	clock_t timer;
	clock_t elapsedGameTime;
	clock_t totalGameTime;
	int updateFrameLag;

	const uint buffLen = 100;
	std::queue<float> *frameBuffer;

	void InitWindow(void);
	void TerminateWindow(void);
	template <typename T>
	inline bool PointVisible(T x, T y) const;
	void Tick(void);
	void DoUpdate(void);
	void DoDraw(void);
	void HandleEvents(void);
	void ResetZBuffer(void);
};