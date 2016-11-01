#include "Camera.h"
#include <MathEx.h>

Camera::Camera(void)
{
	position = new Vect3();
	rotation = new Vect3();
	view = new MTRX4_IDENTITY;
}

Camera::Camera(const Vect3 pos)
{ 
	position = new Vect3(pos);
	rotation = new Vect3();
	view = new MTRX4_IDENTITY;
}

Camera::~Camera(void)
{
	delete position;
	delete rotation;
	delete view;
}

void Camera::SetYawRads(float rads)
{
	rotation->X = GetClampedRads(rads);
}

void Camera::SetYawDegr(float degr)
{
	SetYawRads(degr * deg2rad);
}

void Camera::SetPitchRads(float rads)
{
	rotation->Y = GetClampedRads(rads);
}

void Camera::SetPitchDegr(float degr)
{
	SetPitchRads(degr * deg2rad);
}

void Camera::SetRollRads(float rads)
{
	rotation->Z = GetClampedRads(rads);
}

void Camera::SetRollDegr(float degr)
{
	SetRollRads(degr * deg2rad);
}

void Camera::SetRotationRads(Vect3 rads)
{
	rotation->X = GetClampedRads(rads.X);
	rotation->Y = GetClampedRads(rads.Y);
	rotation->Z = GetClampedRads(rads.Z);
}

void Camera::SetRotationDegr(Vect3 degr)
{
	SetRotationRads(degr * deg2rad);
}

void Camera::AppendYawRads(float rads)
{
	SetYawRads(rotation->X + rads);
}

void Camera::AppendYawDegr(float degr)
{
	AppendYawRads(degr * deg2rad);
}

void Camera::AppendPitchRads(float rads)
{
	SetPitchRads(rotation->Y + rads);
}

void Camera::AppendPitchDegr(float degr)
{
	AppendPitchRads(degr * deg2rad);
}

void Camera::AppendRollRads(float rads)
{
	SetRollRads(rotation->Z + rads);
}

void Camera::AppendRollDegr(float degr)
{
	AppendRollDegr(degr * deg2rad);
}

void Camera::AppendRotationRads(Vect3 rads)
{
	SetRotationRads(*rotation + rads);
}

void Camera::AppendRotationDegr(Vect3 degr)
{
	AppendRotationRads(degr * deg2rad);
}

float Camera::GetYaw(void) const
{
	return rotation->X;
}

float Camera::GetPitch(void) const
{
	return rotation->Y;
}

float Camera::GetRoll(void) const
{
	return rotation->Z;
}

Vect3 Camera::GetRotation(void) const
{
	return *rotation;
}

void Camera::SetX(float x)
{
	position->X = x;
}

void Camera::SetY(float y)
{
	position->Y = y;
}

void Camera::SetZ(float z)
{
	position->Z = z;
}

void Camera::SetPosition(Vect3 pos)
{
	*position = pos;
}

void Camera::AppendX(float x)
{
	position->X += x;
}

void Camera::AppendY(float y)
{
	position->Y += y;
}

void Camera::AppendZ(float z)
{
	position->Z += z;
}

void Camera::AppendPosition(Vect3 pos)
{
	*position += pos;
}

float Camera::GetX(void) const
{
	return position->X;
}

float Camera::GetY(void) const
{
	return position->Y;
}

float Camera::GetZ(void) const
{
	return position->Z;
}

Vect3 Camera::GetPosition(void) const
{
	return *position;
}

void Camera::Move(Vect3 direction)
{
	Mtrx4 movement = Mtrx4::CreateYawPitchRoll(rotation->X, rotation->Y, rotation->Z);
	Vect4 vH = movement * direction;

	AppendX(vH.X);
	AppendY(vH.Y);
	AppendZ(vH.Z);
}

void Camera::Update(void)
{
	*view = Mtrx4::CreateYawPitchRoll(-rotation->X, -rotation->Y, -rotation->Z);
	*view *= Mtrx4::CreateTranslation(&Vect3::Negate(position));
}

const Mtrx4 * Camera::GetView(void) const
{
	return view;
}

float Camera::GetClampedRads(float rads)
{
	rads = fmodf(rads, M_TAU);
	if (rads < 0) rads += M_TAU;
	return rads;
}
