#pragma once

#include "Game_WIN.h"
#include "View.h"

typedef struct Camera
	: private View, public GameComponent
{
public:
	Camera(winGame *game, float fov, float nearClip, float farClip);
	Camera(winGame *game, float width, float height, float nearClip, float farClip);
	~Camera(void);

	void Bind(vect3 *pos, vect3 *rot);
	void UnBind(void);
	void Update(GameTime gameTime, const KeyboardState& kstate, const MouseState& mstate) override;
private:
	winGame *game;
	bool *bound;
} cam;