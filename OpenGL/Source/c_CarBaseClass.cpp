#include "c_CarBaseClass.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Application.h"

c_CarBaseClass::c_CarBaseClass()
{
}
c_CarBaseClass::~c_CarBaseClass()
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
}

float c_CarBaseClass::GetSteeringAngle()
{
	return SteeringAngle;
}
void c_CarBaseClass::updateAppearance(const char* meshPath, const char* TGApath)
{
	this->meshPath = meshPath;
	this->TGApath = TGApath;
	mesh = MeshBuilder::GenerateOBJ("Mesh", meshPath);
	mesh->textureID = LoadTGA(TGApath);
}

void c_CarBaseClass::Movement(double dt)
{
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
				Driving = false;
				Backwards = false;
				Acceleration = 0;
				VelocityZ = 0;
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
				Driving = false;
				Backwards = false;
				Acceleration = 0;
				VelocityZ = 0;
			}
		}
	}
}