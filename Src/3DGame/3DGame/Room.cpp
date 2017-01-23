#include "Room.h"

RoomGame::RoomGame(const uint w, const uint h)
	: winGame("RoomTest", w, h), plr(new Player(vect3(0, eyeHeight, -10)))
{
	renderer = new TriangleRenderer(this);
	cam = new Camera(this, 45.0f, 0.1f, 100.0f);
}

RoomGame::~RoomGame(void)
{
	delete_s(cam);
	delete_s(plr);
	delete_s(renderer);
}

void RoomGame::OnInitialize(void)
{
	cam->Bind(&plr->pos, &plr->rot);
	renderer->Add(&planes[0].model);
	renderer->Add(&planes[1].model);
	renderer->Add(&planes[2].model);
}

void RoomGame::OnRender(void)
{
	printf("FPS: %7.3f|%7.3f\n", GetAvarageFPS(), GetFps());
}

void RoomGame::OnUpdate(GameTime gameTime, const KeyboardState& kstate, const MouseState& mstate)
{
	/* Update player movement. */
	if (kstate[Keys::Escape]) Terminate();
	if (kstate[Keys::W]) plr->Move(VECT3_FORWARD * moveScalar);
	if (kstate[Keys::S]) plr->Move(VECT3_BACK * moveScalar);
	if (kstate[Keys::A]) plr->Move(VECT3_LEFT * moveScalar);
	if (kstate[Keys::D]) plr->Move(VECT3_RIGHT * moveScalar);
	plr->Look(mstate.dx * lookScalar * M_DEG2RAD, mstate.dy * lookScalar * M_DEG2RAD);
}