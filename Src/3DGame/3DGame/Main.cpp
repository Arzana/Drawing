#include "Main.h"

GameWindow *window;

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", 800, 600);
	window->Draw = Render;
	GF_SetWindow(window);
	window->Run();

	delete window;
	printf("Press any key to continue...");
	getchar();
	return EXIT_SUCCESS;
}

void Render(void)
{
	GF_StartRender(GF_TRIANGLES);
	GF_SetBufferLength(3);

	GF_AddPoint(60, 10, CLR_BLUE);
	GF_AddPoint(10, 60, CLR_RED);
	GF_AddPoint(110, 60, CLR_GREEN);

	GF_EndRender();
}