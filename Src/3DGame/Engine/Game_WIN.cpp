#include "Game_WIN.h"

#define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))

Game_WIN::Game_WIN(const char * title, const uint width, const uint height)
	: gfWinWnd(title, width, height)
	, inactiveSleepTime(0), accumelatedElapsedTime(0), previousTicks(0), timer(0), gameTime(GameTime()), updateFrameLag(0)
	, IsFixedTimeStep(new bool(true)), oldMousePos(VECT3_ZERO), frameBuffer(new std::queue<float>())
{
	ResetZBuff();
}

Game_WIN::~Game_WIN(void)
{
	delete IsFixedTimeStep;
	delete frameBuffer;
}

void Game_WIN::Run(void)
{
	ShowWnd();
	timer = clock();

	OnInitialize();

	while (*isRunning)
	{
		Tick();
	}

	OnTerminate();
	HideWnd();
}

float Game_WIN::GetFps(void) const
{
	return frameBuffer->back();
}

float Game_WIN::GetAvarageFPS(void) const
{
	float sum = 0;
	const std::deque<float> imp = frameBuffer->_Get_container();

	size_t len = imp.size();
	for (size_t i = 0; i < len; i++)
	{
		sum += imp.at(i);
	}

	return sum / len;
}

void Game_WIN::Tick(void)
{
RetryTick:
	clock_t currentTicks = clock();
	accumelatedElapsedTime += currentTicks - previousTicks;
	previousTicks = currentTicks;

	if (*IsFixedTimeStep && accumelatedElapsedTime < TARGET_ELAPSED_TIME)
	{
		clock_t sleepTime = (float)(TARGET_ELAPSED_TIME - accumelatedElapsedTime) / mili2clocks;
		Sleep(sleepTime);

		goto RetryTick;
	}

	if (accumelatedElapsedTime > MAX_ELAPSED_TIME) accumelatedElapsedTime = MAX_ELAPSED_TIME;

	if (*IsFixedTimeStep)
	{
		gameTime.elapsedGameTime += TARGET_ELAPSED_TIME;
		int stepCount = 0;

		while (accumelatedElapsedTime >= TARGET_ELAPSED_TIME)
		{
			gameTime.totalGameTime += TARGET_ELAPSED_TIME;
			accumelatedElapsedTime -= TARGET_ELAPSED_TIME;
			++stepCount;

			DoUpdate();
		}

		updateFrameLag += max(0, stepCount - 1);

		if (gameTime.lag && updateFrameLag == 0) gameTime.lag = false;
		else if (updateFrameLag >= 5) gameTime.lag = true;

		if (stepCount == 1 && updateFrameLag > 0) updateFrameLag--;

		gameTime.elapsedGameTime = TARGET_ELAPSED_TIME * stepCount;
	}
	else
	{
		gameTime.elapsedGameTime = accumelatedElapsedTime;
		gameTime.totalGameTime += accumelatedElapsedTime;
		accumelatedElapsedTime = 0;

		DoUpdate();
	}

	DoDraw();
}

void Game_WIN::DoUpdate(void)
{
	MSG msg;
	while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
	}

	OnUpdate(gameTime);
}

void Game_WIN::DoDraw(void)
{
	float curFps = 1.0f / gameTime.GetDeltaTime();
	frameBuffer->push(curFps);
	if (frameBuffer->size() > buffLen) frameBuffer->pop();

	OnRender();
	ResetZBuff();
	RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
}

LRESULT CALLBACK Game_WIN::WndProc(uint msg, WPARAM wParam, LPARAM lParam)
{
	int x, y, dX, dY;

	switch (msg)
	{
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		break;
	case WM_MOUSEMOVE:
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);
		dX = oldMousePos.X - x;
		dY = oldMousePos.Y - y;

		if (oldMousePos.Z) OnMouseMove(x, y, dX, dY);
		oldMousePos = vect3(x, y, 1);
		break;
	default:
		return gfWinWnd::WndProc(msg, wParam, lParam);
	}

	return 0;
}