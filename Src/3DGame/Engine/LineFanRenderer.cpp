#include "LineFanRenderer.h"

LineFanRenderer::LineFanRenderer(winGame * game)
	: game(game), buff(new std::vector<vrtx>())
{
	game->AddComponent(this);
}

LineFanRenderer::LineFanRenderer(void)
{
	delete_s(buff);
	game = NULL;
}

void LineFanRenderer::SetCenter(vrtx v)
{
	buff->clear();
	buff->push_back(v);
}

void LineFanRenderer::Add(vrtx v)
{
	buff->push_back(v);
}

void LineFanRenderer::Draw(void)
{
	if (buff->size() > 0)
	{
		game->Start(GF_LINE_FAN);
		game->SetBufferLength(buff->size());

		for (size_t i = 0; i < buff->size(); i++)
		{
			vrtx cur = buff->at(i);
			game->AddVertex(cur.v, cur.c);
		}

		if (!game->End()) game->Terminate();
	}
}
