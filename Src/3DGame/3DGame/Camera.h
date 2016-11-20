#pragma once

#include <Mtrx4.h>

class Camera
{
public:
	Camera(void);
	Camera(const Vect3 pos);
	~Camera(void);

	void SetYawRads(float rads);
	void SetYawDegr(float degr);
	void SetPitchRads(float rads);
	void SetPitchDegr(float degr);
	void SetRollRads(float rads);
	void SetRollDegr(float degr);
	void SetRotationRads(Vect3 rads);
	void SetRotationDegr(Vect3 degr);
	void AppendYawRads(float rads);
	void AppendYawDegr(float degr);
	void AppendPitchRads(float rads);
	void AppendPitchDegr(float degr);
	void AppendRollRads(float rads);
	void AppendRollDegr(float degr);
	void AppendRotationRads(Vect3 rads);
	void AppendRotationDegr(Vect3 degr);
	float GetYaw(void) const;
	float GetPitch(void) const;
	float GetRoll(void) const;
	Vect3 GetRotation(void) const;

	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetPosition(Vect3 pos);
	void AppendX(float x);
	void AppendY(float y);
	void AppendZ(float z);
	void AppendPosition(Vect3 pos);
	float GetX(void) const;
	float GetY(void) const;
	float GetZ(void) const;
	Vect3 GetPosition(void) const;

	void Move(Vect3 direction);

	void Update(void);
	const Mtrx4* GetView(void) const;

private:
	Vect3 *position, *rotation;
	Quat *quat;
	Mtrx4 *view;

	float GetClampedRads(float rads);
};