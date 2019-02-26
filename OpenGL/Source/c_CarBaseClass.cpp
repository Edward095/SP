#include "c_CarBaseClass.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Application.h"
#include "c_ObjectManager.h"



c_CarBaseClass::c_CarBaseClass()
{

	MaxSpeed = 1;
	SteeringAngle = 0;
	Steering = 2;
	Duration = 0;
	MaxAcceleration = 1;
	Friction = 0.5;
	padDuration = 0;

	VelocityZ = 0;
	Acceleration = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;
	Driving = false;
	Backwards = false;
	PressQ = false;
	Nitro = false;
	BoostPad = false;
	SlowPad = false;
	once = false;
	Oslowed = false;
	Tslowed = false;
	SpedoVeloZ = 0;
}
c_CarBaseClass::~c_CarBaseClass()
{

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
	//Ability(dt);
	PadEffect(dt);
	isOffRoad();
	if (uniqueName == "player2")
	{
		if (Application::IsKeyPressed(VK_UP) && Backwards == false)
		{
			Acceleration += (MaxAcceleration - Friction);
			VelocityZ += Acceleration * (float)dt;

			float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			//OBB->calcNewAxis(SteeringAngle, 0, 1, 0);
			if (gotCollide("Boostpad", false) || gotCollide("Boostpad2", false) || gotCollide("Boostpad3", false) || gotCollide("Boostpad4", false) || gotCollide("Boostpad5", false) || gotCollide("Boostpad6", false) || gotCollide("Boostpad7", false))
				BoostPad = true;
			if (gotCollide("Slowpad", false) || gotCollide("Slowpad2", false) || gotCollide("Slowpad3", false) || gotCollide("Slowpad4", false) || gotCollide("Slowpad5", false) || gotCollide("Slowpad6", false) || gotCollide("Slowpad7", false))
				SlowPad = true;


			if (!gotCollide(updateX, pos.y, updateZ))
			{
				Driving = true;
				Backwards = false;
				if (Acceleration > MaxAcceleration - Friction)
					Acceleration = MaxAcceleration - Friction;
				if (VelocityZ > MaxSpeed && (!PressQ))
					//VelocityZ -= 0.5;
					VelocityZ = MaxSpeed;
				if (VelocityZ < MaxSpeed && (!PressQ))
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
			if (!Application::IsKeyPressed(VK_UP))
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
						VelocityZ -= Friction;
					}
					if (VelocityZ < 0)
					{
						VelocityZ = 0;
						//Acceleration = 0;
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
				SteeringAngle -= Steering;
			if (Backwards)
				SteeringAngle += Steering;

		}


		if (Application::IsKeyPressed(VK_LEFT))
		{
			if (Driving)
				SteeringAngle += Steering;
			if (Backwards)
				SteeringAngle -= Steering;
		}


		if (Application::IsKeyPressed(VK_DOWN) && Driving == false)
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
			if (!Application::IsKeyPressed(VK_DOWN))
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
						VelocityZ += Friction;
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
	else
	{
		if (Application::IsKeyPressed('W') && Backwards == false)

		{
			//Ability(dt);
			Acceleration += (MaxAcceleration - Friction);
			VelocityZ += Acceleration * (float)dt;

			float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
			//OBB->calcNewAxis(SteeringAngle, 0, 1, 0);
			if (gotCollide("Boostpad",false) || gotCollide("Boostpad2", false) || gotCollide("Boostpad3", false) || gotCollide("Boostpad4", false) || gotCollide("Boostpad5", false) || gotCollide("Boostpad6", false) || gotCollide("Boostpad7", false))
			BoostPad = true;
			if (gotCollide("Slowpad", false) || gotCollide("Slowpad2", false) || gotCollide("Slowpad3", false) || gotCollide("Slowpad4", false) || gotCollide("Slowpad5", false) || gotCollide("Slowpad6", false) || gotCollide("Slowpad7", false))
			SlowPad = true;

			if (!gotCollide(updateX, pos.y, updateZ))
			{
				Driving = true;
				Backwards = false;
				//Collided = false;
				if (Acceleration > MaxAcceleration - Friction)
					Acceleration = MaxAcceleration - Friction;
				//if (VelocityZ > MaxSpeed && (PressQ))
					//VelocityZ = 1.5; 
				if (VelocityZ > MaxSpeed && (!PressQ))
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
				Collided = true;
				if (Collided == true)
				{
					Acceleration = 0;
					VelocityZ = -VelocityZ / 2;
				}
			}
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
					Collided = false;
					if (Acceleration < 0)
					{
						Acceleration = 0;
						VelocityZ -= Friction;
					}
					if (VelocityZ < 0 && Collided == false)
					{
						VelocityZ = 0;
						//Acceleration = 0;
						Driving = false;
						Backwards = false;
					}

				}
				else
				{
					Driving = false;
					Backwards = false;

					//Collided = true;
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
						VelocityZ += Friction;
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

float c_CarBaseClass::GetMaxAcceleration()
{
	return MaxAcceleration;
}

float c_CarBaseClass::GetAcceleration()
{
	return Acceleration;
}

float c_CarBaseClass::GetSpeed()
{
	return VelocityZ;
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
	c_Sound* Audio = c_Sound::getInstance();
	if (BoostPad)
	{
		if (padDuration == 0)
		{
			Audio->f_Game_Fanfare_Boost();
		}
		padDuration++;
		if (padDuration >= 50) // 3 sec/dt
		{
			BoostPad = false;
			padDuration = 0;
		}
	}
	if (SlowPad)
	{
		if (padDuration == 0)
		{
			Audio->f_Game_Fanfare_Slow();
		}
		padDuration++;
		if (padDuration >= 25) // 3 sec/dt
		{
			SlowPad = false;
			padDuration = 0;
		}
	}
}


void c_CarBaseClass::SetSpeed(float speed)
{
	this->VelocityZ = speed;
}

void c_CarBaseClass::PSpeed(float speed)
{
	this->VelocityZ = speed;
}

void c_CarBaseClass::SetOSlowed(bool speed)
{
	Oslowed = speed;
}

void c_CarBaseClass::SetTSlowed(bool speed)
{
	Tslowed = speed;
}

float c_CarBaseClass::GetSpedoSpeed()
{
	SpedoVeloZ = (VelocityZ * 180);
	if (VelocityZ <= 0)
		SpedoVeloZ = 0;

	return SpedoVeloZ;
}
void c_CarBaseClass::SetSteeringAngle(float angle)
{
	this->SteeringAngle = angle;
}