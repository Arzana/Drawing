#include "Shapes.h"

void CopyVertices(poly::clrvect4 *from, poly *to, octet numVertices)
{
	to->vrtxCount = numVertices;
	for (octet i = 0; i < numVertices; i++)
	{
		to->vertexes[i] = poly::clrvect4(from[i]);
	}
}

void ClipPolyOnWAxis(poly *face)
{
	octet vrtxCount = 0;
	poly::clrvect4 result[MAX_VRTX_PRE_POLY];

	poly::clrvect4 *prevVrtx = &face->vertexes[face->vrtxCount - 1];
	poly::clrvect4 *curVrtx = face->First();
	float prevDot = prevVrtx->v.W < W_CLIPPING_PLANE ? -1 : 1;

	while (curVrtx != face->Last())
	{
		float curDot = curVrtx->v.W < W_CLIPPING_PLANE ? -1 : 1;
		if (prevDot * curDot < 0)	// W clipping needed
		{
			float a = (W_CLIPPING_PLANE - prevVrtx->v.W) / (prevVrtx->v.W - curVrtx->v.W);
			result[vrtxCount++] = poly::clrvect4::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot > 0) result[vrtxCount++] = *curVrtx;

		prevDot = curDot;
		prevVrtx = curVrtx++;
	}

	CopyVertices(result, face, vrtxCount);
}

void ClipPolyOnAxis(poly *face, int axis)
{
	octet vrtxCount = 0;
	poly::clrvect4 result[MAX_VRTX_PRE_POLY];

	// Clip against first plane
	poly::clrvect4 *prevVrtx = &face->vertexes[face->vrtxCount - 1];
	poly::clrvect4 *curVrtx = face->First();
	float prevDot = prevVrtx->v[axis] <= prevVrtx->v.W ? 1 : -1;

	while (curVrtx != face->Last())
	{
		float curDot = curVrtx->v[axis] <= curVrtx->v.W ? 1 : -1;
		if (prevDot * curDot < 0)	// Clip needed against plan W=0
		{
			float a = (prevVrtx->v.W - prevVrtx->v[axis]) / ((prevVrtx->v.W - prevVrtx->v[axis]) - (curVrtx->v.W - curVrtx->v[axis]));
			result[vrtxCount++] = poly::clrvect4::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot > 0) result[vrtxCount++] = *curVrtx;

		prevDot = curDot;
		prevVrtx = curVrtx++;
	}

	CopyVertices(result, face, vrtxCount);
	vrtxCount = 0;

	// Clip against opposite plane
	prevVrtx = &face->vertexes[face->vrtxCount - 1];
	prevDot = -prevVrtx->v[axis] <= prevVrtx->v.W ? 1 : -1;
	curVrtx = face->First();
	while (curVrtx != face->Last())
	{
		float curDot = -curVrtx->v[axis] <= curVrtx->v.W ? 1 : -1;
		if (prevDot * curDot < 0)
		{
			float a = (prevVrtx->v.W + prevVrtx->v[axis]) / ((prevVrtx->v.W + prevVrtx->v[axis]) - (curVrtx->v.W + curVrtx->v[axis]));
			result[vrtxCount++] = poly::clrvect4::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot > 0) result[vrtxCount++] = *curVrtx;

		prevDot = curDot;
		prevVrtx = curVrtx++;
	}

	CopyVertices(result, face, vrtxCount);
}

void ClipPoly(poly *face)
{
	ClipPolyOnWAxis(face);
	ClipPolyOnAxis(face, 0);
	ClipPolyOnAxis(face, 1);
	ClipPolyOnAxis(face, 2);
}