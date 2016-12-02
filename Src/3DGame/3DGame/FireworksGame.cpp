#define _VECT_CONV

#include "FireworksGame.h"
#include <MathEx.h>
#include <Utils.h>

#define FOV_Y		45.0f
#define Z_DIST(r)	((r) / sinf(FOV_Y / 2 * M_DEG2RAD))

using namespace concurrency;

FireworksGame::FireworksGame(const uint width, const uint height)
	: winGame("Fireworks", width, height)
	, cam(new Camera(vect3(0, 0, Z_DIST(1))))
	, vertices(NULL), vel(NULL)
{
	SetFlag_VertexBuffering(true);
	SetProjection_Frustrum(FOV_Y, width / height, 0.1f, 100.0f);
}

FireworksGame::~FireworksGame(void)
{
	delete_s(cam);
	free_s(vel);
}

void FireworksGame::OnInitialize(void)
{
	printf("Starting particle creation.\n");
	SetBufferLength(pSize);
	vel = malloc_s(vect3, pSize);
	vertices = GetVertexBuffer();

	for (size_t yaw = 0; yaw < ppAxis; yaw++)
	{
		float yawf = lerp(0, M_TAU, invLerp(0, ppAxis, yaw));
		int red = lerp(0, 255, invLerp(0, ppAxis, yaw));
		for (size_t pitch = 0; pitch < ppAxis; pitch++)
		{
			float pitchf = lerp(0, M_TAU, invLerp(0, ppAxis, pitch));
			int green = lerp(0, 255, invLerp(0, ppAxis, pitch));
			for (size_t roll = 0; roll < ppAxis; roll++)
			{
				float rollf = lerp(0, M_TAU, invLerp(0, ppAxis, roll));
				int blue = lerp(0, 255, invLerp(0, ppAxis, roll));
				mtrx4 m = mtrx4::CreateRotationQ(&quat::CreateYawPitchRoll(yawf, pitchf, rollf));

				AddVertex(VECT3_ZERO, Color(red, green, blue));
				vel[xyz2i(roll, pitch, yaw, ppAxis, ppAxis)] = V4ToV3(m * (VECT3_BACK * (rand() % 100) * 0.0001f));
			}
		}
	}

	printf("Particle creation complete.\nCreated %d vertexes.\n\n", pSize);
}

void FireworksGame::OnTerminate(void)
{
	SetFlag_VertexBuffering(false);
}

void FireworksGame::OnUpdate(GameTime time)
{
	array_view<vect3, 1> a(pSize, vertices);
	array_view<vect3, 1> b(pSize, vel);

	parallel_for_each(
		a.extent,
		[=](index<1> i) __GPU_ONLY
	{
		a[i] += b[i];
	});

	if (++updCount > 500)
	{
		updCount = 0;
		parallel_for_each(
			a.extent,
			[=](index<1> i) __GPU_ONLY
		{
			b[i] *= -1;
		});
	}
}

void FireworksGame::OnRender(void)
{
	cam->Update();

	Clear(CLR_BLACK);
	SetView(cam->GetView());

	Start(GF_POINTS);
	End();

#pragma region FPS VIEW
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE chndl = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cPos = GetConsoleScreenBufferInfo(chndl, &csbi) ? csbi.dwCursorPosition : COORD();
	cPos.Y -= 1;
	SetConsoleCursorPosition(chndl, cPos);
	printf("FPS: %7.3f|%7.3f\n", GetAvarageFPS(), GetFps());
#pragma endregion
}

void FireworksGame::OnMouseMove(int x, int y, int dX, int dY)
{
	cam->AppendYawDegr(dX * scalar);
	cam->AppendPitchDegr(dY * scalar);
}

void FireworksGame::OnKeyDown(int sym)
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