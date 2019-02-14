#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera()
{

}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = Position = pos;
	this->target = Target = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = Up = right.Cross(view).Normalized();
}

void FirstPersonCamera::Update(double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	static const float CAMERA_SPEED = 60.f;
	Mtx44 rotation;

}