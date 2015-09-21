#include "Main.h"

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;

Map* map = NULL;
Vector3* player;
Matrix* rotation;
Vector3* playerR;

int main(int args, char* argv[])
{
	player = new VECT_ZERO;
	playerR = new VECT_ZERO;
	rotation = new MTRX_Identity;
	
	InitWindow();
	InitMap();
	
	bool isRunning = true;
	SDL_Event ev;

	while (isRunning)
	{
		while (SDL_PollEvent(&ev) != NULL)
		{
			switch (ev.type)
			{
			case(SDL_QUIT) :
				isRunning = false;
				break;
			case(SDL_KEYDOWN) :
				switch (ev.key.keysym.scancode)
				{
				case(SDL_SCANCODE_W) :
					*player += rotation->GetForward();
					break;
				case(SDL_SCANCODE_S) :
					*player += rotation->GetBackward();
					break;
				case(SDL_SCANCODE_A) :
					*player += rotation->GetLeft();
					break;
				case(SDL_SCANCODE_D) :
					*player += rotation->GetRight();
					break;
				case(SDL_SCANCODE_LEFT) :
					playerR->Y -= 0.1f;
					break;
				case(SDL_SCANCODE_RIGHT) :
					playerR->Y += 0.1f;
					break;
				case(SDL_SCANCODE_DOWN) :
					playerR->X -= 0.1f;
					break;
				case(SDL_SCANCODE_UP) :
					playerR->X += 0.1f;
					break;
				case(SDL_SCANCODE_ESCAPE) :
					isRunning = false;
					break;
				}
				break;
			}
		}

		*rotation = Matrix::FromYawPitchRoll(playerR->Y * DEG2RAD, playerR->X * DEG2RAD, playerR->Z * DEG2RAD);

		RenderScreen();
	}

	DestroyWindow();
	delete map;
	delete player;
	delete playerR;
	delete rotation;
	return EXIT_SUCCESS;
}

void InitWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		ThrowFatalError(SDL_GetError());
		exit(EXIT_FAILURE);
	}

	window = SDL_CreateWindow("Drawing_Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);
}

void InitMap()
{
	map = new Map();
	if (!map->Load("C:\\Users\\Fam. de Jong\\Documents\\Visual Studio 2013\\Projects\\C++\\Drawing\\map.txt"))
	{
		ThrowFatalError(map->GetError());
		exit(EXIT_FAILURE);
	}
}

void ThrowFatalError(const char* message)
{
	SDL_MessageBoxButtonData* bData = new SDL_MessageBoxButtonData();
	bData->flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
	bData->text = "Exit";

	SDL_MessageBoxData* wData = new SDL_MessageBoxData();
	wData->buttons = bData;
	wData->numbuttons = 1;
	wData->window = window;
	wData->title = "An error occured!";
	wData->message = message;

	SDL_ShowMessageBox(wData, NULL);

	delete bData;
	delete wData;
}

void DestroyWindow()
{
	SDL_DestroyWindow(window);
	window = NULL;
	surface = NULL;
	SDL_Quit();
}

void RenderScreen()
{
	int num_lines = 12;
	Line* lines = (Line*)malloc(sizeof(Line) * num_lines);

	// Back wall
	lines[0] = Line(Vector3(5, 5, 5), Vector3(10, 5, 5));		// Top
	lines[1] = Line(Vector3(10, 5, 5), Vector3(10, 10, 5));		// Right
	lines[2] = Line(Vector3(10, 10, 5), Vector3(5, 10, 5));		// Bottom
	lines[3] = Line(Vector3(5, 10, 5), Vector3(5, 5, 5));		// Left
	// Right wall
	lines[4] = Line(Vector3(10, 5, 5), Vector3(10, 5, 0));		// Top
	lines[5] = Line(Vector3(10, 5, 0), Vector3(10, 10, 0));		// Right
	lines[6] = Line(Vector3(10, 10, 0), Vector3(10, 10, 5));	// Bottom
	// Left wall
	lines[7] = Line(Vector3(5, 5, 5), Vector3(5, 5, 0));		// Top
	lines[8] = Line(Vector3(5, 5, 0), Vector3(5, 10, 0));		// Left
	lines[9] = Line(Vector3(5, 10, 0), Vector3(5, 10, 5));		// Bottom
	// Front wall
	lines[10] = Line(Vector3(5, 5, 0), Vector3(10, 5, 0));		// Top
	lines[11] = Line(Vector3(5, 10, 0), Vector3(10, 10, 0));	// Bottom

	Vector3 targ = *player + rotation->GetForward();
	Matrix matrix = Matrix::View(*player, targ, rotation->GetUp()) * Matrix::Perspective(60, ASPR, PLANE_NEAR, PLANE_FAR);

	Line::Multiply(matrix, num_lines, lines);
	num_lines = Line::CheckVisiblity(lines, num_lines);
	
	// Start rendering
	ClearScreen(0x000000);
	for (int i = 0; i < num_lines; i++) WuLine(lines[i]);

	// End rendering
	SDL_UpdateWindowSurface(window);
	free(lines);
}

void WuLine(Line& l)
{
	bool steep = abs(l.B.Y - l.A.Y) > abs(l.B.X - l.A.X);

	if (steep)
	{
		swap(l.A.X, l.A.Y);
		swap(l.B.X, l.B.Y);
	}
	
	if (l.A.X > l.B.X)
	{
		swap(l.A.X, l.B.X);
		swap(l.A.Y, l.B.Y);
	}

	int deltaX = l.B.X - l.A.X;
	int deltaY = l.B.Y - l.A.Y;
	double gradient = double(deltaY) / double(deltaX);

	// Handle first endpoint
	int xEnd = round(l.A.X);
	double yEnd = l.A.Y + gradient * (xEnd - l.A.X);
	double xGap = rfpart(l.A.X + 0.5);
	int xPxL1 = xEnd;				// This will be used in the main loop.
	int yPxL1 = ipart(yEnd);

	if (steep)
	{
		PlotA(yPxL1, xPxL1, rfpart(yEnd) * xGap);
		PlotA(yPxL1 + 1, xPxL1, fpart(yEnd) * xGap);
	}
	else
	{
		PlotA(xPxL1, yPxL1, rfpart(yEnd) * xGap);
		PlotA(xPxL1, yPxL1 + 1, fpart(yEnd) * xGap);
	}

	double intery = yEnd + gradient;	// First y-intersection for the main loop.

	// Handle second endpoint.
	xEnd = round(l.B.X);
	yEnd = l.B.Y + gradient * (xEnd - l.B.X);
	xGap = fpart(l.B.X + 0.5);
	int xPxL2 = xEnd;					// This will be used in the main loop.
	int yPxL2 = ipart(yEnd);

	if (steep)
	{
		PlotA(yPxL2, xPxL2, rfpart(yEnd) * xGap);
		PlotA(yPxL2 + 1, xPxL2, fpart(yEnd) * xGap);
	}
	else
	{
		PlotA(xPxL2, yPxL2, rfpart(yEnd) * xGap);
		PlotA(xPxL2, yPxL2 + 1, fpart(yEnd) * xGap);
	}

	// Main loop.
	for (int x = (xPxL1 + 1); x <= (xPxL2 - 1); x++)
	{
		if (steep)
		{
			PlotA(ipart(intery), x, rfpart(intery));
			PlotA(ipart(intery) + 1, x, fpart(intery));
		}
		else
		{
			PlotA(x, ipart(intery), rfpart(intery));
			PlotA(x, ipart(intery) + 1, fpart(intery));
		}

		intery += gradient;
	}
}

void Plot(int x, int y, int c)
{
	if (x > 0 && y > 0 && x < WINDOW_WIDTH && y < WINDOW_HEIGHT)
	{
		int* pix = (int*)surface->pixels;
		pix[y * ipart(WINDOW_WIDTH) + x] = c;
	}
}

void PlotA(int x, int y, float a)
{
	Plot(x, y, pack(255, 0, 255, a));
}

void ClearScreen(int c)
{
	int* pix = (int*)surface->pixels;
	for (int i = 0; i < WINDOW_HEIGHT * WINDOW_WIDTH; i++) pix[i] = c;
}