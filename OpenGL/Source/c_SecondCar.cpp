#include "c_SecondCar.h"
#include "c_ObjectManager.h"
#include "c_OffRoadManager.h"
#include "c_SceneManager.h"
#include "LoadTGA.h"


c_SecondCar::c_SecondCar()
{
	MaxSpeed = 2.f;
	SteeringAngle = 0;
	Steering = 4.f;
	MaxAcceleration = 0.8f;
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
	c_SceneManager* scene = c_SceneManager::getInstance();
	c_ObjectManager* obj = c_ObjectManager::getInstance();
	elapsedTime += (float)(dt);

	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed('P') && !PressQ)
		{
			PressQ = true;
			if (scene->singleOrMulti('S'))
			{
				stopTime = true;
				abilityDuration = elapsedTime + 5.f;
				coolDown = elapsedTime + 15.f;
			}
			else if (scene->singleOrMulti('M'))
			{
				abilityDuration = elapsedTime + 5.f;
				coolDown = elapsedTime + 15.f;
				c_Entity* entity = obj->getCanCollide("player2",this->uniqueName);
				if (entity == nullptr)
					entity = obj->getCanCollide("player1", this->uniqueName);

				c_CarBaseClass* car;
				c_FirstCar* first = dynamic_cast <c_FirstCar*>(entity);
				if (first)
					car = first;
				c_SecondCar* second = dynamic_cast <c_SecondCar*>(entity);
				if (second)
					car = second;
				c_ThirdCar* third = dynamic_cast <c_ThirdCar*>(entity);
				if (third)
					car = third;

				car->SetMaxSpeed(1.f);
				car->setSlowed(true);
			}
			
			Audio->f_Game_Ability_Freezetime();
		}
	}
	else
	{
		if (Application::IsKeyPressed('Q') && !PressQ)
		{
			PressQ = true;
			if (scene->singleOrMulti('S'))
			{
				stopTime = true;
				abilityDuration = elapsedTime + 5.f;
				coolDown = elapsedTime + 15.f;
			}
			else if (scene->singleOrMulti('M'))
			{
				abilityDuration = elapsedTime + 5.f;
				coolDown = elapsedTime + 15.f;
				c_Entity* entity = obj->getCanCollide("player2", this->uniqueName);
				if (entity == nullptr)
					entity = obj->getCanCollide("player1", this->uniqueName);

				c_CarBaseClass* car;
				c_FirstCar* first = dynamic_cast <c_FirstCar*>(entity);
				if (first)
					car = first;
				c_SecondCar* second = dynamic_cast <c_SecondCar*>(entity);
				if (second)
					car = second;
				c_ThirdCar* third = dynamic_cast <c_ThirdCar*>(entity);
				if (third)
					car = third;

				car->SetMaxSpeed(1.f);
				car->setSlowed(true);
			}

			Audio->f_Game_Ability_Freezetime();
		}
	}
	if (elapsedTime >= abilityDuration)
	{
		if (scene->singleOrMulti('S'))
		{
			stopTime = false;
		}
		else if (scene->singleOrMulti('M'))
		{
			c_Entity* entity = obj->getCanCollide("player2", this->uniqueName);
			if(entity == nullptr)
				entity = obj->getCanCollide("player1", this->uniqueName);
			c_CarBaseClass* car;
			c_FirstCar* first = dynamic_cast <c_FirstCar*>(entity);
			if (first)
			{
				car = first;
				car->SetMaxSpeed(2.5f);
			}
			c_SecondCar* second = dynamic_cast <c_SecondCar*>(entity);
			if (second)
			{
				car = second;
				car->SetMaxSpeed(2.f);
			}			
			c_ThirdCar* third = dynamic_cast <c_ThirdCar*>(entity);
			if (third)
			{
				car = third;
				car->SetMaxSpeed(3.f);
			}
			car->setSlowed(false);
		}
	}
	if (elapsedTime >= coolDown)
	{
		PressQ = false;
	}
}
void c_SecondCar::isOffRoad()
{
	c_OffRoadManager* manager = c_OffRoadManager::getInstance();

	for (int i = 0; i < (int)manager->getList().size(); i++)
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
		if (slowed)
			MaxSpeed = 1.f;
		else
			MaxSpeed = 2.f;
	}
}
void c_SecondCar::resetVar()
{
	MaxSpeed = 2.f;
	SteeringAngle = 0;
	Steering = 4.f;
	MaxAcceleration = 0.8f;
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