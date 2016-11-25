#pragma once

#include <Mtrx4.h>

typedef struct Camera
{
public:
	Camera(void);
	Camera(const vect3 pos);
	~Camera(void);

	void SetYawRads(float rads);
	void SetYawDegr(float degr);
	void SetPitchRads(float rads);
	void SetPitchDegr(float degr);
	void SetRollRads(float rads);
	void SetRollDegr(float degr);
	void SetRotationRads(vect3 rads);
	void SetRotationDegr(vect3 degr);
	void AppendYawRads(float rads);
	void AppendYawDegr(float degr);
	void AppendPitchRads(float rads);
	void AppendPitchDegr(float degr);
	void AppendRollRads(float rads);
	void AppendRollDegr(float degr);
	void AppendRotationRads(vect3 rads);
	void AppendRotationDegr(vect3 degr);
	float GetYaw(void) const;
	float GetPitch(void) const;
	float GetRoll(void) const;
	vect3 GetRotation(void) const;

	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetPosition(vect3 pos);
	void AppendX(float x);
	void AppendY(float y);
	void AppendZ(float z);
	void AppendPosition(vect3 pos);
	float GetX(void) const;
	float GetY(void) const;
	float GetZ(void) const;
	vect3 GetPosition(void) const;

	void Move(vect3 direction);

	void Update(void);
	const mtrx4* GetView(void) const;

private:
	vect3 *position, *rotation;
	quat *rot;
	mtrx4 *view;

	float GetClampedRads(float rads);
} cam;