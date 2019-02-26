#include "c_SecondCar.h"
#include "c_ObjectManager.h"
#include "c_OffRoadManager.h"
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
	FreezeTime = 0;
	Driving = false;
	Backwards = false;
	PressQ = false;
	Nitro = false;
	BoostPad = false;
	SlowPad = false;
	once = false;

	abilityUsed = true;

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
	FreezeTime = (float)(dt + (dt * 0));

	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed('P'))
		{
			if (abilityUsed)
			{
				Audio->f_Game_Ability_Freezetime();
				abilityUsed = false;
			}
			PressQ = true;
		}
	}
	else
	{
		if (Application::IsKeyPressed('2'))
		{
			if (abilityUsed)
			{
				Audio->f_Game_Ability_Freezetime();
				abilityUsed = false;
			}
			PressQ = true;
		}
	}


	if (PressQ && Duration <= 150)
	{
		Duration++;
		elapsedTime -= FreezeTime;
		if (Duration >= 150) // 3 sec/dt
		{
			PressQ = false;
			Duration = 0;
		}
	}
}

void c_SecondCar::PowerUp(bool check)
{

}
void c_SecondCar::isOffRoad()
{
	c_OffRoadManager* manager = c_OffRoadManager::getInstance();

	for (int i = 0; i < manager->getList().size(); i++)
	{
		if (gotCollide(manager->getList()[i], false) || !gotCollide("track", false))
		{
			offRoad = true;
			break;
		}
		else offRoad = false;
	}
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