#include "Main.h"

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
int *pixels = NULL;

Map *map = NULL;
Vector3 *player;
Matrix *rotation;
Vector3 *playerR;

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
	delete map, player, playerR, rotation;
	return EXIT_SUCCESS;
}

void InitWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		ThrowFatalError(SDL_GetError());	// SDL failed to initialze the graphics so throw the last known error.
		exit(EXIT_FAILURE);
	}

	window = SDL_CreateWindow("Drawing_Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);	// Create the new window in the center of the screen and show it.
}

void InitMap()
{
	map = new Map();
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{										// Could not get the current directory (Admin only?)
		ThrowFatalError("Could not get current directory.");
		exit(EXIT_FAILURE);
	}

	strcat_s(cCurrentPath, "\\map.txt");	// Add the given string to the current path to get the map path.

	if (!map->Load(cCurrentPath))
	{										// An error occured while loading the map so throw the last known error.
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
	/* Construct the vertex buffer. */
	const int VerticesInTriangle = 3;
	int mapLength = map->GetLength();
	int vertexLength = VerticesInTriangle * mapLength * 2;
	Vector3 *vertices = (Vector3*)malloc(sizeof(Vector3) * vertexLength);

	int vertexIndex = 0;
	for (int sectorIndex = 0; sectorIndex < mapLength; sectorIndex++)
	{
		Sector *curSector = map->GetSector(sectorIndex);
		float sectorCeiling = curSector->GetCeiling();
		const Vector3 **sectorVertices = curSector->GetVertices();

		for (int i = 0; i < VerticesInTriangle; i++)
		{
			vertices[vertexIndex++] = Vector3(sectorVertices[i]->X, sectorCeiling, sectorVertices[i]->Z);
		}

		for (int i = 0; i < VerticesInTriangle; i++)
		{
			vertices[vertexIndex++] = *sectorVertices[i];
		}

		delete sectorVertices;
	}

	/* Initalize the matrices. */
	Vector3 targ = *player + rotation->GetForward();
	Matrix modelM = MTRX_Identity;
	Matrix viewM = Matrix::View(*player, targ, rotation->GetUp());
	Matrix projM = Matrix::Perspective(60, ASPR, 0.1f, PLANE_FAR);
	Matrix mvp = modelM * viewM * projM;

	/* Transform the vertices. */
	Matrix::Transform(&mvp, vertices, vertexLength);

	/* Start rendering. */
	pixels = (int*)surface->pixels;
	ClearScreen(0x000000);
	for (int i = 0; i < vertexLength; i += VerticesInTriangle)
	{
		Triangle current = Triangle(vertices[i], vertices[i + 1], vertices[i + 2]);
		if (/*Triangle::CheckVisibility(&current, 1)*/1) RenderTriangle(&current, 0x00FF00);
	}

	/* End rendering. */
	SDL_UpdateWindowSurface(window);
	pixels = NULL;
	free(vertices);
}

void FillBottomFlatTriangle(Triangle *t, int c)
{
	float invSlope1 = (t->B.X - t->A.X) / (t->B.Y - t->A.Y);
	float invSlope2 = (t->C.X - t->A.X) / (t->C.Y - t->A.Y);

	float curX1 = t->A.X;
	float curX2 = t->A.X;

	for (int scanLineY = t->A.Y; scanLineY <= t->B.Y; scanLineY++)
	{
		BresenhamLine(round(curX1), scanLineY, round(curX2), scanLineY, c);
		curX1 += invSlope1;
		curX2 += invSlope2;
	}
}

void FillTopFlatTriangle(Triangle *t, int c)
{
	float invSlope1 = (t->C.X - t->A.X) / (t->C.Y - t->A.Y);
	float invSlope2 = (t->C.X - t->B.X) / (t->C.Y - t->B.Y);

	float curX1 = t->C.X;
	float curx2 = t->C.X;

	for (int scanlineY = t->C.Y; scanlineY > t->A.Y; scanlineY--)
	{
		curX1 -= invSlope1;
		curx2 -= invSlope2;
		BresenhamLine(round(curX1), scanlineY, round(curx2), scanlineY, c);
	}
}

void RenderTriangle(Triangle *t, int c)
{
	t->SortVerticesByY();

	if (t->B.Y == t->C.Y) FillBottomFlatTriangle(t, c);
	else if (t->B.Y == t->A.Y) FillTopFlatTriangle(t, c);
	else
	{
		Vector3 v4 = Vector3(ipart(t->A.X + ((float)(t->B.Y - t->A.Y) / (float)(t->C.Y - t->A.Y)) * (t->C.X - t->A.X)), t->B.Y, 0);

		Triangle *temp = &Triangle(t->A, t->B, v4);
		FillBottomFlatTriangle(temp, c);

		t = &Triangle(t->B, v4, t->C);
		FillTopFlatTriangle(t, c);
	}
}

void BresenhamLine(float x0, float y0, float x1, float y1, int c)
{
	bool steep = abs(y1 - y0) > abs(x1 - x0);	// Check if the line is steep.

	if (steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	float deltaX = x1 - x0;
	float deltaY = y1 - y0;
	int sign = deltaY == 0 ? 0 : (deltaY > 0 ? 1 : -1);
	float gradient = abs(deltaY / deltaX);

	float error = 0;

	if (steep)
	{
		for (int x = x0, y = y0; x <= x1; x++)
		{
			Plot(y, x, c);
			error += gradient;

			while (error > 0.5f)
			{
				y += sign;
				Plot(y, x, c);
				error -= 1;
			}
		}
	}
	else
	{
		for (int x = x0, y = y0; x <= x1; x++)
		{
			Plot(x, y, c);
			error += gradient;

			while (error >= 0.5f)
			{
				y += sign;
				Plot(x, y, c);
				error -= 1;
			}
		}
	}
}

void WuLine(Line *l, int c)
{
	bool steep = abs(l->B.Y - l->A.Y) > abs(l->B.X - l->A.X);

	if (steep)
	{
		swap(l->A.X, l->A.Y);
		swap(l->B.X, l->B.Y);
	}

	if (l->A.X > l->B.X)
	{
		swap(l->A.X, l->B.X);
		swap(l->A.Y, l->B.Y);
	}

	int deltaX = l->B.X - l->A.X;
	int deltaY = l->B.Y - l->A.Y;
	double gradient = double(deltaY) / double(deltaX);

	// Handle first endpoint
	int xEnd = round(l->A.X);
	double yEnd = l->A.Y + gradient * (xEnd - l->A.X);
	double xGap = rfpart(l->A.X + 0.5);
	int xPxL1 = xEnd;					// This will be used in the main loop.
	int yPxL1 = ipart(yEnd);

	if (steep)
	{
		PlotA(yPxL1, xPxL1, rfpart(yEnd) * xGap, c);
		PlotA(yPxL1 + 1, xPxL1, fpart(yEnd) * xGap, c);
	}
	else
	{
		PlotA(xPxL1, yPxL1, rfpart(yEnd) * xGap, c);
		PlotA(xPxL1, yPxL1 + 1, fpart(yEnd) * xGap, c);
	}

	double intery = yEnd + gradient;	// First y-intersection for the main loop.

	// Handle second endpoint.
	xEnd = round(l->B.X);
	yEnd = l->B.Y + gradient * (xEnd - l->B.X);
	xGap = fpart(l->B.X + 0.5);
	int xPxL2 = xEnd;					// This will be used in the main loop.
	int yPxL2 = ipart(yEnd);

	if (steep)
	{
		PlotA(yPxL2, xPxL2, rfpart(yEnd) * xGap, c);
		PlotA(yPxL2 + 1, xPxL2, fpart(yEnd) * xGap, c);
	}
	else
	{
		PlotA(xPxL2, yPxL2, rfpart(yEnd) * xGap, c);
		PlotA(xPxL2, yPxL2 + 1, fpart(yEnd) * xGap, c);
	}

	// Main loop.
	for (int x = (xPxL1 + 1); x <= (xPxL2 - 1); x++)
	{
		if (steep)
		{
			PlotA(ipart(intery), x, rfpart(intery), c);
			PlotA(ipart(intery) + 1, x, fpart(intery), c);
		}
		else
		{
			PlotA(x, ipart(intery), rfpart(intery), c);
			PlotA(x, ipart(intery) + 1, fpart(intery), c);
		}

		intery += gradient;
	}
}

void Plot(int x, int y, int c)
{
	if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT)
	{
		pixels[y * ipart(WINDOW_WIDTH) + x] = c;
	}
}

void PlotA(int x, int y, float a, int c)
{
	Plot(x, y, pack(((c >> 16) & 255), (c & 255), ((c >> 8) & 255), a));
}

void ClearScreen(int c)
{
	for (int i = 0; i < WINDOW_HEIGHT * WINDOW_WIDTH; i++) pixels[i] = c;
}