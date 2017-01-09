#pragma once

#include "Game_WIN.h"
#include "Triangle.h"

typedef struct TriangleRenderer
	: public GameComponent
{
public:
	TriangleRenderer(winGame *game, size_t length);
	~TriangleRenderer(void);

	void Add(trgl trgl);
	void Draw(void) override;
private:
	size_t len, i;
	vect4 *hBuff;
	clr *cBuff;
	winGame *game;
} trglRndr;