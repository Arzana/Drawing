#define _VECT_CONV

#include "Camera.h"
#include <MathEx.h>
#include <Utils.h>

Camera::Camera(void)
{
	position = new VECT3_ZERO;
	rotation = new VECT3_ZERO;
	view = new MTRX4_IDENTITY;
}

Camera::Camera(const vect3 pos)
{ 
	position = new vect3(pos);
	rotation = new VECT3_ZERO;
	view = new MTRX4_IDENTITY;
}

Camera::~Camera(void)
{
	delete position;
	delete rotation;
	delete view;
}

void Camera::SetYaw(float rads)
{
	rotation->X = GetClampedRads(rads);
}

void Camera::SetPitch(float rads)
{
	rotation->Y = GetClampedRads(rads);
}

void Camera::SetRoll(float rads)
{
	rotation->Z = GetClampedRads(rads);
}

void Camera::SetRotation(vect3 rads)
{
	SetYaw(rads.X);
	SetPitch(rads.Y);
	SetRoll(rads.Z);
}

void Camera::AppendYaw(float rads)
{
	SetYaw(rotation->X + rads);
}

void Camera::AppendPitch(float rads)
{
	SetPitch(rotation->Y + rads);
}

void Camera::AppendRoll(float rads)
{
	SetRoll(rotation->Z + rads);
}

void Camera::AppendRotation(vect3 rads)
{
	SetRotation(*rotation + rads);
}

void Camera::Move(vect3 direction)
{
	if (direction == VECT3_ZERO) return;
	vect4 camDir = mtrx4::CreateRotationQ(rotation->X, 0, rotation->Z) * direction;
	camDir.ToNDC();
	AppendPosition(V4ToV3(camDir));
}

const mtrx4 * Camera::Update(void)
{
	return &(*view = mtrx4::CreateView(position, rotation));
}

float Camera::GetClampedRads(float rads)
{
	rads = fmodf(rads, M_TAU);
	if (rads < 0) rads += M_TAU;
	return rads;
}
