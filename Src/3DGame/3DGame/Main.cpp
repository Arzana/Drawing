#include "Main.h"

GameWindow *window;

int main(int argc, char *argv[])
{
	window = new GameWindow("Test", 800, 600);

	return EXIT_SUCCESS;
}