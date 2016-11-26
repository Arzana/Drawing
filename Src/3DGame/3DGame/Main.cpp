#include "Main.h"

GameWindow *window;
Camera *player;
const float scalar = 0.1f;

const size_t ppAxis = 50;
const size_t pSize = cube(ppAxis);
size_t updCount = 0;
vect3 *vertices, *vel;

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", WIDTH, HEIGHT);
	window->OnInitialize = Init;
	window->OnTerminate = Term;
	window->Update = Update;
	window->Draw = Render;
	window->KeyDown = KeyPress;
	window->MouseMove = MouseMove;

	player = new Camera(vect3(0, 0, Z_DIST(1)));

	GF_Init(OPTMZ_POINTS);
	GF_SetWindow(window);
	GF_SetFrustrum(FOV_Y, aspr, DEPTH_NEAR, DEPTH_FAR);

	window->Run();
	GF_End();

	delete window;
	delete player;

	printf("Press any key to continue...");
	getchar();
	return EXIT_SUCCESS;
}

void MouseMove(int x, int y, int dX, int dY)
{
	player->AppendYawDegr(dX * scalar);
	player->AppendPitchDegr(dY * scalar);
}

void KeyPress(int scanCode)
{
	switch (scanCode)
	{
	case VK_ESCAPE: // ESC
		window->Terminate();
		break;
	case 0x57: // W
		player->Move(VECT3_FORWARD * scalar);
		break;
	case 0x53: // S
		player->Move(VECT3_BACK * scalar);
		break;
	case 0x41: // A
		player->Move(VECT3_LEFT * scalar);
		break;
	case 0x44: // D
		player->Move(VECT3_RIGHT * scalar);
		break;
	case 0x45: // E
		player->Move(VECT3_UP * scalar);
		break;
	case 0x51: // Q
		player->Move(VECT3_DOWN * scalar);
		break;
	}
}

void Init(void)
{
	printf("Starting particle creation.\n");
	GF_SetFlagVBuff(true);
	GF_SetBufferLength(pSize);
	vel = malloc_s(vect3, pSize);
	vertices = GF_GetVectBuffer();

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

				GF_AddPoint(VECT3_ZERO, Color(red, green, blue));
				vel[xyz2i(roll, pitch, yaw, ppAxis, ppAxis)] = V4ToV3(&(m * (VECT3_BACK * (rand() % 100) * 0.0001f)));
			}
		}
	}

	printf("Particle creation complete.\nCreated %d vertexes.\n", pSize);
}

void Term(void)
{
	free_s(vel);
	GF_SetFlagVBuff(false);
}

void Update(void)
{
	concurrency::array_view<vect3, 1> a(pSize, vertices);
	concurrency::array_view<vect3, 1> b(pSize, vel);

	concurrency::parallel_for_each(
		a.extent,
		[=](concurrency::index<1> i) __GPU_ONLY
	{
		a[i] += b[i];
	});
}

void Render(void)
{
	player->Update();

	window->Clear(CLR_BLACK);
	GF_SetViewMatrix(player->GetView());

	GF_StartRender(GF_POINTS);
	if (GF_EndRender()) return;

#pragma region FPS VIEW
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE chndl = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cPos = GetConsoleScreenBufferInfo(chndl, &csbi) ? csbi.dwCursorPosition : COORD();
	cPos.Y -= 1;
	SetConsoleCursorPosition(chndl, cPos);
	printf("FPS: %7.3f|%7.3f\n", window->GetAvarageFPS(), window->GetFps());
#pragma endregion
}