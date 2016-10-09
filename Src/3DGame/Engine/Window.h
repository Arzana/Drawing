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
	GameWindow(const char *title, const uint width, const uint height);
	~GameWindow();

	void Clear(const Color c);
	void Plot(const Vect2 *v, const Color c);
	void Plot(const uint x, const uint y, const Color c);
	void Plot_S(const Vect2 *v, const Color c);
	void Plot_S(const uint x, const uint y, const Color c);

private:
	typedef struct SDL_Window SDL_Window;
	typedef struct SDL_Surface SDL_Surface;

	SDL_Window *window;
	SDL_Surface *surface;
	const char *title;
	uint width, height;

	void InitWindow(void);
	void TerminateWindow(void);
	bool PointVisible(const uint x, const uint y) const;
};