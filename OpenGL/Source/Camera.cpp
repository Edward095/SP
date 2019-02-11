#include "Camera.h"
#include "Application.h"
/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera::Update(double dt)
{
	Vector3 forward = target - position;
	if (Application::IsKeyPressed('E'))
		position += 0.3  * dt * forward;
	if(Application::IsKeyPressed('Q'))
		position -= 0.3 * dt * forward;
	if (Application::IsKeyPressed('A'))
		position -= forward.Cross(up).Normalized() * 0.5;
	if (Application::IsKeyPressed('D'))
		position += forward.Cross(up).Normalized() * 0.5;
	if (Application::IsKeyPressed('W'))
		position += Vector3(0, 1, 0) * 100 * dt;
	if (Application::IsKeyPressed('S'))
		position -= Vector3(0, 1, 0) * 100 * dt;
}