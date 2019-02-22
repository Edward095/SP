#include "c_FirstCar.h"
#include "Application.h"
#include "MyMath.h"
#include "c_ObjectManager.h"
#include "LoadTGA.h"

c_FirstCar::c_FirstCar()
{
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;
	MaxSpeed = 0.8;
	SteeringAngle = 0;
	Duration = 0;
	MaxAcceleration = 0.6;
	Friction = 0.04;
	Steering = 3;
	Driving = false;
	Backwards = false;
	PressQ = false;
	Nitro = false;
	BoostPad = false;
	SlowPad = false;
	once = false;
}
c_FirstCar::c_FirstCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos)
{
	init(uniqueName, meshPath, TGApath, pos);
}
c_FirstCar::~c_FirstCar()
{

}


void c_FirstCar::Ability(double dt)
{
	if (Application::IsKeyPressed('1'))
	{
		if (Driving || Backwards)
			PressQ = true;
	}
	if (VelocityZ > MaxSpeed && (PressQ))
				VelocityZ = 1.5; 

	if (PressQ)
	{
		//VelocityZ += Acceleration * (float)dt;

		//VelocityZ = 1.5;
		Duration++;
		
	}
	if (Duration >= 150) // 3 sec/dt
	{
		PressQ = false;
		Duration = 0;
	}
}

void c_FirstCar::PowerUp(bool check)
{
	if (check)
	{
		Nitro = true;
	}

}