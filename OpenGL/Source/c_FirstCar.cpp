#include "c_FirstCar.h"
#include "Application.h"

c_FirstCar::c_FirstCar()
{
	driving = false;
	VelocityZ = 0;
	acceleration = 0;
	VelocityX = 0;
}

c_FirstCar::~c_FirstCar()
{

}

void c_FirstCar::Movement(double dt)
{
	if (Application::IsKeyPressed('W'))
	{
		acceleration += 0.1;
		VelocityZ += acceleration * dt;
		driving = true;
		backwards = false;

		if (acceleration > 30)
		{
			acceleration = 30;
		}
	}
	if (driving)
	{
		if (!Application::IsKeyPressed('W'))
		{
			acceleration -= 0.5;
			VelocityZ += acceleration * dt;

			if (acceleration <0)
			{
				acceleration = 0;
				driving = false;
			}
		}

	}





	if (Application::IsKeyPressed('S'))
	{
		acceleration -= 0.1;
		VelocityZ += acceleration * dt;
		backwards = true;
		driving = false;

		if (acceleration < -10)
		{
			acceleration = -10;
		}

	}
	if (backwards)
	{
		if (!Application::IsKeyPressed('S'))
		{
			//velocityZ = 0;
			acceleration += 0.5;
			VelocityZ += (float)(acceleration * dt);

			if (acceleration > 0)
			{
				acceleration = 0;
				backwards = false;
			}
		}
	}


}

float c_FirstCar::GetVelocityZ()
{
	return VelocityZ;
}

