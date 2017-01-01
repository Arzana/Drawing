#include "MouseState.h"

void MouseState::Update(const int newX, const int newY)
{
	if (x != -1)
	{
		dx += x - newX;
		dy += y - newY;
	}
	x = newX;
	y = newY;
	if (!inBounds) inBounds = true;
}

void MouseState::DeltaReset(void)
{
	dx = 0;
	dy = 0;
}
