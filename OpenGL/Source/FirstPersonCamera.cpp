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

	/*if (target.x > -480.f && target.x < 480.f && target.y > 2.f && target.y < 480.f && target.z > -480.f && target.z < 480.f)
	{
		if (Application::IsKeyPressed('A'))
		{
			position = position - right;
		}
		if (Application::IsKeyPressed('D'))
		{
			position = position + right;
		}
		if (Application::IsKeyPressed('W'))
		{
			position = position + up;
		}
		if (Application::IsKeyPressed('S'))
		{
			position = position - up;
		}

		if (Application::IsKeyPressed('N'))
		{
			position = position + view.Normalized();
		}
		if (Application::IsKeyPressed('M'))
		{
			position = position - view.Normalized();
		}
	}
	else
	{
		if (target.x >= 480.f)
		{
			position.x = position.x - 0.1;
		}
		else
		{
			position.x = position.x + 0.1;
		}

		if (target.y >= 480.f)
		{
			position.y = position.y - 0.1;
		}
		if (target.y <= 2.f)
		{
			position.y = position.y + 0.1;
		}

		if (target.z >= 480.f)
		{
			position.z = position.z - 0.1;
		}
		else
		{
			position.z = position.z + 0.1;
		}
	}
	target = position + view;*/

	/*if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		rotation.SetToRotation(yaw, 0, 1, 0);
		up = rotation * up;
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		rotation.SetToRotation(yaw, 0, 1, 0);
		up = rotation * up;
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		right = rotation * right;
		view = rotation * view;
		target = position + view;
	}
	else if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		right = rotation * right;
		view = rotation * view;
		target = position + view;
	}*/
}