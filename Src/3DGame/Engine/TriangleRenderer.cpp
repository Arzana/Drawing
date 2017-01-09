#include "TriangleRenderer.h"

TriangleRenderer::TriangleRenderer(winGame * game, size_t length)
	: game(game), len(length * 3), i(0)
{
	if (length < 1) throw std::invalid_argument("length must be greater than zero!");

	hBuff = malloc_s(vect4, len);
	cBuff = malloc_s(clr, len);

	game->AddComponent(this);
}

TriangleRenderer::~TriangleRenderer(void)
{
	free_s(hBuff);
	free_s(cBuff);
	game = NULL;
}

void TriangleRenderer::Add(trgl trgl)
{
	if (i + 3 > len) throw std::bad_function_call();

	hBuff[i] = trgl.v0.v;
	cBuff[i++] = trgl.v0.c;

	hBuff[i] = trgl.v1.v;
	cBuff[i++] = trgl.v1.c;

	hBuff[i] = trgl.v2.v;
	cBuff[i++] = trgl.v2.c;
}

void TriangleRenderer::Draw(void)
{
	game->Clear(CLR_BLACK);
	game->Start(GF_TRIANGLES);
	game->SetBufferLength(len);

	for (size_t i = 0; i < len; i++)
	{
		game->AddVertex(hBuff[i], cBuff[i]);
	}

	if (!game->End()) game->Terminate();
}
