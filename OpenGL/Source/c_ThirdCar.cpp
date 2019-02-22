#include "c_ThirdCar.h"
#include "Application.h"
#include "c_ObjectManager.h"
#include "LoadTGA.h"

c_ThirdCar::c_ThirdCar()
{
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;

	MaxSpeed = 1;
	SteeringAngle = 0;
	Duration = 0;
	MaxAcceleration = 0.4;
	Friction = 0.04;
	Steering = 2;
	Driving = false;
	Backwards = false;
	PressQ = false;
	Nitro = false;
	BoostPad = false;
	SlowPad = false;
	once = false;

	offRoad = false;
}
c_ThirdCar::c_ThirdCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos,bool canCollide)
{
	init(uniqueName, meshPath, TGApath, pos, canCollide);
}


c_ThirdCar::~c_ThirdCar()
{
}


void c_ThirdCar::Ability(double dt)
{
	if (Application::IsKeyPressed('3') && !once)
	{
		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * 75);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * 75);

		updatePos(pos.x + updateX, pos.y, pos.z + updateZ);
		//PressQ = true;
		once = true;
	}
	if (once)
	{
		Duration++;
		
	}
	if (Duration >= 150) // 3 sec/dt
	{
		PressQ = false;
		once = false;
		Duration = 0;
	}
}

void c_ThirdCar::PowerUp(bool check)
{

}

void c_ThirdCar::isOffRoad()
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
		SetMaxSpeed(1.f);
	}
}