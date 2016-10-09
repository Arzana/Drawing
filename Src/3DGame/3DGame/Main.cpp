#include "Main.h"

GameWindow *window;

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", 800, 600);
	window->Draw = Render;
	window->Run();

	delete window;
	return EXIT_SUCCESS;
}

void Render(void)
{
	window->Plot(&Vect2(10, 10), Color(255, 0, 0));
}