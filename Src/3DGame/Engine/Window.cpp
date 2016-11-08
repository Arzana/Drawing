#include "Utils.h"
#include "Window.h"
#include <SDL.h>
#include <cfloat>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define pix			((Color*)surface->pixels)

GameWindow::GameWindow(const char * title, const uint width, const uint height)
{
	this->title = title;
	this->width = width;
	this->height = height;

	IsFixedTimeStep = new bool(true);
	Lag = new bool(false);

	inactiveSleepTime = 0;
	accumelatedElapsedTime = 0;
	previousTicks = 0;
	timer = 0;
	totalGameTime = 0;
	elapsedGameTime = 0;
	updateFrameLag = 0;

	frameBuffer = new std::queue<float>();

	isRunning = NULL;
	Update = NULL;
	Draw = NULL;
	KeyDown = NULL;
	MouseMove = NULL;

	InitWindow();
	zBuffer = malloc_s(float, width * height);
	ResetZBuffer();
}

GameWindow::~GameWindow()
{
	TerminateWindow();
	if (zBuffer) free_s(zBuffer);
	if (isRunning) delete isRunning;
	if (IsFixedTimeStep) delete IsFixedTimeStep;
	if (Lag) delete Lag;
	if (frameBuffer) delete frameBuffer;
}

void GameWindow::Plot(const Vertex * vtx)
{
	pix[ipart(vtx->v.Y) * width + ipart(vtx->v.X)] = vtx->c;
}

void GameWindow::Plot(const Vect3 * v, const Color c)
{
	pix[ipart(v->Y) * width + ipart(v->X)] = c;
}

void GameWindow::Plot(const float x, const float y, const float z, const Color c)
{
	pix[ipart(y) * width + ipart(x)] = c;
}

void GameWindow::TryPlot(const Vertex * vtx)
{
	uint i = ipart(vtx->v.Y) * width + ipart(vtx->v.X);
	if (vtx->v.Z > zBuffer[i]) return;

	zBuffer[i] = vtx->v.Z;
	pix[i] = vtx->c;
}

void GameWindow::TryPlot(const Vect3 * v, const Color c)
{
	uint i = ipart(v->Y) * width + ipart(v->X);
	if (v->Z > zBuffer[i]) return;

	zBuffer[i] = v->Z;
	pix[i] = c;
}

void GameWindow::TryPlot(const float x, const float y, const float z, const Color c)
{
	uint i = ipart(y) * width + ipart(x);
	if (z > zBuffer[i]) return;

	zBuffer[i] = z;
	pix[i] = c;
}

void GameWindow::Clear(const Color c)
{
	for (size_t i = 0; i < height * width; i++)
	{
		pix[i] = c;
	}
}

void GameWindow::Run(void)
{
	if (!isRunning) isRunning = new int(1);
	else *isRunning = 1;

	timer = clock();
	while (*isRunning)
	{
		Tick();
	}
}

void GameWindow::Terminate()
{
	*isRunning = 0;
}

uint GameWindow::GetWidth(void) const
{
	return width;
}

uint GameWindow::GetHeight(void) const
{
	return height;
}

float GameWindow::GetFps(void) const
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

void GameWindow::InitWindow(void)
{
	if (SDL_Init(SDL_INIT_VIDEO)) exit(EXIT_FAILURE);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);

#ifdef _MOUSE_LOCK
	SDL_ShowCursor(SDL_DISABLE);
#endif
}

void GameWindow::TerminateWindow(void)
{
	if (window)
	{
		SDL_DestroyWindow(window);
		window = NULL;
		surface = NULL;
		SDL_Quit();
	}
}

template <typename T>
inline bool GameWindow::PointVisible(T x, T y) const
{
	return x > 0 && x < width && y > 0 && y < height;
}

void GameWindow::Tick()
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
	float currentFps = 1.0f / (elapsedGameTime / (float)CLOCKS_PER_SEC);
	frameBuffer->push(currentFps);
	if (frameBuffer->size() > buffLen) frameBuffer->pop();

	if (Draw) Draw();
	if (zBuffer) ResetZBuffer();

	SDL_UpdateWindowSurface(window);
}

void GameWindow::HandleEvents(void)
{
	SDL_Event ev;
	bool fixMove = false;

	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			*isRunning = 0;
			break;
		case SDL_KEYDOWN:
			if (KeyDown) KeyDown(ev.key.keysym.scancode);
			break;
#ifdef _MOUSE_LOCK
		case SDL_MOUSEMOTION:
			if (ev.motion.x + ev.motion.xrel <= 0 ||
				ev.motion.x + ev.motion.xrel >= width ||
				ev.motion.y + ev.motion.yrel <= 0 ||
				ev.motion.y + ev.motion.yrel >= height)
			{
				SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
				SDL_WarpMouseInWindow(window, width >> 1, height >> 1);
				SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
				fixMove = true;
			}
			else if (MouseMove && !fixMove) MouseMove(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel);
			break;
#endif
		}
	}
}

void GameWindow::ResetZBuffer(void)
{
	uint length = width * height;
	for (size_t i = 0; i < length; i++)
	{
		zBuffer[i] = FLT_MAX;
	}
}