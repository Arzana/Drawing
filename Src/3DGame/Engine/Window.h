#pragma once

#include "Vect2.h"
#include "Color.h"

#ifndef EXIT_FAILURE
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1
#endif

class GameWindow
{
public:
	void (*Draw)(void);

	GameWindow(const char *title, const uint width, const uint height);
	~GameWindow();

	void Clear(const Color c);
	void Plot(const Vect2 *v, const Color c);
	void Plot(const uint x, const uint y, const Color c);
	void Plot_S(const Vect2 *v, const Color c);
	void Plot_S(const uint x, const uint y, const Color c);
	void TryPlot(const Vect3 *v, const Color c);
	void TryPlot(const float x, const float y, const float z, const Color c);
	void TryPlot_S(const Vect3 *v, const Color c);
	void TryPlot_S(const float x, const float y, const float z, const Color c);
	void Run(void);
	void Terminate();
	uint GetWidth(void) const;
	uint GetHeight(void) const;

private:
	typedef struct SDL_Window SDL_Window;
	typedef struct SDL_Surface SDL_Surface;

	SDL_Window *window;
	SDL_Surface *surface;
	float *zBuffer;
	const char *title;
	uint width, height;

	void InitWindow(void);
	void TerminateWindow(void);
	bool PointVisible(const uint x, const uint y) const;
	void Tick(void);
	void ResetZBuffer(void);
};