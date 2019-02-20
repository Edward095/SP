#include "c_ThirdCar.h"
#include "Application.h"
#include "c_ObjectManager.h"
#include "LoadTGA.h"

c_ThirdCar::c_ThirdCar()
{
}
c_ThirdCar::c_ThirdCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos)
{
	init(uniqueName, meshPath, TGApath, pos);
}

c_ThirdCar::~c_ThirdCar()
{
}


void c_ThirdCar::Ability(double dt)
{
	if (Application::IsKeyPressed('Q') && !once)
	{
		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * 75);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * 75);

		updatePos(pos.x + updateX, pos.y, pos.z + updateZ);
		PressQ = true;
		once = true;
	}
	if (PressQ && once)
	{
		Duration++;
		if (Duration >= 150) // 3 sec/dt
		{
			PressQ = false;
			once = false;
			Duration = 0;
		}
	}
}

void c_ThirdCar::PowerUp(bool check)
{

}