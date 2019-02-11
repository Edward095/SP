#ifndef FirstPersonCamera_H
#define FirstPersonCamera_H

#include "Vector3.h"
#include "Mtx44.h"


class FirstPersonCamera
{
public:

	Vector3 position;
	Vector3 target;
	Vector3 front;
	Vector3 up;
	
	FirstPersonCamera();
	~FirstPersonCamera();
	virtual void Init(const Vector3& pos);
	virtual void Update(double dt);

	Mtx44 LookAt();
	void Invert();
	float yaw = -90.0f;
	float pitch = 0.0f;

private:
	bool firstMouse;
	double lastX = 0.0;
	double lastY = 0.0;
	float sensitivity = 0.08f;
	
	float bounceTime = 0.f;
	float elapsedTime = 0.f;
	bool noFly = true;
	bool offBound = false;
	void UpdateMouse();
	bool checkCollisions(float x, float zUpper,float zLower);
	void WBmove(double dt, float x, float zUpper, float zLower);//WB = with bounderies
	void WOBmove(double dt);//WOB = without bounderies
	void gunRoom(double dt);
};

#endif