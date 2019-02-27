#include "NPCCamera.h"
#include "Application.h"


NPCCamera::NPCCamera()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

NPCCamera::~NPCCamera()
{
}

void NPCCamera::Init(const Vector3& pos)
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

void NPCCamera::UpdateMouse()
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

void NPCCamera::Update(double dt)
{
	elapsedTime += (float)dt;
	
	UpdateMouse();
	position.y = 75;

}

Mtx44 NPCCamera::LookAt()
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

void NPCCamera::Invert()
{
	pitch = -pitch;
}

bool NPCCamera::checkCollisions(float xUpper, float xLower, float zUpper, float zLower)
{
	return (position.x > xUpper || position.x < xLower ||
		position.z >zUpper || position.z < zLower);
}

void NPCCamera::WBmove(double dt, float xUpper, float xLower, float zUpper, float zLower)
{
	static const float NPCCamera_SPEED = 100.f;

	Vector3 right = front.Cross(up).Normalize();

	if (Application::IsKeyPressed('D'))
	{
		position += right * NPCCamera_SPEED * (float)(dt);
		if (checkCollisions(xUpper, xLower,zUpper,zLower))
		{
			position -= right * NPCCamera_SPEED * (float)(dt);
		}
	}
	if (Application::IsKeyPressed('W'))
	{
		position += front * NPCCamera_SPEED * (float)(dt);
		if (checkCollisions(xUpper, xLower, zUpper, zLower))
		{
			position -= front * NPCCamera_SPEED * (float)(dt);
		}
	}
	if (Application::IsKeyPressed('A'))
	{
		position -= right * NPCCamera_SPEED * (float)(dt);
		if (checkCollisions(xUpper, xLower, zUpper, zLower))
		{
			position += right * NPCCamera_SPEED * (float)(dt);
		}
	}
	if (Application::IsKeyPressed('S'))
	{
		position -= front * NPCCamera_SPEED * (float)(dt);
		if (checkCollisions(xUpper, xLower, zUpper, zLower))
		{
			position += front * NPCCamera_SPEED * (float)(dt);
		}
	}
}
void NPCCamera::WOBmove(double dt)
{
	static const float NPCCamera_SPEED = 100.f;

	Vector3 right = front.Cross(up).Normalize();

	if (Application::IsKeyPressed('D'))
	{
		position += right * NPCCamera_SPEED * (float)dt;
	}
	if (Application::IsKeyPressed('W'))
	{
		position += front * NPCCamera_SPEED * (float)dt;
	}
	if (Application::IsKeyPressed('A'))
	{
		position -= right * NPCCamera_SPEED * (float)dt;
	}
	if (Application::IsKeyPressed('S'))
	{
		position -= front * NPCCamera_SPEED * (float)dt;
	}
}