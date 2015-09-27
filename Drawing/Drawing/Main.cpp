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
		ThrowFatalError(SDL_GetError());
		exit(EXIT_FAILURE);
	}

	window = SDL_CreateWindow("Drawing_Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	surface = SDL_GetWindowSurface(window);
}

void InitMap()
{
	map = new Map();
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		ThrowFatalError("Could not get current directory.");
		exit(EXIT_FAILURE);
	}

	strcat_s(cCurrentPath, "\\map.txt");

	if (!map->Load(cCurrentPath))
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
	const int VerticesInTriangle = 3;
	int mapLength = map->GetLength();
	int vertexLength = VerticesInTriangle * mapLength;
	Vector3 *vertices = (Vector3*)malloc(sizeof(Vector3) * vertexLength);

	int vertexIndex = 0;
	for (int sectorIndex = 0; sectorIndex < mapLength; sectorIndex++)
	{
		Sector *curSector = map->GetSector(sectorIndex);
		const Vector3 **sectorVertices = curSector->GetVertices();

		for (int i = 0; i < VerticesInTriangle; i++) vertices[vertexIndex++] = Vector3(sectorVertices[i]->X, curSector->GetCeiling(), sectorVertices[i]->Z);

		delete sectorVertices;
	}

	Vector3 targ = *player + rotation->GetForward();
	Matrix modelM = MTRX_Identity;
	Matrix viewM = Matrix::View(*player, targ, rotation->GetUp());
	Matrix projM = Matrix::Perspective(60, ASPR, PLANE_NEAR, PLANE_FAR);
	Matrix mvp = modelM * viewM * projM;

	Matrix::Transform(&mvp, vertices, vertexLength);

	// Start rendering
	pixels = (int*)surface->pixels;
	ClearScreen(0x000000);
	for (int i = 0; i < vertexLength; i += VerticesInTriangle)
	{
		Triangle current = Triangle(vertices[i], vertices[i + 1], vertices[i + 2]);
		if (Triangle::CheckVisibility(&current, 1)) RenderTriangle(current.A.X, current.A.Y, current.B.X, current.B.Y, current.C.X, current.C.Y);
	}

	// End rendering
	SDL_UpdateWindowSurface(window);
	pixels = NULL;
	free(vertices);
}

void RenderTriangle(float x0, float y0, float x1, float y1, float x2, float y2)
{
	const int c = 0xFF00FF;
	float x00 = x0, x01 = x0, y00 = y0, y01 = y0;

	bool steepAB = abs(y1 - y00) > abs(x1 - x00);
	bool steepAC = abs(y2 - y01) > abs(x2 - x01);

	if (steepAB)
	{
		swap(x00, y00);
		swap(x1, y1);
	}

	if (steepAC)
	{
		swap(x01, y01);
		swap(x2, y2);
	}

	if (x00 > x1)
	{
		swap(x00, x1);
		swap(y00, y1);
	}

	if (x01 > x2)
	{
		swap(x01, x2);
		swap(y01, y2);
	}

	float delta_XAB = x1 - x00, delta_XAC = x2 - x01;
	float deltaYAB = y1 - y00, deltaYAC = y2 - y01;
	int signAB = deltaYAB == 0 ? 0 : (deltaYAB > 0 ? 1 : -1), signAC = deltaYAC == 0 ? 0 : (deltaYAC > 0 ? 1 : -1);
	float gradientAB = abs(deltaYAB / delta_XAB), gradientAC = abs(deltaYAC / delta_XAC);

	int errorAB = 0, errorAC = 0;
	for (int aX = x00, bX = x01, aY = y00, bY = y01; aX <= (steepAB ? y1 : x1) && bX <= (steepAC ? y2 : x2);)
	{
		if (steepAB && steepAC) BresenhamLine(aY, aX, bY, bX);
		else if (steepAB) BresenhamLine(aY, aX, bX, bY);
		else if (steepAC) BresenhamLine(aX, aY, bY, bX);

		errorAB += gradientAB, errorAC += gradientAC;

		while (errorAB > 0.5f)
		{
			aY += signAB;
			errorAB -= 1;
		}

		while (errorAC > 0.5f)
		{
			bY += signAC;
			errorAC -= 1;
		}

		if (aX <= steepAB ? y1 : x1) aX++;
		if (bX <= steepAC ? y2 : x2) bX++;
	}
}

void BresenhamLine(float x0, float y0, float x1, float y1)
{
	const int c = 0xFF00FF;
	bool steep = abs(y1 - y0) > abs(x1 - x0);

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

void WuLine(Line *l)
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
	xEnd = round(l->B.X);
	yEnd = l->B.Y + gradient * (xEnd - l->B.X);
	xGap = fpart(l->B.X + 0.5);
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
	if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT) pixels[y * ipart(WINDOW_WIDTH) + x] = c;
}

void PlotA(int x, int y, float a)
{
	Plot(x, y, pack(255, 0, 255, a));
}

void ClearScreen(int c)
{
	for (int i = 0; i < WINDOW_HEIGHT * WINDOW_WIDTH; i++) pixels[i] = c;
}