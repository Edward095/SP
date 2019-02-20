#include "c_CarBaseClass.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Application.h"
#include "c_ObjectManager.h"

c_ObjectManager* OBJmanager = c_ObjectManager::getInstance();

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

	MaxSpeed = 1;
	SteeringAngle = 0;
	Steering = 2;
	Duration = 0;
	MaxAcceleration = 1;
	Friction = 0.5;
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
	PadEffect(dt);
	if (Application::IsKeyPressed('W') && Backwards == false)
	{
		Acceleration += (MaxAcceleration - Friction);
		VelocityZ += Acceleration * (float)dt;
		
		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		//OBB->calcNewAxis(SteeringAngle, 0, 1, 0);
		if (gotCollide("Boostpad"))
			BoostPad = true;
		if (gotCollide("Slowpad"))
			SlowPad = true;

		if (gotCollide("player2"))
		{
			//OBJmanager->getObjects()
		}


		if (!gotCollide(updateX, pos.y, updateZ))
		{
			Driving = true;
			Backwards = false;
			if (Acceleration > MaxAcceleration - Friction)
				Acceleration = MaxAcceleration - Friction;
			if (VelocityZ > MaxSpeed && (PressQ))
				VelocityZ = 1.5; 
			else if (VelocityZ > MaxSpeed && (!PressQ || !Nitro))
				//VelocityZ -= 0.5;
				VelocityZ = MaxSpeed;
			if (BoostPad)
				VelocityZ = 1.8f;
			if (SlowPad)
				VelocityZ = 0.5f; 

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
			Acceleration += -Friction;
			VelocityZ -= Acceleration * (float)dt;

			float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			OBB->calcNewAxis(SteeringAngle, 0, 1, 0);
			if (!gotCollide(updateX, pos.y, updateZ))
			{
				if (Acceleration < 0)
				{
					Acceleration = 0;
					VelocityZ -= 0.05f;
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
			SteeringAngle -= Steering;
		if (Backwards)
			SteeringAngle += Steering;

	}


	if (Application::IsKeyPressed('A'))
	{
		if (Driving)
			SteeringAngle += Steering;
		if (Backwards)
			SteeringAngle -= Steering;
	}


	if (Application::IsKeyPressed('S') && Driving == false)
	{
		Acceleration -= (MaxAcceleration - Friction);
		VelocityZ += Acceleration * (float)dt;

		float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
		OBB->calcNewAxis(SteeringAngle, 0, 1, 0);

		if (!gotCollide(updateX, pos.y, updateZ))
		{
			Backwards = true;
			Driving = false;
			
			if (Acceleration < -(MaxAcceleration - Friction))
				Acceleration = -(MaxAcceleration - Friction);
			if (VelocityZ < -MaxSpeed && (PressQ))
				VelocityZ = -2;
			else if (VelocityZ < -MaxSpeed && (!PressQ))
				VelocityZ = -MaxSpeed;
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
			Acceleration += Friction;
			VelocityZ -= Acceleration * (float)dt;

			float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			//OBB->calcNewAxis(SteeringAngle, 0, 1, 0);

			if (!gotCollide(updateX, pos.y, updateZ))
			{
				if (Acceleration > 0)
				{
					Acceleration = 0;
					VelocityZ += 0.05f;
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



float c_CarBaseClass::GetSpeed()
{
	return VelocityZ;
}

float c_CarBaseClass::GetMaxAcceleration()
{
	return MaxAcceleration;
}

float c_CarBaseClass::GetAcceleration()
{
	return Acceleration;
}


void c_CarBaseClass::SetFriction(float friction)
{
	this->Friction = friction;
}

void c_CarBaseClass::SetSteering(float Steering)
{
	this->Steering = Steering;
}

void c_CarBaseClass::SetMaxSpeed(float Speed)
{
	this->MaxSpeed = Speed;
}
void c_CarBaseClass::PadEffect(double dt)
{
	if (BoostPad)
	{
		Duration++;
		if (Duration >= 25) // 3 sec/dt
		{
			BoostPad = false;
			Duration = 0;
		}
	}
	if (SlowPad)
	{
		Duration++;
		if (Duration >= 25) // 3 sec/dt
		{
			SlowPad = false;
			Duration = 0;
		}
	}
}


void c_CarBaseClass::SetSpeed(float speed)
{
	this->VelocityZ = speed;
}