#include "c_FirstCar.h"
#include "Application.h"
#include "MyMath.h"
#include "c_ObjectManager.h"
#include "c_OffRoadManager.h"
#include "LoadTGA.h"

c_FirstCar::c_FirstCar()
{
	MaxSpeed = 2.5f;
	SteeringAngle = 0;
	Steering = 3;
	MaxAcceleration = 0.6f;
	Friction = 0.04f;
	padDuration = 0;
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;
	SpedoVeloZ = 0;
	elapsedTime = 0.f;
	coolDown = 0.f;
	abilityDuration = 0.f;

	Driving = false;
	Backwards = false;
	PressQ = false;
	BoostPad = false;
	SlowPad = false;
	offRoad = false;
	Collided = false;
	stopTime = false;
	slowed = false;
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
	elapsedTime += (float)(dt);

	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed('P'))
		{
			if (Driving)
				PressQ = true;
		}
	}
	else
	{
		if (Application::IsKeyPressed('Q'))
		{
			if (Driving)
				PressQ = true;
		}
	}
	
	//Ability Used,Setting how long it lasts and the cooldown duration
	if (PressQ && coolDown == 0.f)
	{
		abilityDuration = elapsedTime + 4.f;
		coolDown = elapsedTime + 8.f;
		Audio->f_Game_Ability_Nitro();
		//MaxSpeed = 1.5f;
	}
	//Ability Finished but cooldown haven end
	if (PressQ && elapsedTime > abilityDuration && elapsedTime < coolDown)
	{
		MaxSpeed = 0.8f;
	}
	//Cooldown ends ability can be used again
	else if (PressQ && elapsedTime >= coolDown)
	{
		coolDown = 0.f;
		PressQ = false;
	}
}
void c_FirstCar::isOffRoad()
{
	c_OffRoadManager* manager = c_OffRoadManager::getInstance();

	for (int i = 0; i < (int)manager->getList().size(); i++)
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
		SetFriction(0.04f);
		if (elapsedTime < abilityDuration)
			MaxSpeed = 3.f;
		else if (slowed)
			SetMaxSpeed(1.f);
		else
			SetMaxSpeed(2.5f);
	}
}
void c_FirstCar::resetVar()
{
	MaxSpeed = 2.5f;
	SteeringAngle = 0;
	Steering = 3;
	MaxAcceleration = 0.6f;
	Friction = 0.04f;
	padDuration = 0;
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;
	SpedoVeloZ = 0;
	elapsedTime = 0.f;
	coolDown = 0.f;
	abilityDuration = 0.f;

	Driving = false;
	Backwards = false;
	PressQ = false;
	BoostPad = false;
	SlowPad = false;
	offRoad = false;
	Collided = false;
	stopTime = false;
	slowed = false;
}