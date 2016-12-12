#include "Main.h"
#include <WinLogger.h>

int main(int argc, char *argv[])
{
	room *game = new room(WIDTH, HEIGHT);
	game->Run();
	delete game;

	char c;
	printf("Press the ENTER key to continue...");
	scanf("%c", &c);
}