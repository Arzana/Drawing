#include "Shapes.h"

void CopyVertices(vect4 *from, poly *to, octet numVertices)
{
	to->vrtxCount = numVertices;
	for (octet i = 0; i < numVertices; i++)
	{
		to->vertices[i] = vect4(from[i]);
	}
}

void ClipPolyOnWAxis(poly *face)
{
	octet vrtxCount = 0;
	vect4 result[MAX_VRTX_PRE_POLY];

	vect4 *prevVrtx = &face->vertices[face->vrtxCount - 1];
	vect4 *curVrtx = face->First();
	float prevDot = prevVrtx->W < W_CLIPPING_PLANE ? -1 : 1;

	while (curVrtx != face->Last())
	{
		float curDot = curVrtx->W < W_CLIPPING_PLANE ? -1 : 1;
		if (prevDot * curDot < 0)	// W clipping needed
		{
			float a = (W_CLIPPING_PLANE - prevVrtx->W) / (prevVrtx->W - curVrtx->W);
			result[vrtxCount++] = vect4::Lerp(prevVrtx, curVrtx, a);
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
	vect4 result[MAX_VRTX_PRE_POLY];

	// Clip against first plane
	vect4 *prevVrtx = &face->vertices[face->vrtxCount - 1];
	vect4 *curVrtx = face->First();
	float prevDot = (*prevVrtx)[axis] <= prevVrtx->W ? 1 : -1;

	while (curVrtx != face->Last())
	{
		float curDot = (*curVrtx)[axis] <= curVrtx->W ? 1 : -1;
		if (prevDot * curDot < 0)	// Clip needed against plan W=0
		{
			float a = (prevVrtx->W - (*prevVrtx)[axis]) / ((prevVrtx->W - (*prevVrtx)[axis]) - (curVrtx->W - (*curVrtx)[axis]));
			result[vrtxCount++] = vect4::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot > 0) result[vrtxCount++] = *curVrtx;

		prevDot = curDot;
		prevVrtx = curVrtx++;
	}

	CopyVertices(result, face, vrtxCount);
	vrtxCount = 0;

	// Clip against opposite plane
	prevVrtx = &face->vertices[face->vrtxCount - 1];
	prevDot = -(*prevVrtx)[axis] <= prevVrtx->W ? 1 : -1;
	curVrtx = face->First();
	while (curVrtx != face->Last())
	{
		float curDot = -(*curVrtx)[axis] <= curVrtx->W ? 1 : -1;
		if (prevDot * curDot < 0)
		{
			float a = (prevVrtx->W + (*prevVrtx)[axis]) / ((prevVrtx->W + (*prevVrtx)[axis]) - (curVrtx->W + (*curVrtx)[axis]));
			result[vrtxCount++] = vect4::Lerp(prevVrtx, curVrtx, a);
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