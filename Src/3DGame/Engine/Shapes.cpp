#include "Shapes.h"
#include <cstring>

typedef poly::clrvect4 vrtx;

void CopyVertices(vrtx *from, poly *to, octet numVertices)
{
	to->vrtxCount = numVertices;
	memcpy(to->vertexes, from, sizeof(vrtx) * numVertices);
}

void ClipPolyOnWAxis(poly *face)
{
	octet vrtxCount(0);
	vrtx result[MAX_VRTX_POLY];

	vrtx *prevVrtx = face->Last();
	vrtx *curVrtx = face->First();
	bool prevDot = prevVrtx->v.W >= W_CLIPPING_PLANE;

	while (curVrtx != face->End())
	{
		bool curDot = curVrtx->v.W >= W_CLIPPING_PLANE;
		if (prevDot != curDot)	// W clipping needed
		{
			float a = lerpclamp((W_CLIPPING_PLANE - prevVrtx->v.W) / (prevVrtx->v.W - curVrtx->v.W));
			result[vrtxCount++] = vrtx::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot) result[vrtxCount++] = *curVrtx;

		prevDot = curDot;
		prevVrtx = curVrtx++;
	}

	CopyVertices(result, face, vrtxCount);
}

void ClipPolyOnAxis(poly *face, int axis)
{
	octet vrtxCount(0);
	vrtx result[MAX_VRTX_POLY];

	// Clip against first plane
	vrtx *prevVrtx = face->Last();
	vrtx *curVrtx = face->First();
	bool prevDot = prevVrtx->v[axis] <= prevVrtx->v.W;

	while (curVrtx != face->End())
	{
		bool curDot = curVrtx->v[axis] <= curVrtx->v.W;
		if (prevDot != curDot)	// Clip needed against plan W=0
		{
			float a = lerpclamp((prevVrtx->v.W - prevVrtx->v[axis]) / ((prevVrtx->v.W - prevVrtx->v[axis]) - (curVrtx->v.W - curVrtx->v[axis])));
			result[vrtxCount++] = vrtx::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot) result[vrtxCount++] = *curVrtx;

		prevDot = curDot;
		prevVrtx = curVrtx++;
	}

	CopyVertices(result, face, vrtxCount);
	vrtxCount = 0;

	// Clip against opposite plane
	prevVrtx = face->Last();
	prevDot = -prevVrtx->v[axis] <= prevVrtx->v.W;
	curVrtx = face->First();
	while (curVrtx != face->End())
	{
		bool curDot = -curVrtx->v[axis] <= curVrtx->v.W;
		if (prevDot != curDot)
		{
			float a = lerpclamp((prevVrtx->v.W + prevVrtx->v[axis]) / ((prevVrtx->v.W + prevVrtx->v[axis]) - (curVrtx->v.W + curVrtx->v[axis])));
			result[vrtxCount++] = vrtx::Lerp(prevVrtx, curVrtx, a);
		}

		if (curDot) result[vrtxCount++] = *curVrtx;

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