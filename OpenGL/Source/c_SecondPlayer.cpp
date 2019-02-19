#include "c_SecondPlayer.h"
#include "Application.h"


c_SecondPlayer::c_SecondPlayer()
{
	Driving = false;
	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;

	MaxSpeed = 0;
	SteeringAngle = 0;
	Duration = 0;

	once = false;
}


c_SecondPlayer::~c_SecondPlayer()
{
}

void c_SecondPlayer::Movement(double dt)
{
	Ability(dt);
	if (Application::IsKeyPressed(VK_UP) && Backwards == false)
	{
		Acceleration += 0.1;
		VelocityZ += Acceleration * dt;

		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		if (!gotCollide(updateX, pos.y, updateZ))
		{
			Driving = true;
			Backwards = false;
			if (Acceleration > 1)
				Acceleration = 1;
			if (VelocityZ > 1 && (PressQ || Nitro))
				VelocityZ = 1.5;
			else if (VelocityZ > 1 && (!PressQ || !Nitro))
				VelocityZ = 1;
		}
		else
		{
			Driving = false;
			Backwards = false;
			Acceleration = 0;
			VelocityZ = 0;
		}

	}
	if (Driving)
	{
		if (!Application::IsKeyPressed(VK_UP))
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
				Driving = false;
				Backwards = false;
				Acceleration = 0;
				VelocityZ = 0;
			}
		}
	}


	if (Application::IsKeyPressed(VK_RIGHT))
	{
		if (Driving)
			SteeringAngle -= 3;
		if (Backwards)
			SteeringAngle += 3;

	}


	if (Application::IsKeyPressed(VK_LEFT))
	{
		if (Driving)
			SteeringAngle += 3;
		if (Backwards)
			SteeringAngle -= 3;
	}


	if (Application::IsKeyPressed(VK_DOWN) && Driving == false)
	{
		Acceleration -= 0.1;
		VelocityZ += Acceleration * dt;

		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);

		if (!gotCollide(updateX, pos.y, updateZ))
		{
			Backwards = true;
			Driving = false;

			if (Acceleration < -1)
				Acceleration = -1;
			if (VelocityZ < -1 && (PressQ || Nitro))
				VelocityZ = -2;
			else if (VelocityZ < -1 && (!PressQ || !Nitro))
				VelocityZ = -1;
		}
		else
		{
			Driving = false;
			Backwards = false;
			Acceleration = 0;
			VelocityZ = 0;
		}
	}

	if (Backwards)
	{
		if (!Application::IsKeyPressed(VK_DOWN))
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
				Driving = false;
				Backwards = false;
				Acceleration = 0;
				VelocityZ = 0;
			}
		}
	}
}

void c_SecondPlayer::Ability(double dt)
{

}

void c_SecondPlayer::PowerUp(bool check)
{

}