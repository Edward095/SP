#include "c_AI.h"

c_AI::c_AI()
{
	Waypoint = 0;
	Direction = 0;
	Turning = 0;
	ToTurn = 0;
	pos.x = 0;
	pos.y = 1;
	pos.z = 0;
	VelocityZ = 0;
	Acceleration = 0;
	SteeringAngle = 0;
}


c_AI::~c_AI()
{
}

void c_AI::Movement(double dt)
{
	if (pos.x == 0 && pos.z == 0 && Waypoint == 0)
	{
		Waypoint = 1;
		ToTurn = 1;
	}

	if (pos.x == -240 && pos.z == 0 && Waypoint == 1)
		Waypoint = 2;

		if (pos.x == -180 && pos.z == 0)
			ToTurn = 2;

	if (pos.x == -240 && pos.z == -600 && Waypoint == 2)
		Waypoint = 3;

		if (pos.x == -240 && pos.z == -540)
			ToTurn = 3;

	if (pos.x == -30 && pos.z == -600 && Waypoint == 3)
		Waypoint = 4;
	
	if (pos.x == -30 && pos.z == -250 && Waypoint == 4)
		Waypoint = 5;

	if (pos.x == 210 && pos.z == -250 && Waypoint == 5)
		Waypoint = 6;

	if (pos.x == 210 && pos.z == -600 && Waypoint == 6)
		Waypoint = 7;

	if (pos.x == 650 && pos.z == -600 && Waypoint == 7)
		Waypoint = 8;

	if (pos.x == 650 && pos.z == -350 && Waypoint == 8)
		Waypoint = 9;

	if (pos.x == 430 && pos.z == -350 && Waypoint == 9)
		Waypoint = 10;

	if (pos.x == 430 && pos.z == 0 && Waypoint == 10)
		Waypoint = 11;

	if (pos.x == 0 && pos.z == 0 && Waypoint == 11)
		Waypoint = 1;

	Move(dt, Waypoint);
	Turn(dt, ToTurn);
}

void c_AI::Move(double dt, int Points)
{
	Acceleration += 0.1f;
	VelocityZ += Acceleration;

	float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
	float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);

	if (Acceleration > 1)
		Acceleration = 1;

	if (VelocityZ > 1)
		VelocityZ = 1;

	Waypoint = Points;
	/*switch (Waypoint)
	{
	case 1:
		pos.x -= updateX;
		break;
	case 2:
		pos.z -= updateZ;
		break;
	case 3:
		pos.x += updateX;
		break;
	case 4:
		pos.z += VelocityZ;
		break;
	case 5:
		pos.x += VelocityZ;
		break;
	case 6:
		pos.z -= VelocityZ;
		break;
	case 7:
		pos.x += VelocityZ;
		break;
	case 8:
		pos.z += VelocityZ;
		break;
	case 9:
		pos.x -= VelocityZ;
		break;
	case 10:
		pos.z += VelocityZ;
		break;
	case 11:
		pos.x -= VelocityZ;
		break;
	default:
		break;
	}*/

	switch (Waypoint)
	{
	case 1:
		pos.x -= updateZ;
		break;
	case 2:
		pos.z -= updateX;
		break;
	case 3:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	case 4:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	case 5:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	case 6:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	case 7:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	case 8:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	case 9:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	case 10:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	case 11:
		pos.x += updateX;
		pos.z += updateZ;
		break;
	default:
		break;
	}
}

void c_AI::Turn(double dt, int dir)
{

	float updateX = (sin(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);
	float updateZ = (cos(Math::DegreeToRadian(SteeringAngle)) * VelocityZ);


	ToTurn = dir;
	switch (ToTurn)
	{
	case 1:
		//SteeringAngle += 1;
		break;
	case 2:
		SteeringAngle -= 90;
		pos.z += updateX;
		break;
	case 3:
		SteeringAngle += 90;
		pos.x += updateZ;
		break;
	case 4:
		//pos.z += 1;
		break;
	case 5:
		//pos.x += 1;
		break;
	case 6:
		//pos.z -= 1;
		break;
	case 7:
		//pos.x += 1;
		break;
	case 8:
		//pos.z += 1;
		break;
	case 9:
		//pos.x -= 1;
		break;
	case 10:
		//pos.z += 1;
		break;
	case 11:
		//pos.x -= 1;
		break;
	default:
		break;
	}
}

float c_AI::GetTurning()
{
	return Turning;
}

void c_AI::Ability(double dt)
{
}

void c_AI::PowerUp(bool check)
{
}