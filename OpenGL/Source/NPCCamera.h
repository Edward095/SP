#ifndef NPCCAMERA_H
#define NPCCAMERA_H

#include "Vector3.h"
#include "Mtx44.h"


class NPCCamera
{
public:

	Vector3 position;
	Vector3 target;
	Vector3 front;
	Vector3 up;
	
	NPCCamera();
	~NPCCamera();
	virtual void Init(const Vector3& pos);
	virtual void Update(double dt);

	Mtx44 LookAt();
	void Invert();
	float yaw = -90.0f;
	float pitch = 0.0f;
	void WBmove(double dt, float xUpper, float xLower, float zUpper, float zLower);//WB = with bounderies
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
	bool checkCollisions(float xUpper, float xLower, float zUpper,float zLower);
	
	void WOBmove(double dt);//WOB = without bounderies
	void gunRoom(double dt);
};

#endif