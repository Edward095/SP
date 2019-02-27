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
	MaxSpeed = 50;
	SteeringAngle = 0;
	MaxAcceleration = 0.6;
	Friction = 0.04;
	Steering = 3;
	Driving = false;
	Backwards = false;
	PressQ = false;
	Nitro = false;
	BoostPad = false;
	SlowPad = false;
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
	c_Sound* Audio = c_Sound::getInstance();
	elapsedTime += dt;

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
	}
	//Ability OnGoing
	if (PressQ && elapsedTime < abilityDuration)
	{
		if (VelocityZ < 1.5f)
			VelocityZ += 0.2f;
		else 
			VelocityZ = 1.5f;
	}
	//Ability Finished but cooldown haven end
	if (PressQ && elapsedTime > abilityDuration && elapsedTime < coolDown)
	{
		if (VelocityZ > MaxSpeed)
			VelocityZ -= 0.2f;
		else
			VelocityZ = MaxSpeed;
	}
	//Cooldown ends ability can be used again
	else if (PressQ && elapsedTime >= coolDown)
	{
		coolDown = 0.f;
		PressQ = false;
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
		SetMaxSpeed(3.f);
	}
}