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

	//GF_StartRender(GF_LINES);
	//GF_SetBufferLength(lLen << 1);

	//for (size_t i = 0; i < lLen; i++)
	//{
	//	GF_AddPoint(vertices[lines[i][0]]);
	//	GF_AddPoint(vertices[lines[i][1]]);
	//}

	//GF_EndRender();
	RenderTriangle(2);

	printf("%7.3f|%7.3f\n", window->GetAvarageFPS(), window->GetFps());
}