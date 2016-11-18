#define _USE_WINDOW_INTERNAL

#include <cfloat>
#include "Utils.h"
#include "GameWindow.h"

#define pix		((Color*)window->pixels)
void(**KD)(int vk);

GameWindow::GameWindow(const char * title, const uint width, const uint height)
	: inactiveSleepTime(0), accumelatedElapsedTime(0), previousTicks(0), timer(0), totalGameTime(0), elapsedGameTime(0), updateFrameLag(0)
	, Update(NULL), Draw(NULL), KeyDown(NULL), MouseMove(NULL), OnTerminate(NULL)
{
	KD = &KeyDown;

	if (isRunning) return;
	else isRunning = new bool(false);

	window = new WindowBase(title, width, height, WndProc);
	IsFixedTimeStep = new bool(true);
	Lag = new bool(false);

	frameBuffer = new std::queue<float>();
	zBuffer = malloc_s(float, width * height);
	ResetZBuffer();
}

GameWindow::~GameWindow(void)
{
	if (window) delete window;
	if (IsFixedTimeStep) delete IsFixedTimeStep;
	if (Lag) delete Lag;
	if (frameBuffer) delete frameBuffer;
	if (zBuffer) free_s(zBuffer);
	if (isRunning) delete isRunning;

	DeleteObject(hbmp);
	DestroyWindow(window->GetHWND());
}

void GameWindow::Plot(const Vertex *vrtx)
{
	pix[ipart(vrtx->v.Y) * window->width + ipart(vrtx->v.X)] = vrtx->c;
}

void GameWindow::Plot(const Vect3 * v, const Color c)
{
	pix[ipart(v->Y) * window->width + ipart(v->X)] = c;
}

void GameWindow::Plot(const float x, const float y, const float z, const Color c)
{
	pix[ipart(y) * window->width + ipart(x)] = c;
}

void GameWindow::TryPlot(const Vertex * vtx)
{
	uint i = ipart(vtx->v.Y) * window->width + ipart(vtx->v.X);
	if (vtx->v.Z > zBuffer[i]) return;

	zBuffer[i] = vtx->v.Z;
	pix[i] = vtx->c;
}

void GameWindow::TryPlot(const Vect3 * v, const Color c)
{
	uint i = ipart(v->Y) * window->width + ipart(v->X);
	if (v->Z > zBuffer[i]) return;

	zBuffer[i] = v->Z;
	pix[i] = c;
}

void GameWindow::TryPlot(const float x, const float y, const float z, const Color c)
{
	uint i = ipart(y) * window->width + ipart(x);
	if (z > zBuffer[i]) return;

	zBuffer[i] = z;
	pix[i] = c;
}

void GameWindow::Clear(const Color c)
{
	for (size_t i = 0; i < window->height * window->width; i++)
	{
		pix[i] = c;
	}
}

void GameWindow::Run(void)
{
	*isRunning = true;
	timer = clock();

	while (*isRunning)
	{
		Tick();
	}

	if (OnTerminate) OnTerminate();
}

void GameWindow::Terminate(void)
{
	if (isRunning) *isRunning = false;
}

void GameWindow::Show(void) const
{
	ShowWindow(window->GetHWND(), SW_SHOWDEFAULT);
}

void GameWindow::Hide(void) const
{
	CloseWindow(window->GetHWND());
}

uint GameWindow::GetWidth(void) const
{
	return window->width;
}

uint GameWindow::GetHeight(void) const
{
	return window->height;
}

float GameWindow::GetFps(void) const
{
	return frameBuffer->back();
}

float GameWindow::GetAvarageFPS(void) const
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

template<typename T>
inline bool GameWindow::PointVisible(T x, T y) const
{
	return x > 0 && x < window->width && y > 0 && y < window->height;
}

void GameWindow::Tick(void)
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
		elapsedGameTime += TARGET_ELAPSED_TIME;
		int stepCount = 0;

		while (accumelatedElapsedTime >= TARGET_ELAPSED_TIME)
		{
			totalGameTime += TARGET_ELAPSED_TIME;
			accumelatedElapsedTime -= TARGET_ELAPSED_TIME;
			++stepCount;

			DoUpdate();
		}

		updateFrameLag += max(0, stepCount - 1);

		if (*Lag && updateFrameLag == 0) *Lag = false;
		else if (updateFrameLag >= 5) *Lag = true;

		if (stepCount == 1 && updateFrameLag > 0) updateFrameLag--;

		elapsedGameTime = TARGET_ELAPSED_TIME * stepCount;
	}
	else
	{
		elapsedGameTime = accumelatedElapsedTime;
		totalGameTime += accumelatedElapsedTime;
		accumelatedElapsedTime = 0;

		DoUpdate();
	}

	DoDraw();
}

void GameWindow::DoUpdate(void)
{
	HandleEvents();
	if (Update) Update();
}

void GameWindow::DoDraw(void)
{
	float curFps = 1.0f / (elapsedGameTime / (float)CLOCKS_PER_SEC);
	frameBuffer->push(curFps);
	if (frameBuffer->size() > buffLen) frameBuffer->pop();

	if (Draw) Draw();
	if (zBuffer) ResetZBuffer();

	RedrawWindow(window->GetHWND(), NULL, NULL, RDW_INVALIDATE);
}

void GameWindow::HandleEvents(void)
{
	MSG msg;
	while (PeekMessage(&msg, window->GetHWND(), 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, uint msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		*isRunning = false;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (KD) (*KD)(wParam);
		break;
	case WM_PAINT:
		PAINTSTRUCT		ps;
		HDC				hdc;
		BITMAP			bmp;
		HDC				hdcMem;
		HGDIOBJ			oldBmp;

		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		oldBmp = SelectObject(hdcMem, hbmp);

		GetObject(hbmp, sizeof(BITMAP), &bmp);
		BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBmp);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void GameWindow::ResetZBuffer(void)
{
	uint length = window->width * window->height;
	for (size_t i = 0; i < length; i++)
	{
		zBuffer[i] = FLT_MAX;
	}
}