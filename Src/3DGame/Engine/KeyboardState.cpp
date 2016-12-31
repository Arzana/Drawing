#include "KeyboardState.h"

bool KeyboardState::IsDown(Keys key) const
{
	uint element;
	switch ((uint)key >> 5)
	{
	case 0:
		element = keys0;
		break;
	case 1:
		element = keys1;
		break;
	case 2:
		element = keys2;
		break;
	case 3:
		element = keys3;
		break;
	case 4:
		element = keys4;
		break;
	case 5:
		element = keys5;
		break;
	case 6:
		element = keys6;
		break;
	case 7:
		element = keys7;
		break;
	default:
		element = 0;
		break;
	}

	return element & (1 << ((uint)key & 0x1f));
}

void KeyboardState::SetDown(Keys key)
{
	uint mask = 1 << ((uint)key & 0x1f);
	switch ((uint)key >> 5)
	{
	case 0:
		keys0 |= mask;
		break;
	case 1:
		keys1 |= mask;
		break;
	case 2:
		keys2 |= mask;
		break;
	case 3:
		keys3 |= mask;
		break;
	case 4:
		keys4 |= mask;
		break;
	case 5:
		keys5 |= mask;
		break;
	case 6:
		keys6 |= mask;
		break;
	case 7:
		keys7 |= mask;
		break;
	}
}

void KeyboardState::SetUp(Keys key)
{
	uint mask = ~(1 << ((uint)key & 0x1f));
	switch ((uint)key >> 5)
	{
	case 0:
		keys0 &= mask;
		break;
	case 1:
		keys1 &= mask;
		break;
	case 2:
		keys2 &= mask;
		break;
	case 3:
		keys3 &= mask;
		break;
	case 4:
		keys4 &= mask;
		break;
	case 5:
		keys5 &= mask;
		break;
	case 6:
		keys6 &= mask;
		break;
	case 7:
		keys7 &= mask;
		break;
	}
}

void KeyboardState::Clear(void)
{
	keys0 = 0;
	keys1 = 0;
	keys2 = 0;
	keys3 = 0;
	keys4 = 0;
	keys5 = 0;
	keys6 = 0;
	keys7 = 0;
}
