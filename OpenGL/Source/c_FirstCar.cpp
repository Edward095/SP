#include "c_FirstCar.h"
#include "Application.h"
#include "MyMath.h"
#include "c_ObjectManager.h"
#include "c_OffRoadManager.h"
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

	abilityUsed = true;
	offRoad = false;
}
c_FirstCar::c_FirstCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos, bool canCollide)
{
	init(uniqueName, meshPath, TGApath, pos, canCollide);
}
c_FirstCar::~c_FirstCar()
{

}


void c_FirstCar::Ability(double dt)
{
	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed('P'))
		{
			if (Driving || Backwards)
				if (abilityUsed)
				{
					Audio->f_Game_Ability_Nitro();
					abilityUsed = false;
				}
				PressQ = true;
		}
	}
	else
	{
		if (Application::IsKeyPressed('1'))
		{
			if (Driving || Backwards)
				if (abilityUsed)
				{
					Audio->f_Game_Ability_Nitro();
					abilityUsed = false;
				}
				PressQ = true;
		}
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
		abilityUsed = true;
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
void c_FirstCar::isOffRoad()
{
	c_OffRoadManager* manager = c_OffRoadManager::getInstance();

	for (int i = 0; i < manager->getList().size(); i++)
	{
		if (gotCollide(manager->getList()[i],false) || !gotCollide("track", false))
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
		SetFriction(0.04);
		SetMaxSpeed(0.8f);
	}
}