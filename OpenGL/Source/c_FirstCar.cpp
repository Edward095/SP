#include "c_FirstCar.h"
#include "Application.h"
#include "MyMath.h"

c_FirstCar::c_FirstCar()
{
	Driving = false;
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;

	MaxSpeed = 0;
	SteeringAngle = 0;
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
			Ability();
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
			Ability();
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

void c_FirstCar::Ability()
{
	if (Application::IsKeyPressed('Q'))
	{
		if (Driving || Backwards)
		{
			PressQ = true;
		}
	}

	if (PressQ)
	{
		if (VelocityZ < 1)
		{
			PressQ = false;
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