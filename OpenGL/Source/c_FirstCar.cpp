#include "c_FirstCar.h"
#include "Application.h"
#include "MyMath.h"

c_FirstCar::c_FirstCar()
{
	Driving = false;
	VelocityZ = 0;
	Acceleration = 0;

	MaxSpeed = 0;
	SteeringAngle = 0;
	frontCollide = false;
	backCollide = false;
}
c_FirstCar::~c_FirstCar()
{

}

void c_FirstCar::Movement(double dt)
{
	if (Application::IsKeyPressed('W'))
	{
		Acceleration += 0.1;
		VelocityZ += Acceleration * dt;
		pos.x += (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		pos.y = pos.y;
		pos.z += (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		if (!backCollide)
		{
			if (gotCollide())
			{
				frontCollide = true;
				pos.x -= (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
				pos.y = pos.y;
				pos.z -= (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			}
			else
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
		}
		Driving = true;
		Backwards = false;
		Ability();
		if (Acceleration > 1)
			Acceleration = 1;

		if (VelocityZ > 1 && (PressQ || Nitro))
			VelocityZ = 1.5;
		else if (VelocityZ > 1 && (!PressQ || !Nitro))
			VelocityZ = 1;
		backCollide = false;
	}
	if (Driving)
	{
		if (!Application::IsKeyPressed('W'))
		{
			Acceleration -= 0.1;
			VelocityZ += Acceleration * dt;
			pos.x += (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			pos.y = pos.y;
			pos.z += (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			if (gotCollide())
			{
				pos.x -= (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
				pos.y = pos.y;
				pos.z -= (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			}
			else
			{
				if (Acceleration < 0)
				{
					Acceleration = 0;
					VelocityZ -= 0.05;
				}
				if (VelocityZ < 0)
				{
					VelocityZ = 0;
				}
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


	if (Application::IsKeyPressed('S'))
	{
		Acceleration -= 0.1;
		VelocityZ += Acceleration * dt;
		pos.x += (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		pos.y = pos.y;
		pos.z += (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		if (!frontCollide)
		{
			if (gotCollide())
			{
				backCollide = true;
				pos.x -= (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
				pos.y = pos.y;
				pos.z -= (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			}
			else
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
		}
		Backwards = true;
		Driving = false;
		Ability();
		if (Acceleration < -1)
			Acceleration = -1;
		if (VelocityZ < -1 && (PressQ || Nitro))
			VelocityZ = -2;
		else if (VelocityZ < -1 && (!PressQ || !Nitro))
			VelocityZ = -1;
		frontCollide = false;
	}

	if (Backwards)
	{
		if (!Application::IsKeyPressed('S'))
		{
			Acceleration += 0.1;
			VelocityZ += Acceleration * dt;
			pos.x += (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			pos.y = pos.y;
			pos.z += (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);

			if (Acceleration > 0)
			{
				Acceleration = 0;
				VelocityZ += 0.05;
			}
			if (VelocityZ > 0)
			{
				VelocityZ = 0;
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