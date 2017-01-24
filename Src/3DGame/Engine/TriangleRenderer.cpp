#include "TriangleRenderer.h"

TriangleRenderer::TriangleRenderer(winGame * game)
	: game(game), buff(new std::vector<vrtx>())
{
	game->AddComponent(this);
}

TriangleRenderer::~TriangleRenderer(void)
{
	delete_s(buff);
	game = NULL;
}

void TriangleRenderer::Add(trgl trgl)
{
	buff->push_back(trgl.v0);
	buff->push_back(trgl.v1);
	buff->push_back(trgl.v2);
}

void TriangleRenderer::Add(const geon *geon)
{
	for (size_t i = 0; i < geon->trglCount; i++)
	{
		Add(geon->GetTriangles()[i]);
	}
}

void TriangleRenderer::Draw(void)
{
	if (buff->size() > 0)
	{
		game->Start(GF_TRIANGLES);
		game->SetBufferLength(buff->size());

		for (size_t i = 0; i < buff->size(); i++)
		{
			vrtx cur = buff->at(i);
			game->AddVertex(cur.v, cur.c);
		}

		if (!game->End()) game->Terminate();
	}
}
