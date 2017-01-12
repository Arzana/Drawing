#pragma once

#include <vector>
#include "Game_WIN.h"
#include "Geon.h"

typedef struct TriangleRenderer
	: public GameComponent
{
public:
	TriangleRenderer(winGame *game);
	~TriangleRenderer(void);

	void Add(trgl trgl);
	void Add(const geon *geon);
	void Draw(void) override;
private:
	std::vector<vrtx> *buff;
	winGame *game;
} trglRndr;