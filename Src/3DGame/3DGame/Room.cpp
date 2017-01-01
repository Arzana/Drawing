#include "Room.h"
#include "Utils.h"

RoomGame::RoomGame(const uint w, const uint h)
	: winGame("RoomTest", w, h)
	, cam(new Camera(vect3(0, 0, -10)))
{
	SetProjection_Frustrum(45.0f, (float)w / h, 0.1f, 100.0f);
}

RoomGame::~RoomGame(void)
{
	delete_s(cam);
}

void RoomGame::OnRender(void)
{
	SetView(cam->Update());
	Clear(CLR_BLACK);

	Start(GF_TRIANGLES);
	SetBufferLength(tLen * 3);

	for (size_t i = 0; i < tLen; i++)
	{
		AddVertex(vertices[trgls[i][0]]);
		AddVertex(vertices[trgls[i][1]]);
		AddVertex(vertices[trgls[i][2]]);
	}

	if (!End()) Terminate();
}

void RoomGame::OnUpdate(GameTime gameTime, const KeyboardState& kstate, const MouseState& mstate)
{
	if (kstate[Keys::Escape]) Terminate();
	if (kstate[Keys::W]) cam->Move(VECT3_FORWARD * scalar);
	if (kstate[Keys::S]) cam->Move(VECT3_BACK * scalar);
	if (kstate[Keys::A]) cam->Move(VECT3_LEFT * scalar);
	if (kstate[Keys::D]) cam->Move(VECT3_RIGHT * scalar);

	cam->AppendYawDegr(mstate.dx * scalar);
	cam->AppendPitchDegr(mstate.dy * scalar);
}