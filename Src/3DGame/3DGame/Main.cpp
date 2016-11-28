#include "Main.h"

int main(int argc, char *argv[])
{
	fireworks *game = new fireworks(WIDTH, HEIGHT);
	game->Run();
	delete game;

	printf("Press any key to continue...");
	getchar();
}