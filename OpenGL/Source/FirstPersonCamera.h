#ifndef FirstPersonCamera_H
#define FirstPersonCamera_H

#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

class FirstPersonCamera :public Camera
{
public:

	Vector3 Position;
	Vector3 Target;
	Vector3 Up;
	
	FirstPersonCamera();
	~FirstPersonCamera();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
};

#endif