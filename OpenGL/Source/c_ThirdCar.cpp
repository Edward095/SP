#include "c_ThirdCar.h"
#include "Application.h"
#include "c_ObjectManager.h"
#include "c_OffRoadManager.h"
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
	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed('P') && !once)
		{
			float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * 75);
			float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * 75);

			if (pos.x + updateX < 700 && pos.z + updateZ < 700 && pos.x + updateX > -700 && pos.z + updateZ > -700)
			{
				updatePos(pos.x + updateX, pos.y, pos.z + updateZ);
			}
			//PressQ = true;
			once = true;
		}
	}
	else
	{
		if (Application::IsKeyPressed('3') && !once)
		{
			float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * 75);
			float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * 75);
			if (pos.x + updateX < 700 && pos.z + updateZ < 700 && pos.x + updateX > -700 && pos.z + updateZ > -700)
			{
				updatePos(pos.x + updateX, pos.y, pos.z + updateZ);
			}
			//PressQ = true;
			once = true;
		}
	}
	
	if (once)
	{
		Duration++;
		
	}
	if (Duration >= 50) // 3 sec/dt
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
		SetMaxSpeed(1.f);
	}
}