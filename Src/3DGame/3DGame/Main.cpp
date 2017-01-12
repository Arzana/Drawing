#include "Main.h"

int main(int argc, char *argv[])
{
	SetWindowPos(GetConsoleWindow(), HWND_TOP, -1920, 0, 1000, 600, SWP_NOZORDER);

	room *game = new room(WIDTH, HEIGHT);
	game->Run();
	delete game;

	char c;
	printf("Press the ENTER key to continue...");
	scanf("%c", &c);
}