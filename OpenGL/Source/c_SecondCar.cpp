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
	Driving = false;
	Backwards = false;
	PressQ = false;
	Nitro = false;
	BoostPad = false;
	SlowPad = false;
	once = false;
	offRoad = false;
	Oslowed = false;
	Tslowed = false;
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
	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed('P'))
		{
			//PressQ = true;
		}
	}
	else
	{
		if (Application::IsKeyPressed('Q'))
		{
			//PressQ = true;
		}
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

		if (Oslowed && !Tslowed)
			SetMaxSpeed(0.3f);

		if (Tslowed && !Oslowed)
			SetMaxSpeed(0.3f);

		if (!Tslowed && !Oslowed)
			SetMaxSpeed(0.6f);

		if (Tslowed && Oslowed)
			SetMaxSpeed(0.6f);
	}
}

void c_SecondCar::SetOSlowed(bool speed)
{
	Oslowed = speed;
}

void c_SecondCar::SetTSlowed(bool speed)
{
	Tslowed = speed;
}
