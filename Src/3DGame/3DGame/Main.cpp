#include "Main.h"

GameWindow *window;
Camera *cam;
const float scalar = 0.1f;

const size_t ppAxis = 25;
const size_t pSize = cube(ppAxis);
struct particle { Vrtx vrtx; Vect3 vel; } part[pSize];

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", WIDTH, HEIGHT);
	window->OnInitialize = Init;
	window->Update = Update;
	window->Draw = Render;
	window->KeyDown = KeyPress;
	window->MouseMove = MouseMove;

	cam = new Camera(Vect3(0, 0, Z_DIST(1)));

	GF_Init(OPTMZ_POINTS);
	GF_SetWindow(window);
	GF_SetFrustrum(FOV_Y, aspr, DEPTH_NEAR, DEPTH_FAR);

	printf("...\n");
	window->Run();
	GF_End();

	delete window;
	delete cam;

	printf("Press any key to continue...");
	getchar();
	return EXIT_SUCCESS;
}

void MouseMove(int x, int y, int dX, int dY)
{
	cam->AppendYawDegr(dX * scalar);
	cam->AppendPitchDegr(dY * scalar);
}

void KeyPress(int scanCode)
{
	switch (scanCode)
	{
	case VK_ESCAPE: // ESC
		window->Terminate();
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

void Init(void)
{
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
				Mtrx4 m = Mtrx4::CreateRotationQ(&Quat::CreateYawPitchRoll(yawf, pitchf, rollf));
				Vect4 vel = m * (VECT3_BACK * (rand() % 100) * 0.0001f);

				part[xyz2i(roll, pitch, yaw, ppAxis, ppAxis)] =
				{
					Vrtx(VECT3_ZERO, Color(red, green, blue)),
					Vect3(vel.X, vel.Y, vel.Z)
				};
			}
		}
	}
}

size_t updCount = 0;
void Update(void)
{
	++updCount;
	for (size_t i = 0; i < pSize; i++)
	{
		part[i].vrtx.v += part[i].vel;
	}

	if (updCount > 500)
	{
		updCount = 0;
		for (size_t i = 0; i < pSize; i++)
		{
			part[i].vel *= -1;
		}
	}
}

void Render(void)
{
	cam->Update();

	window->Clear(CLR_BLACK);
	GF_SetViewMatrix(cam->GetView());

	GF_StartRender(GF_POINTS);
	GF_SetBufferLength(pSize);
	for (size_t i = 0; i < pSize; i++)
	{
		GF_AddPoint(part[i].vrtx);
	}
	GF_EndRender();

#pragma region FPS VIEW
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE chndl = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cPos = GetConsoleScreenBufferInfo(chndl, &csbi) ? csbi.dwCursorPosition : COORD();
	cPos.Y -= 1;
	SetConsoleCursorPosition(chndl, cPos);
	printf("FPS: %7.3f|%7.3f\n", window->GetAvarageFPS(), window->GetFps());
#pragma endregion
}