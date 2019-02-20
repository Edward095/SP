#include "c_FirstCar.h"
#include "Application.h"
#include "MyMath.h"
#include "c_ObjectManager.h"
#include "LoadTGA.h"

c_FirstCar::c_FirstCar()
{
	//Driving = false;
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;
	MaxSpeed = 0.8;
	SteeringAngle = 0;
	Duration = 0;
	MaxAcceleration = 0.6;
	Friction = 0.04;
	Steering = 3;
	once = false;
}
c_FirstCar::c_FirstCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos)
{
	init(uniqueName, meshPath, TGApath, pos);
}
c_FirstCar::~c_FirstCar()
{

}

void c_FirstCar::Movement(double dt)
{
	
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
			Ability(dt);
			if (Acceleration > 1)
				Acceleration = 1;
			if (VelocityZ > 1 && (PressQ || Nitro))
				VelocityZ = 1.5;
			else if (VelocityZ > 1 && (!PressQ || !Nitro))
				VelocityZ = 1;
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
			Acceleration -= 0.1;
			VelocityZ += Acceleration * dt;

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
		if (Driving)
			SteeringAngle -= 3;
		if (Backwards)
			SteeringAngle += 3;
		
	}


	if (Application::IsKeyPressed('A'))
	{
		if (Driving)
			SteeringAngle += 3;
		if (Backwards)
			SteeringAngle -= 3;
	}


	if (Application::IsKeyPressed('S') && Driving == false)
	{
		Acceleration -= 0.1;
		VelocityZ += Acceleration * dt;

		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);

		if (!gotCollide(updateX, pos.y, updateZ))
		{
			Backwards = true;
			Driving = false;
			Ability(dt);
			if (Acceleration < -1)
				Acceleration = -1;
			if (VelocityZ < -1 && (PressQ || Nitro))
				VelocityZ = -2;
			else if (VelocityZ < -1 && (!PressQ || !Nitro))
				VelocityZ = -1;
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
			Acceleration += 0.1;
			VelocityZ += Acceleration * dt;

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
}

void c_FirstCar::Ability(double dt)
{
	if (Application::IsKeyPressed('Q'))
	{
		if (Driving || Backwards)
			PressQ = true;
	}

	if (PressQ)
	{
		Duration++;
		if (Duration >= 150) // 3 sec/dt
		{
			PressQ = false;
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