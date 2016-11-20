#include "Main.h"

GameWindow *window;
Camera *cam;
const float scalar = 0.1f;

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", WIDTH, HEIGHT);
	window->Draw = Render;
	window->KeyDown = KeyPress;
	window->MouseMove = MouseMove;

	cam = new Camera(Vect3(0, 0, Z_DIST(1)));

	GF_Init();
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

void Render(void)
{
	cam->Update();

	window->Clear(CLR_BLACK);
	GF_SetViewMatrix(cam->GetView());

	//GF_StartRender(GF_LINES);
	//GF_SetBufferLength(lLen << 1);

	//for (size_t i = 0; i < lLen; i++)
	//{
	//	GF_AddPoint(vertices[lines[i][0]]);
	//	GF_AddPoint(vertices[lines[i][1]]);
	//}

	//GF_EndRender();
	RenderTriangle(2);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE chndl = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cPos = GetConsoleScreenBufferInfo(chndl, &csbi) ? csbi.dwCursorPosition : COORD();
	cPos.Y -= 1;
	SetConsoleCursorPosition(chndl, cPos);
	printf("FPS: %7.3f|%7.3f\n", window->GetAvarageFPS(), window->GetFps());
}