#include "Room.h"
#include "Utils.h"

RoomGame::RoomGame(const uint w, const uint h)
	: winGame("RoomTest", w, h), ppos(0, eyeHeight, -10), prot(VECT3_ZERO)
{
	renderer = new TriangleRenderer(this, tLen);
	cam = new Camera(this, 45.0f, 0.1f, 100.0f);
}

RoomGame::~RoomGame(void)
{
	delete_s(cam);
	delete_s(renderer);
}

void RoomGame::OnInitialize(void)
{
	cam->Bind(&ppos, &prot);

	for (size_t i = 0; i < tLen; i++)
	{
		renderer->Add(trgl(vertices[trgls[i][0]], vertices[trgls[i][1]], vertices[trgls[i][2]]));
	}
}

void RoomGame::OnRender(void)
{
	printf("FPS: %7.3f|%7.3f\n", GetAvarageFPS(), GetFps());
}

void Move(vect3 *old, vect3 *rot, vect3 dir)
{
	(*old) += vect4::ToNDC(mtrx4::CreateRotationQ(rot->X, 0, rot->Z) * dir);
}

void RoomGame::OnUpdate(GameTime gameTime, const KeyboardState& kstate, const MouseState& mstate)
{
	/* Update player movement. */
	if (kstate[Keys::Escape]) Terminate();
	if (kstate[Keys::W]) Move(&ppos, &prot, VECT3_FORWARD * moveScalar);
	if (kstate[Keys::S]) Move(&ppos, &prot, VECT3_BACK * moveScalar);
	if (kstate[Keys::A]) Move(&ppos, &prot, VECT3_LEFT * moveScalar);
	if (kstate[Keys::D]) Move(&ppos, &prot, VECT3_RIGHT * moveScalar);
	prot.X += (mstate.dx * lookScalar) * M_DEG2RAD;
	prot.Y += (mstate.dy * lookScalar) * M_DEG2RAD;
}