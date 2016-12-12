#include "Room.h"
#include "Utils.h"

RoomGame::RoomGame(const uint w, const uint h)
	: winGame("RoomTest", w, h)
	, cam(new Camera(vect3(0, 0, -10)))
{
	SetProjection_Frustrum(45.0f, w / h, 0.1f, 100.0f);
}

RoomGame::~RoomGame(void)
{
	delete_s(cam);
}

void RoomGame::OnUpdate(GameTime time)
{
	cam->Update();
}

void RoomGame::OnRender(void)
{
	SetView(cam->GetView());
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

void RoomGame::OnMouseMove(int x, int y, int dX, int dY)
{
	cam->AppendYawDegr(dX * scalar);
	cam->AppendPitchDegr(dY * scalar);
}

void RoomGame::OnKeyDown(int sym)
{
	switch (sym)
	{
	case VK_ESCAPE: // ESC
		Terminate();
		break;
	case 0x57: // W
		cam->Move(VECT3_FORWARD * scalar);
		break;
	case 0x53: // S
		cam->Move(VECT3_BACK * scalar);
		break;
	case 0x41: // A
		cam->Move(VECT3_LEFT * scalar);
		break;
	case 0x44: // D
		cam->Move(VECT3_RIGHT * scalar);
		break;
	case 0x45: // E
		cam->Move(VECT3_UP * scalar);
		break;
	case 0x51: // Q
		cam->Move(VECT3_DOWN * scalar);
		break;
	}
}