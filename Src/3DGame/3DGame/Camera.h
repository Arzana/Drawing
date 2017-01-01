#pragma once

#include <Mtrx4.h>

typedef struct Camera
{
public:
	Camera(void);
	Camera(const vect3 pos);
	~Camera(void);

	void SetYaw(float rads);
	void SetPitch(float rads);
	void SetRoll(float rads);
	void SetRotation(vect3 rads);
	inline void SetYawDegr(float degr) { SetYaw(degr * M_DEG2RAD); }
	inline void SetPitchDegr(float degr) { SetPitch(degr * M_DEG2RAD); }
	inline void SetRollDegr(float degr) { SetRoll(degr * M_DEG2RAD); }
	inline void SetRotationDegr(vect3 degr) { SetRotation(degr * M_DEG2RAD); }

	void AppendYaw(float rads);
	void AppendPitch(float rads);
	void AppendRoll(float rads);
	void AppendRotation(vect3 rads);
	inline void AppendYawDegr(float degr) { AppendYaw(degr * M_DEG2RAD); }
	inline void AppendPitchDegr(float degr) { AppendPitch(degr * M_DEG2RAD); }
	inline void AppendRollDegr(float degr) { AppendRoll(degr * M_DEG2RAD); }
	inline void AppendRotationDegr(vect3 degr) { AppendRotation(degr * M_DEG2RAD); }

	inline float GetYaw(void) const { return rotation->X; }
	inline float GetPitch(void) const { return rotation->Y; }
	inline float GetRoll(void) const { return rotation->Z; }
	inline vect3 GetRotation(void) const { return *rotation; }

	inline void SetX(float x) { position->X = x; }
	inline void SetY(float y) { position->Y = y; }
	inline void SetZ(float z) { position->Z = z; }
	inline void SetPosition(vect3 pos) { *position = pos; }

	inline void AppendX(float x) { position->X += x; }
	inline void AppendY(float y) { position->Y += y; }
	inline void AppendZ(float z) { position->Z += z; }
	inline void AppendPosition(vect3 pos) { *position += pos; }

	inline float GetX(void) const { return position->X; }
	inline float GetY(void) const { return position->Y; }
	inline float GetZ(void) const { return position->Z; }
	inline vect3 GetPosition(void) const { return *position; }

	void Move(vect3 direction);

	const mtrx4* Update(void);
	inline const mtrx4* GetView(void) const { return view; }

private:
	vect3 *position, *rotation;
	mtrx4 *view;

	float GetClampedRads(float rads);
} cam;