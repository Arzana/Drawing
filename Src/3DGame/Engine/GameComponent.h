#pragma once

#include "Utils.h"
#include "GameTime.h"
#include "MouseState.h"
#include "KeyboardState.h"

typedef struct GameComponent
{
public:
	GameComponent(void) : enabled(new bool(true)) { }
	~GameComponent(void) { delete_s(enabled); }

	virtual void Initialize(void) { }
	virtual void Update(GameTime gameTime, const KeyboardState& kstate, const MouseState& mstate) { }
	virtual void Draw(void) { }

	virtual void Enable(void) { *enabled = true; }
	virtual void Disable(void) { *enabled = false; }
protected:
	bool *enabled;
} gameComp;