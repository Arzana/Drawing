#pragma once

#include <Game_WIN.h>
#include <Camera.h>
#include <TriangleRenderer.h>
#include <LineFanRenderer.h>
#include "Laser.h"
#include "Player.h"

const float moveScalar = 0.1f,
			eyeHeight = 0.5f;

typedef struct RoomGame
	: public winGame
{
public:
	RoomGame(const uint w, const uint h);
	~RoomGame(void);
private:
	Player *plr;
	Camera *cam;
	TriangleRenderer *sceneRenderer;

	const vrtx vertices[32] =
	{
		vrtx(0, 0, 0, CLR_BLUE),
		vrtx(10, 0, 0, CLR_BLUE),
		vrtx(0, 0, 10, CLR_BLUE),
		vrtx(10, 0, 10, CLR_BLUE),
		vrtx(0, -1, 0, CLR_DARKGRAY),
		vrtx(10, -1, 0, CLR_DARKGRAY),
		vrtx(0, -1, 10, CLR_DARKGRAY),
		vrtx(10, -1, 10, CLR_DARKGRAY)
	};

	const wall planes[3]
	{
		wall(vertices[0], vertices[1], vertices[3], vertices[2]),
		wall(vertices[4], vertices[5], vertices[7], vertices[6]),
		wall(vertices[0], vertices[4], vertices[5], vertices[1])
	};

	void OnInitialize(void) override;
	void OnRender(void) override;
	void OnUpdate(GameTime gameTime, const KeyboardState& kstate, const MouseState& mstate) override;
} room;