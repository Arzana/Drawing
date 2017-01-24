#include "Room.h"

RoomGame::RoomGame(const uint w, const uint h)
	: winGame("RoomTest", w, h), plr(new Player(vect3(0, eyeHeight, -10)))
{
	sceneRenderer = new TriangleRenderer(this);
	cam = new Camera(this, 45.0f, 0.1f, 100.0f);
}

RoomGame::~RoomGame(void)
{
	delete_s(cam);
	delete_s(plr);
	delete_s(sceneRenderer);
}

void RoomGame::OnInitialize(void)
{
	cam->Bind(&plr->pos, &plr->rot);
	sceneRenderer->Add(&planes[0].model);
	sceneRenderer->Add(&planes[1].model);
	sceneRenderer->Add(&planes[2].model);
}

void RoomGame::OnRender(void)
{
	printf("FPS: %7.3f|%7.3f\n", GetAvarageFPS(), GetFps());
}

void RoomGame::OnUpdate(GameTime gameTime, const KeyboardState& kstate, const MouseState& mstate)
{
	if (kstate[Keys::Escape]) Terminate();
	plr->Look(mstate.dx * moveScalar * M_DEG2RAD, mstate.dy * moveScalar * M_DEG2RAD);

	/* Update player movement. */
	vect3 dir = VECT3_ZERO;
	if (plr->pos.Y - eyeHeight > 0) plr->Move(VECT3_UP * gameTime.GetDeltaTime());
	else
	{
		if (kstate[Keys::W]) dir.Z += moveScalar * gameTime.GetDeltaTime();
		if (kstate[Keys::S]) dir.Z -= moveScalar * gameTime.GetDeltaTime();
		if (kstate[Keys::A]) dir.X -= moveScalar * gameTime.GetDeltaTime();
		if (kstate[Keys::D]) dir.X += moveScalar * gameTime.GetDeltaTime();
	}

	/* Collision detection */
	plr->Move(dir);
	bool collided = false;
	for (size_t i = 2; i < 3; i++)
	{
		if (planes[i].CheckCollision(plr))
		{
			collided = true;
			break;
		}
	}
	if (collided) plr->Move(-dir);
}