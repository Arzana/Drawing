#include "Main.h"

GameWindow *window;
Camera *cam;

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", WIDTH, HEIGHT);
	window->Draw = Render;
	window->KeyDown = KeyPress;
	window->MouseMove = MouseMove;

	cam = new Camera(Vect3(0, 0, (2 / sin(FOV_Y / 2 * deg2rad))));

	GF_SetWindow(window);
	GF_SetFrustrum(FOV_Y, aspr, DEPTH_NEAR, DEPTH_FAR);

	window->Run();

	delete window;
	delete cam;

	printf("Press any key to continue...");
	getchar();
	return EXIT_SUCCESS;
}

void MouseMove(int x, int y, int dX, int dY)
{
	//cam->AppendYawDegr(dX * 0.1);
	//cam->AppendPitchDegr(dY * 0.1);
}

void KeyPress(int scanCode)
{
	const float scalar = 0.5f;
	switch (scanCode)
	{
	case 41: // ESC
		window->Terminate();
		break;
	case 26: // W
		cam->Move(VECT3_FORWARD * scalar);
		break;
	case 22: // S
		cam->Move(VECT3_BACK * scalar);
		break;
	case 4: // A
		cam->Move(VECT3_LEFT * scalar);
		break;
	case 7: // D
		cam->Move(VECT3_RIGHT * scalar);
		break;
	case 8: // E
		cam->Move(VECT3_UP * scalar);
		break;
	case 20: // Q
		cam->Move(VECT3_DOWN * scalar);
		break;
	}
}

void Render(void)
{
	cam->Update();

	window->Clear(CLR_BLACK);
	GF_SetViewMatrix(cam->GetView());

	GF_StartRender(GF_TRIANGLES);
	GF_SetBufferLength(3);
	GF_AddPoint(0, -0.5, 1, CLR_RED);
	GF_AddPoint(-0.5, 0.5, 1, CLR_GREEN);
	GF_AddPoint(0.5, 0.5, 1, CLR_BLUE);
	GF_EndRender();

	printf("%7.3f|%7.3f\n", window->GetAvarageFPS(), window->GetFps());
}