#pragma once

#include <vector>
#include "Game_WIN.h"
#include "Vertex.h"

typedef struct LineFanRenderer
	: public GameComponent
{
public:
	LineFanRenderer(winGame *game);
	LineFanRenderer(void);

	void SetCenter(vrtx v);
	void Add(vrtx v);
	void Draw(void) override;
private:
	std::vector<vrtx> *buff;
	winGame *game;
} linefanRndr;