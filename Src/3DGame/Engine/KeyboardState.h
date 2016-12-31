#pragma once

#include "MathEx.h"
#include "Keys.h"

typedef struct KeyboardState
{
	uint keys0, keys1, keys2, keys3, keys4, keys5, keys6, keys7;

	KeyboardState(void)
		: keys0(0), keys1(0), keys2(0), keys3(0), keys4(0), keys5(0), keys6(0), keys7(0)
	{ }

	inline bool operator[](const Keys& key) const { return IsDown(key); }
	inline KeyboardState operator<<(const Keys& key) { SetDown(key); return *this; }
	inline KeyboardState operator>>(const Keys& key) { SetUp(key); return *this; }

	bool IsDown(Keys key) const;
	void SetDown(Keys key);
	void SetUp(Keys key);
	void Clear(void);
} kstate;