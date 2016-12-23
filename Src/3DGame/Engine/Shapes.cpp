#include "Shapes.h"

#define SIMPL_WDOT(x)	((x) < W_CLIPPING_PLANE ? -1 : 1)
typedef poly::clrvect4 vrtx;

void CopyVertices(vrtx *from, poly *to, octet numVertices)
{
	to->vrtxCount = numVertices;
	for (octet i = 0; i < numVertices; i++)
	{
		to->vertexes[i] = vrtx(from[i]);
	}
}

void ClipPolyOnWAxis(poly *face)
{
	octet vrtxCount(0);
	vrtx result[MAX_VRTX_PRE_POLY];

	vrtx *prevVrtx = face->Last();
	vrtx *curVrtx = face->First();
	char prevDot = SIMPL_WDOT(prevVrtx->v.W);

	while (curVrtx != face->End())
	{
		char curDot = SIMPL_WDOT(curVrtx->v.W);
		if (prevDot * curDot < 0)	// W clipping needed
		{
			float a = (W_CLIPPING_PLANE - prevVrtx->v.W) / (prevVrtx->v.W - curVrtx->v.W);
			result[vrtxCount++] = vrtx::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot > 0) result[vrtxCount++] = *curVrtx;

		prevDot = curDot;
		prevVrtx = curVrtx++;
	}

	CopyVertices(result, face, vrtxCount);
}

void ClipPolyOnAxis(poly *face, int axis)
{
	octet vrtxCount(0);
	vrtx result[MAX_VRTX_PRE_POLY];

	// Clip against first plane
	vrtx *prevVrtx = face->Last();
	vrtx *curVrtx = face->First();
	char prevDot = prevVrtx->v[axis] <= prevVrtx->v.W ? 1 : -1;

	while (curVrtx != face->End())
	{
		char curDot = curVrtx->v[axis] <= curVrtx->v.W ? 1 : -1;
		if (prevDot * curDot < 0)	// Clip needed against plan W=0
		{
			float a = (prevVrtx->v.W - prevVrtx->v[axis]) / ((prevVrtx->v.W - prevVrtx->v[axis]) - (curVrtx->v.W - curVrtx->v[axis]));
			result[vrtxCount++] = vrtx::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot > 0) result[vrtxCount++] = *curVrtx;

		prevDot = curDot;
		prevVrtx = curVrtx++;
	}

	CopyVertices(result, face, vrtxCount);
	vrtxCount = 0;

	// Clip against opposite plane
	prevVrtx = face->Last();
	prevDot = -prevVrtx->v[axis] <= prevVrtx->v.W ? 1 : -1;
	curVrtx = face->First();
	while (curVrtx != face->End())
	{
		char curDot = -curVrtx->v[axis] <= curVrtx->v.W ? 1 : -1;
		if (prevDot * curDot < 0)
		{
			float a = (prevVrtx->v.W + prevVrtx->v[axis]) / ((prevVrtx->v.W + prevVrtx->v[axis]) - (curVrtx->v.W + curVrtx->v[axis]));
			result[vrtxCount++] = vrtx::Lerp(prevVrtx, curVrtx, a);
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