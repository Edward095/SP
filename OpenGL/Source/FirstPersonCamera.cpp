#include "FirstPersonCamera.h"
#include "Application.h"


FirstPersonCamera::FirstPersonCamera()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Init(const Vector3& pos)
{
	firstMouse = true;
	lastX = 400.0f;
	lastY = 300.0f;
	yaw = -89.0f;//rotate abt y
	pitch = 0.0f;//rotate abt x
	sensitivity = 0.08f;

	front = Vector3(0.0f, 0.0f, 1.0f);
	up = Vector3(0.0f, 1.0f, 0.0f);
	this->position = pos;

	UpdateMouse();
}

void FirstPersonCamera::UpdateMouse()
{
	if (firstMouse)
	{
		lastX = Application::mouse_x;
		lastY = Application::mouse_y;
		firstMouse = false;
	}

	double xoffset = (Application::mouse_x - lastX);

	double yoffset = (lastY - Application::mouse_y);
	lastX = Application::mouse_x;
	lastY = Application::mouse_y;


	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += (float)xoffset;
	pitch += (float)yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	float pitchRad = Math::DegreeToRadian(pitch);
	float yawRad = Math::DegreeToRadian(yaw);
	front.x = cos(pitchRad) * cos(yawRad);
	front.y = sin(pitchRad);
	front.z = cos(pitchRad) * sin(yawRad);
	front.Normalize();

	this->target = position + front;

}

void FirstPersonCamera::Update(double dt)
{
	elapsedTime += dt;
	
	//if (position.z > 5 && position.z < 9)//Shooting Range
	//	WBmove(dt, 3, 9, 5);
	//else
	//	WBmove(dt, 3, 18, 12);//gun Room
	WOBmove(dt);

	UpdateMouse();

}

Mtx44 FirstPersonCamera::LookAt()
{
	Vector3 f = front.Normalize();
	Vector3 s = f.Cross(up).Normalize();
	Vector3 u = s.Cross(f);

	Mtx44 mat(s.x, u.x, -f.x, 0,
		s.y, u.y, -f.y, 0,
		s.z, u.z, -f.z, 0,
		-s.Dot(target), -u.Dot(target), f.Dot(target), 1);

	return mat;
}

void FirstPersonCamera::Invert()
{
	pitch = -pitch;
}

bool FirstPersonCamera::checkCollisions(float x, float zUpper, float zLower)
{
	return (position.x > x || position.x < -x ||
		position.z >zUpper || position.z < zLower);
}

void FirstPersonCamera::WBmove(double dt, float x, float zUpper, float zLower)
{
	static const float FirstPersonCamera_SPEED = 50.f;

	Vector3 right = front.Cross(up).Normalize();

	if (Application::IsKeyPressed('D'))
	{
		position += right * FirstPersonCamera_SPEED * dt;
		if (checkCollisions(x,zUpper,zLower))
		{
			position -= right * FirstPersonCamera_SPEED * dt;
		}
	}
	if (Application::IsKeyPressed('W'))
	{
		position += front * FirstPersonCamera_SPEED * dt;
		if (checkCollisions(x, zUpper, zLower))
		{
			position -= front * FirstPersonCamera_SPEED * dt;
		}
	}
	if (Application::IsKeyPressed('A'))
	{
		position -= right * FirstPersonCamera_SPEED * dt;
		if (checkCollisions(x, zUpper, zLower))
		{
			position += right * FirstPersonCamera_SPEED * dt;
		}
	}
	if (Application::IsKeyPressed('S'))
	{
		position -= front * FirstPersonCamera_SPEED * dt;
		if (checkCollisions(x, zUpper, zLower))
		{
			position += front * FirstPersonCamera_SPEED * dt;
		}
	}
}
void FirstPersonCamera::WOBmove(double dt)
{
	static const float FirstPersonCamera_SPEED = 10.f;

	Vector3 right = front.Cross(up).Normalize();

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		position += right * FirstPersonCamera_SPEED * dt;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		position += front * FirstPersonCamera_SPEED * dt;
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		position -= right * FirstPersonCamera_SPEED * dt;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		position -= front * FirstPersonCamera_SPEED * dt;
	}
	if (Application::IsKeyPressed('Q'))
	{
		position += Vector3(0, 1, 0) * 0.75f;
	}
	if (Application::IsKeyPressed('E'))
	{
		position += Vector3(0, -1, 0) * 0.75f;
	}
}