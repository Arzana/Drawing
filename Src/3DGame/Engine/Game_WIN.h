#pragma once

#include <queue>
#include "GameTime.h"
#include "GF_WIN_Window.h"
#include "GameComponent.h"

#define FPS_LOCK	60.0f

typedef struct Game_WIN
	: private gfWinWnd
{
	friend struct TriangleRenderer;
	friend struct LineFanRenderer;
	friend struct Camera;

public:
	bool *IsFixedTimeStep;

	Game_WIN(const char *title, uint width, uint height);
	~Game_WIN(void);

	virtual void OnInitialize(void);
	virtual void OnUpdate(GameTime time, const KeyboardState& kstate, const MouseState& mstate) { }
	virtual void OnRender(void) { }
	virtual void OnTerminate(void) { }

	void Run(void);
	inline void Terminate(void) { *isRunning = false; }
	float GetFps(void) const;
	float GetAvarageFPS(void) const;

	void AddComponent(GameComponent *comp);
private:
	const clock_t mili2clocks = CLOCKS_PER_SEC / 1000;
	const TimeSpan MAX_ELAPSED_TIME = 500 * mili2clocks;
	const TimeSpan TARGET_ELAPSED_TIME = TimeSpan(1.0f / FPS_LOCK * CLOCKS_PER_SEC);
	const uint buffLen = 100;

	TimeSpan inactiveSleepTime;
	TimeSpan accumelatedElapsedTime;
	GameTime gameTime;
	clock_t timer;
	clock_t previousTicks;
	int updateFrameLag;

	kstate *keyState;
	mstate *mouseState;
	std::queue<float> *frameBuffer;
	std::vector<gameComp*> *components;

	void Tick(void);
	void DoUpdate(void);
	void DoDraw(void);
	LRESULT CALLBACK WndProc(uint msg, WPARAM wParam, LPARAM lParam) override;
} winGame;