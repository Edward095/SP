#include "c_FirstCar.h"
#include "Application.h"
#include "MyMath.h"
#include "c_ObjectManager.h"
#include "LoadTGA.h"

c_FirstCar::c_FirstCar()
{
	Driving = false;
	Backwards = false;
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;

	MaxSpeed = 1;
	SteeringAngle = 0;
	Duration = 0;

	once = false;
	MaxAcceleration = 1;
	Friction = 0.5;

}
c_FirstCar::~c_FirstCar()
{

}

void c_FirstCar::Ability(double dt)
{

	//if (Application::IsKeyPressed('Q'))
	//{
	//	if (Driving || Backwards)
	//		PressQ = true;
	//}

	//if (PressQ)
	//{
	//	Duration++;
	//	if (Duration >= 150) // 3 sec/dt
	//	{
	//		PressQ = false;
	//		Duration = 0;
	//	}
	//}
	if (Application::IsKeyPressed('Q') && !once)


	Ability(dt);
	if (Application::IsKeyPressed('W') && Backwards == false)
	{
		Acceleration += 0.1;
		VelocityZ += Acceleration * dt;

		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		if (!gotCollide(updateX, pos.y, updateZ))
		{
			Driving = true;
			Backwards = false;
			//Ability(dt);
			if (Acceleration > MaxAcceleration - Friction)
				Acceleration = MaxAcceleration - Friction;
			if (VelocityZ > MaxSpeed && (PressQ))
				VelocityZ =  1.5;
			else if (VelocityZ > MaxSpeed && (!PressQ || !Nitro))
				VelocityZ = MaxSpeed;
		}
		else
		{
			Acceleration -= 0.1;
			VelocityZ -= Acceleration * dt;
		}
		
	}
	if (Driving)
	{
		if (!Application::IsKeyPressed('W'))
		{
			Acceleration -= Friction;
			VelocityZ -= Acceleration * dt;

			float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			if (!gotCollide(updateX, pos.y, updateZ))
			{
				if (Acceleration < 0)
				{
					Acceleration = 0;
					VelocityZ -= 0.05;
				}
				if (VelocityZ < 0)
				{
					VelocityZ = 0;
					Driving = false;
					Backwards = false;
				}
			}
			else
			{
				Acceleration -= 0.1;
				VelocityZ -= Acceleration * dt;
			}
		}
	}


	if (Application::IsKeyPressed('D'))
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
		Acceleration -= 0.1;
		VelocityZ += Acceleration * dt;

		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);

		if (!gotCollide(updateX, pos.y, updateZ))
		{
			Backwards = true;
			Driving = false;
			//Ability(dt);
			if (Acceleration < -MaxAcceleration + Friction)
				Acceleration = -MaxAcceleration + Friction;
			if (VelocityZ < -MaxSpeed && (PressQ))
				VelocityZ = -1.5;
			else if (VelocityZ < -MaxSpeed && (!PressQ || !Nitro))
				VelocityZ = -MaxSpeed;
		}
		else
		{
			Acceleration += 0.1;
			VelocityZ -= Acceleration * dt;
		}
	}

	if (Backwards)
	{
		if (!Application::IsKeyPressed('S'))
		{
			Acceleration += Friction;
			VelocityZ -= Acceleration * dt;

			float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			if (!gotCollide(updateX, pos.y, updateZ))
			{
				if (Acceleration > 0)
				{
					Acceleration = 0;
					VelocityZ += 0.05;
				}
				if (VelocityZ > 0)
				{
					VelocityZ = 0;
					Driving = false;
					Backwards = false;
				}
			}
			else
			{
				Acceleration -= 0.1;
				VelocityZ -= Acceleration * dt;
			}
		}
	}
	if (BoostPad)
	{
		Duration++;
		if (Duration >= 150) // 3 sec/dt
		{
			BoostPad = false;
			Duration = 0;
		}
	}
}

void c_FirstCar::PowerUp(bool check)
{
	if (check)
	{
		Nitro = true;
	}

}
