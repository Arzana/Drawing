#include "Main.h"

GameWindow *window;
float rot = 0;
Vect3 camPos = Vect3(0, 0, 2);

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", WIDTH, HEIGHT);
	window->Draw = Render;
	window->KeyDown = KeyPress;

	GF_SetWindow(window);
	GF_SetFrustrum(FOV_Y, aspr, DEPTH_NEAR, DEPTH_FAR);

	window->Run();

	delete window;
	printf("Press any key to continue...");
	getchar();
	return EXIT_SUCCESS;
}

void KeyPress(int scanCode)
{
	switch (scanCode)
	{
	case 41: // ESC
		window->Terminate();
		break;
	}
}

void Render(void)
{
	window->Clear(CLR_BLACK);

	rot += rot < 360 ? 0.5 : -rot;
	Mtrx4 rotation = Mtrx4::CreateYawPitchRoll(rot * deg2rad, rot * deg2rad, rot * deg2rad);
	GF_SetViewMatrix(&Mtrx4::CreateView(&camPos, &(camPos + rotation.Forwards()), &rotation.Up()));

	GF_StartRender(GF_LINE_LOOP);
	GF_SetBufferLength(4);
	GF_AddPoint(-0.5, -0.5, -2, CLR_BLACK);
	GF_AddPoint(0.5, -0.5, -2, CLR_RED);
	GF_AddPoint(0.5, 0.5, -2, CLR_GREEN);
	GF_AddPoint(-0.5, 0.5, -2, CLR_BLUE);
	GF_EndRender();

	GF_StartRender(GF_LINE_LOOP);
	GF_SetBufferLength(4);
	GF_AddPoint(-0.5, -0.5, -3, CLR_WHITE);
	GF_AddPoint(0.5, -0.5, -3, CLR_CYAN);
	GF_AddPoint(0.5, 0.5, -3, CLR_MAGENTA);
	GF_AddPoint(-0.5, 0.5, -3, CLR_YELLOW);
	GF_EndRender();

	GF_StartRender(GF_LINES);
	GF_SetBufferLength(8);
	GF_AddPoint(-0.5, -0.5, -2, CLR_BLACK);
	GF_AddPoint(-0.5, -0.5, -3, CLR_WHITE);
	GF_AddPoint(0.5, -0.5, -2, CLR_RED);
	GF_AddPoint(0.5, -0.5, -3, CLR_CYAN);
	GF_AddPoint(0.5, 0.5, -2, CLR_GREEN);
	GF_AddPoint(0.5, 0.5, -3, CLR_MAGENTA);
	GF_AddPoint(-0.5, 0.5, -2, CLR_BLUE);
	GF_AddPoint(-0.5, 0.5, -3, CLR_YELLOW);
	GF_EndRender();
}