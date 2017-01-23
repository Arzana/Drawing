#include "Room.h"
#include "Utils.h"

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

	/* Floor */
	renderer->Add(&plane(vertices[0], vertices[1], vertices[13], vertices[12]));
	renderer->Add(&plane(vertices[2], vertices[3], vertices[14], vertices[15]));
	renderer->Add(&plane(vertices[1], vertices[2], vertices[5], vertices[4]));
	renderer->Add(&plane(vertices[10], vertices[11], vertices[14], vertices[13]));
	renderer->Add(&plane(vertices[6], vertices[7], vertices[9], vertices[8]));

	/* Ceiling */
	renderer->Add(&plane(vertices[16], vertices[17], vertices[29], vertices[28]));
	renderer->Add(&plane(vertices[18], vertices[19], vertices[30], vertices[31]));
	renderer->Add(&plane(vertices[17], vertices[18], vertices[21], vertices[20]));
	renderer->Add(&plane(vertices[26], vertices[27], vertices[30], vertices[29]));
	renderer->Add(&plane(vertices[22], vertices[23], vertices[25], vertices[24]));

	/* Walls */
	renderer->Add(&plane(vertices[16], vertices[19], vertices[3], vertices[0]));
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