#include "c_SecondCar.h"
#include "c_ObjectManager.h"
#include "LoadTGA.h"


c_SecondCar::c_SecondCar()
{
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