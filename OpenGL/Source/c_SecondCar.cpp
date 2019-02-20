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
	once = false;
}
c_SecondCar::c_SecondCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos)
{
	init(uniqueName, meshPath, TGApath, pos);
}


c_SecondCar::~c_SecondCar()
{
}

void c_SecondCar::Ability(double dt)
{

}

void c_SecondCar::PowerUp(bool check)
{

}