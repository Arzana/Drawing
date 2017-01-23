#pragma once

#include <Game_WIN.h>
#include <Camera.h>
#include <TriangleRenderer.h>
#include <Shapes.h>
#include "Player.h"

const float lookScalar = 0.1f,
			moveScalar = 0.01f,
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
	TriangleRenderer *renderer;

	const vrtx vertices[32] =
	{
		vrtx(0, 0, 0, CLR_BLUE),
		vrtx(2, 0, 0, CLR_BLUE),
		vrtx(8, 0, 0, CLR_BLUE),
		vrtx(10, 0, 0, CLR_BLUE),
		vrtx(2, 0, 2, CLR_BLUE),
		vrtx(8, 0, 2, CLR_BLUE),
		vrtx(2, 0, 4, CLR_BLUE),
		vrtx(8, 0, 4, CLR_BLUE),
		vrtx(2, 0, 6, CLR_BLUE),
		vrtx(8, 0, 6, CLR_BLUE),
		vrtx(2, 0, 8, CLR_BLUE),
		vrtx(8, 0, 8, CLR_BLUE),
		vrtx(0, 0, 10, CLR_BLUE),
		vrtx(2, 0, 10, CLR_BLUE),
		vrtx(8, 0, 10, CLR_BLUE),
		vrtx(10, 0, 10, CLR_BLUE),
		vrtx(0, -1, 0, CLR_DARKGRAY),
		vrtx(2, -1, 0, CLR_DARKGRAY),
		vrtx(8, -1, 0, CLR_DARKGRAY),
		vrtx(10, -1, 0, CLR_DARKGRAY),
		vrtx(2, -1, 2, CLR_DARKGRAY),
		vrtx(8, -1, 2, CLR_DARKGRAY),
		vrtx(2, -1, 4, CLR_DARKGRAY),
		vrtx(8, -1, 4, CLR_DARKGRAY),
		vrtx(2, -1, 6, CLR_DARKGRAY),
		vrtx(8, -1, 6, CLR_DARKGRAY),
		vrtx(2, -1, 8, CLR_DARKGRAY),
		vrtx(8, -1, 8, CLR_DARKGRAY),
		vrtx(0, -1, 10, CLR_DARKGRAY),
		vrtx(2, -1, 10, CLR_DARKGRAY),
		vrtx(8, -1, 10, CLR_DARKGRAY),
		vrtx(10, -1, 10, CLR_DARKGRAY)
	};

	void OnInitialize(void) override;
	void OnRender(void) override;
	void OnUpdate(GameTime gameTime, const KeyboardState& kstate, const MouseState& mstate) override;
} room;