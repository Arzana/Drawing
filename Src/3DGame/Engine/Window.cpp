#include "Utils.h"
#include "Window.h"
#include <SDL.h>
#include <cfloat>

int *isRunning;

GameWindow::GameWindow(const char * title, const uint width, const uint height)
{
	this->title = title;
	this->width = width;
	this->height = height;
	zBuffer = malloc_s(float, width * height);
	isRunning = NULL;
	Draw = NULL;

	InitWindow();
	ResetZBuffer();
}

GameWindow::~GameWindow()
{
	TerminateWindow();
	if (zBuffer) free(zBuffer);
	if (isRunning) delete isRunning;
}

void GameWindow::Clear(const Color c)
{
	Color *pix = (Color*)surface->pixels;
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			pix[y * width + x] = c;
		}
	}
}

void GameWindow::Plot(const Vect2 * v, const Color c)
{
	Plot(ipart(v->X), ipart(v->Y), c);
}

void GameWindow::Plot(const uint x, const uint y, const Color c)
{
	Color *pix = (Color*)surface->pixels;
	pix[y * width + x] = c;
}

void GameWindow::Plot_S(const Vect2 * v, const Color c)
{
	if (PointVisible(ipart(v->X), ipart(v->Y))) Plot(v, c);
}

void GameWindow::Plot_S(const uint x, const uint y, const Color c)
{
	if (PointVisible(x, y)) Plot(x, y, c);
}

void GameWindow::TryPlot(const Vect3 * v, const Color c)
{
	uint i = v->Y * width + v->X;
	if (v->Z < zBuffer[i]) Plot(v->X, v->Y, c);
}

void GameWindow::TryPlot(const float x, const float y, const float z, const Color c)
{
	TryPlot(&Vect3(x, y, z), c);
}

void GameWindow::TryPlot_S(const Vect3 * v, const Color c)
{
	if (PointVisible(v->X, v->Y)) TryPlot(v, c);
}

void GameWindow::TryPlot_S(const float x, const float y, const float z, const Color c)
{
	TryPlot_S(&Vect3(x, y, z), c);
}

void GameWindow::Run(void)
{
	if (!isRunning) isRunning = new int(1);
	else *isRunning = 1;

	while (*isRunning)
	{
		Tick();
		if (Draw) Draw();
		SDL_UpdateWindowSurface(window);
		ResetZBuffer();
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

void GameWindow::InitWindow(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) != NULL) exit(EXIT_FAILURE);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);
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

bool GameWindow::PointVisible(const uint x, const uint y) const
{
	return x > 0 && x < width && y > 0 && y < height;
}

void GameWindow::Tick()
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			*isRunning = 0;
			break;
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