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
	Cooldown = 300;
	offRoad = false;
	abilityUsed = false;
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
	c_Sound* Audio = c_Sound::getInstance();

	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed('P'))
		{
			if (Driving || Backwards)
			{
				PressQ = true;
			}
		}
	}
	else
	{
		if (Application::IsKeyPressed('Q'))
		{
			if (Driving)
				PressQ = true;
			else
				PressQ = false;
		}
	}
	
	if (VelocityZ > MaxSpeed && (PressQ))
	{
		if (VelocityZ < 1.5)
		{
			VelocityZ += 0.1;
		}
		else
		{
			VelocityZ = 1.5;
		}
	}
	if (PressQ)
	{
		Cooldown = 300;
		Duration++;
		if (!abilityUsed)
		{
			abilityUsed = true;
			Audio->f_Game_Ability_Nitro();
		}
	}
	if (Duration > 200) // 4 sec/dt
	{
		if (VelocityZ > MaxSpeed)
		{
			VelocityZ -= 0.3;
		}
		else
		{
			VelocityZ = MaxSpeed;
			PressQ = false;
			Cooldown--;
		}
		if (Cooldown <= 0)
		{
			Duration = 0;
			Cooldown = 300;
		}


	}

	if (Cooldown <= 0)
	{
		abilityUsed = false;
		Duration = 0;
		Cooldown = 300;
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
		if (Application::IsKeyPressed('Q') || Application::IsKeyPressed('P'))
		{
			if (Driving || Backwards)
			{
				PressQ = false;
			}
		}
		SetFriction(0.5f);
		SetMaxSpeed(0.3f);
	}
	else
	{
		SetFriction(0.04);
		SetMaxSpeed(0.8f);
	}
}