#pragma once

#include <Game_WIN.h>
#include "Camera.h"

typedef struct FireworksGame
	: public winGame
{
public:
	FireworksGame(const uint width, const uint height);
	~FireworksGame(void);
private:
	const float scalar = 0.1f;
	const size_t ppAxis = 30;
	const size_t pSize = cube(ppAxis);

	size_t updCount = 0;
	Camera *cam;
	vect3 *vertices, *vel;

	void OnInitialize(void) override;
	void OnTerminate(void) override;
	void OnUpdate(GameTime time, const KeyboardState kstate) override;
	void OnRender(void) override;
	void OnMouseMove(int x, int y, int dX, int dY) override;
} fireworks;