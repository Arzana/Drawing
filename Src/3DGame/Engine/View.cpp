#define _VECT_CONV

#include "View.h"
#include "MathEx.h"
#include "Utils.h"

View::View(void)
{
	position = new VECT3_ZERO;
	rotation = new VECT3_ZERO;
	view = new MTRX4_IDENTITY;
}

View::View(const vect3 pos)
{ 
	position = new vect3(pos);
	rotation = new VECT3_ZERO;
	view = new MTRX4_IDENTITY;
}

View::~View(void)
{
	delete position;
	delete rotation;
	delete view;
}

void View::SetYaw(float rads)
{
	rotation->X = GetClampedRads(rads);
}

void View::SetPitch(float rads)
{
	rotation->Y = GetClampedRads(rads);
}

void View::SetRoll(float rads)
{
	rotation->Z = GetClampedRads(rads);
}

void View::SetRotation(vect3 rads)
{
	SetYaw(rads.X);
	SetPitch(rads.Y);
	SetRoll(rads.Z);
}

void View::AppendYaw(float rads)
{
	SetYaw(rotation->X + rads);
}

void View::AppendPitch(float rads)
{
	SetPitch(rotation->Y + rads);
}

void View::AppendRoll(float rads)
{
	SetRoll(rotation->Z + rads);
}

void View::AppendRotation(vect3 rads)
{
	SetRotation(*rotation + rads);
}

void View::Move(vect3 direction)
{
	if (direction == VECT3_ZERO) return;
	AppendPosition(vect4::ToNDC(mtrx4::CreateRotationQ(rotation->X, 0, rotation->Z) * direction));
}

const mtrx4 * View::Update(void)
{
	return &(*view = mtrx4::CreateView(position, rotation));
}

float View::GetClampedRads(float rads)
{
	rads = fmodf(rads, M_TAU);
	if (rads < 0) rads += M_TAU;
	return rads;
}
