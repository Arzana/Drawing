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
	switch (scanCode)
	{
	case 41: // ESC
		window->Terminate();
		break;
	case 26: // W
		cam->Move(VECT3_FORWARD);
		break;
	case 22: // S
		cam->Move(VECT3_BACK);
		break;
	case 4: // A
		cam->Move(VECT3_LEFT);
		break;
	case 7: // D
		cam->Move(VECT3_RIGHT);
		break;
	}
}

void Render(void)
{
	cam->Update();

	window->Clear(CLR_BLACK);
	GF_SetViewMatrix(cam->GetView());

	RenderCube(CUBE_LINES);

	printf("FPS: %f\n", window->GetFps());
}