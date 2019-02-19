#include "c_FirstCar.h"
#include "Application.h"
#include "MyMath.h"
#include "c_ObjectManager.h"
#include "LoadTGA.h"

c_FirstCar::c_FirstCar()
{
	Driving = false;
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;

	MaxSpeed = 1;
	SteeringAngle = 0;
	Duration = 0;
	MaxAcceleration = 1;
	Friction = 0.01;
	once = false;
}
c_FirstCar::~c_FirstCar()
{

}

void c_FirstCar::Ability(double dt)
{
	//if (Application::IsKeyPressed('Q'))
	//{
	//	if (Driving || Backwards)
	//		PressQ = true;
	//}

	//if (PressQ)
	//{
	//	Duration++;
	//	if (Duration >= 150) // 3 sec/dt
	//	{
	//		PressQ = false;
	//		Duration = 0;
	//	}
	//}
	if (Application::IsKeyPressed('Q') && !once)
	{
		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * 75);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * 75);

		updatePos(pos.x + updateX, pos.y, pos.z + updateZ);
		PressQ = true;
		once = true;
	}
	if (PressQ && once)
	{
		Duration++;
		if (Duration >= 150) // 3 sec/dt
		{
			PressQ = false;
			once = false;
			Duration = 0;
		}
	}
	if (BoostPad)
	{
		Duration++;
		if (Duration >= 150) // 3 sec/dt
		{
			BoostPad = false;
			Duration = 0;
		}
	}
	if (SlowPad)
	{
		Duration++;
		if (Duration >= 150) // 3 sec/dt
		{
			SlowPad = false;
			Duration = 0;
		}
	}
}

void c_FirstCar::PowerUp(bool check)
{
	if (check)
	{
		Nitro = true;
	}

}
