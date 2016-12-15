#pragma once

#include <queue>
#include "GameTime.h"
#include "GF_WIN_Window.h"

#define FPS_LOCK	60.0f

typedef struct Game_WIN
	: public gfWinWnd
{
public:
	bool *IsFixedTimeStep;

	Game_WIN(const char *title, const uint width, const uint height);
	~Game_WIN(void);

	virtual void OnInitialize(void);
	virtual void OnUpdate(GameTime time) { }
	virtual void OnRender(void) { }
	virtual void OnKeyDown(int sym) { }
	virtual void OnMouseMove(int x, int y, int dX, int dY) { }
	virtual void OnTerminate(void) { }

	void Run(void);
	inline void Terminate(void) { *isRunning = false; }
	float GetFps(void) const;
	float GetAvarageFPS(void) const;
private:
	const clock_t mili2clocks = CLOCKS_PER_SEC / 1000;
	const TimeSpan MAX_ELAPSED_TIME = 500 * mili2clocks;
	const TimeSpan TARGET_ELAPSED_TIME = 1.0f / FPS_LOCK * CLOCKS_PER_SEC;
	const uint buffLen = 100;

	TimeSpan inactiveSleepTime;
	TimeSpan accumelatedElapsedTime;
	GameTime gameTime;
	clock_t timer;
	clock_t previousTicks;
	int updateFrameLag;

	vect3 oldMousePos;
	std::queue<float> *frameBuffer;

	void Tick(void);
	void DoUpdate(void);
	void DoDraw(void);
	LRESULT CALLBACK WndProc(uint msg, WPARAM wParam, LPARAM lParam) override;
} winGame;