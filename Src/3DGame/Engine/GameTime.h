#pragma once

#include <ctime>

typedef clock_t TimeSpan;

typedef struct GameTime
{
	TimeSpan elapsedGameTime, totalGameTime;
	bool lag;

	GameTime(void) : elapsedGameTime(0), totalGameTime(0), lag(false) { }
	inline float GetDeltaTime(void) { return elapsedGameTime / (float)CLOCKS_PER_SEC; }
} gameTime;