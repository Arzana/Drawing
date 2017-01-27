#include "Game_WIN.h"
#include "WinLogger.h"

#define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))
#define BOOL_STR(b)			((b) ? "true" : "false")

Game_WIN::Game_WIN(const char * title, uint width, uint height)
	: gfWinWnd(title, width, height)
	, inactiveSleepTime(0), accumelatedElapsedTime(0), previousTicks(0), timer(0), gameTime(GameTime()), updateFrameLag(0)
	, IsFixedTimeStep(new bool(true))
	, keyState(new kstate()), mouseState(new mstate())
	, frameBuffer(new std::queue<float>())
	, components(new std::vector<gameComp*>())
{
	ResetZBuff();
}

Game_WIN::~Game_WIN(void) 
{
	delete IsFixedTimeStep;
	delete frameBuffer;
	delete components;
}

void Game_WIN::OnInitialize(void)
{
	LogMsg_Game("Initializing windows game.");
	LogMsg_Game("\tFlags { ZBuffering: %s, Clipping: %s, VertexBuffering: %s }",
		BOOL_STR(flags->zBuff), BOOL_STR(flags->clip));

	for (size_t i = 0; i < components->size(); i++)
	{
		(*components)[i]->Initialize();
	}
}

void Game_WIN::Run(void)
{
	timer = clock();

	ShowWnd();
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

void Game_WIN::AddComponent(GameComponent * comp)
{
	components->push_back(comp);
}

void Game_WIN::Tick(void)
{
RetryTick:
	clock_t currentTicks = clock();
	accumelatedElapsedTime += currentTicks - previousTicks;
	previousTicks = currentTicks;

	if (*IsFixedTimeStep && accumelatedElapsedTime < TARGET_ELAPSED_TIME)
	{
		clock_t sleepTime = clock_t((TARGET_ELAPSED_TIME - accumelatedElapsedTime) / (float)mili2clocks);
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

	OnUpdate(gameTime, *keyState, *mouseState);

	for (size_t i = 0; i < components->size(); i++)
	{
		(*components)[i]->Update(gameTime, *keyState, *mouseState);
	}

	mouseState->DeltaReset();
}

void Game_WIN::DoDraw(void)
{
	float curFps = 1.0f / gameTime.GetDeltaTime();
	frameBuffer->push(curFps);
	if (frameBuffer->size() > buffLen) frameBuffer->pop();

	Clear(CLR_BLACK);
	OnRender();

	for (size_t i = 0; i < components->size(); i++)
	{
		(*components)[i]->Draw();
	}

	ResetZBuff();
	RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
}

LRESULT CALLBACK Game_WIN::WndProc(uint msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		*keyState << (Keys)wParam;
		break;
	case WM_KEYUP:
		*keyState >> (Keys)wParam;
		break;
	case WM_MOUSEMOVE:
		mouseState->Update(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_NCMOUSEMOVE:
		mouseState->inBounds = false;
		break;
	case WM_LBUTTONDOWN:
		mouseState->leftDown = true;
		break;
	case WM_LBUTTONUP:
		mouseState->leftDown = false;
		break;
	case WM_MBUTTONDOWN:
		mouseState->middleDown = true;
		break;
	case WM_MBUTTONUP:
		mouseState->middleDown = false;
		break;
	case WM_RBUTTONDOWN:
		mouseState->rightDown = true;
		break;
	case WM_RBUTTONUP:
		mouseState->rightDown = false;
		break;
	case WM_MOUSEWHEEL:
		mouseState->scrollWheel += GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		break;
	default:
		return gfWinWnd::WndProc(msg, wParam, lParam);
	}

	return 0;
}