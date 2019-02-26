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
	abilityUsed = false;
	audioCD = 0;
	audioUsed = false;

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
	c_Sound* Audio = c_Sound::getInstance();
	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed('P'))
		{
			//PressQ = true;
			//PressQ = true;
			audioUsed = true;
		}
	}
	else
	{
		if (Application::IsKeyPressed('Q'))
		{
			//PressQ = true;
			//PressQ = true;
			audioUsed = true;
		}
	}
	if (audioUsed)
	{
		audioCD++;
		if (!abilityUsed)
		{
			Audio->f_Game_Ability_Freezetime();
			abilityUsed = true;
		}
	}
	if (audioCD >= 500)
	{
		abilityUsed = false;
		audioUsed = false;
		audioCD = 0;
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
