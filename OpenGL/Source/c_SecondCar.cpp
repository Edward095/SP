#include "c_SecondCar.h"
#include "c_ObjectManager.h"
#include "LoadTGA.h"


c_SecondCar::c_SecondCar()
{
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;

	MaxSpeed = 0.6;
	SteeringAngle = 0;
	Duration = 0;
	MaxAcceleration = 0.8;
	Friction = 0.04;
	Steering = 4;
	elapsedTime = 0;
	//FreezeTime = 0;
	Driving = false;
	Backwards = false;
	PressQ = false;
	Nitro = false;
	BoostPad = false;
	SlowPad = false;
	once = false;

	offRoad = false;
}
c_SecondCar::c_SecondCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos, bool canCollide)
{
	init(uniqueName, meshPath, TGApath, pos, canCollide);
}


c_SecondCar::~c_SecondCar()
{
}

void c_SecondCar::Ability(double dt)
{
	elapsedTime += dt;
	//FreezeTime = (float)(dt + (dt * 0));

	if (Application::IsKeyPressed('Q'))
	{
			PressQ = true;
	}

	if (PressQ && Duration <= 150)
	{
		Duration++;
		//elapsedTime -= FreezeTime;
	}

	if (Duration >= 150) // 3 sec/dt
	{
		PressQ = false;
		Duration = 0;
	}
}

void c_SecondCar::PowerUp(bool check)
{

}
void c_SecondCar::isOffRoad()
{
	if (!gotCollide("track",false))//|| gotCollide("offRoad1") || gotCollide("offRoad2") || gotCollide("offRoad3") || gotCollide("offRoad4") || gotCollide("offRoad5") || gotCollide("offRoad6"))
		offRoad = true;
	else
		offRoad = false;
	if (offRoad)
	{
		SetFriction(0.5f);
		SetMaxSpeed(0.3f);
	}
	else
	{
		SetFriction(0.04f);
		SetMaxSpeed(0.6f);
	}
}