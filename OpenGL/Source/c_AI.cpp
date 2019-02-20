#include "c_AI.h"
#include "Application.h"

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
}

c_AI::~c_AI()
{
}

void c_AI::LevelOne(double dt)
{
	if ((pos.x == -5 && pos.z == 0 && Waypoint == 0) || (pos.x == 2 && pos.z == 0 && Waypoint == 18))
	{
		Waypoint = 1;
		ToTurn = 1;
	}

	if ((pos.x == -5 && pos.z == 218 && Waypoint == 1) || (pos.x == 2 && pos.z == 218 && Waypoint == 1))
		Waypoint = 2;
		if (pos.x == -5 && pos.z == 198)
			ToTurn = 2;
		else if (pos.x == 2 && pos.z == 198)
			ToTurn = 2;

	if (pos.x == -16 && pos.z == 218 && Waypoint == 2)
		Waypoint = 3;

	if (pos.x == -76 && pos.z == 278 && Waypoint == 3)
		Waypoint = 4;
		if (pos.x == -71 && pos.z == 273)
			ToTurn = 4;

	if (pos.x == -605 && pos.z == 278 && Waypoint == 4)
		Waypoint = 5;
		if (pos.x == -585 && pos.z == 278)
			ToTurn = 5;

	if (pos.x == -605 && pos.z == 43 && Waypoint == 5)
		Waypoint = 6;
		if (pos.x == -605 && pos.z == 63)
			ToTurn = 6;

	if (pos.x == -492 && pos.z == 43 && Waypoint == 6)
		Waypoint = 7;
		if (pos.x == -510 && pos.z == 43)
			ToTurn = 7;

	if (pos.x == -453 && pos.z == 4 && Waypoint == 7)
		Waypoint = 8;
		if (pos.x == -474 && pos.z == 25)
			ToTurn = 8;

	if (pos.x == -418 && pos.z == 39 && Waypoint == 8)
		Waypoint = 9;
		if (pos.x == -420 && pos.z == 37)
			ToTurn = 9;

	if (pos.x == -254 && pos.z == 39 && Waypoint == 9)
		Waypoint = 10;
		if (pos.x == -274 && pos.z == 39)
			ToTurn = 10;

	if (pos.x == -254 && pos.z == -225 && Waypoint == 10)
		Waypoint = 11;
		if (pos.x == -254 && pos.z == -214)
			ToTurn = 11;

	if (pos.x == -580 && pos.z == -225 && Waypoint == 11)
		Waypoint = 12;
		if (pos.x == -560 && pos.z == -225)
			ToTurn = 12;

	if (pos.x == -580 && pos.z == -635 && Waypoint == 12)
		Waypoint = 13;
		if (pos.x == -580 && pos.z == -615)
			ToTurn = 13;

	if (pos.x == -364 && pos.z == -635 && Waypoint == 13)
		Waypoint = 14;
		if (pos.x == -384 && pos.z == -635)
			ToTurn = 14;

	if (pos.x == -364 && pos.z == -427 && Waypoint == 14)
		Waypoint = 15;
		if (pos.x == -364 && pos.z == -447)
			ToTurn = 15;

	if (pos.x == -22 && pos.z == -427 && Waypoint == 15)
		Waypoint = 16;
		if (pos.x == -42 && pos.z == -427)
			ToTurn = 16;

	if (pos.x == -22 && pos.z == -220 && Waypoint == 16)
		Waypoint = 17;
		if (pos.x == -22 && pos.z == -252)
			ToTurn = 17;

	if (pos.x == 2 && pos.z == -196 && Waypoint == 17)
		Waypoint = 18;
		if (pos.x == -3 && pos.z == -201)
			ToTurn = 18;

	Move(dt, Waypoint);
	Turn(dt, ToTurn);
}

void c_AI::Move(double dt, int Points)
{
	Acceleration += 0.1;
	VelocityZ += Acceleration;

	if (Acceleration > 1)
		Acceleration = 1;

	if (VelocityZ > 1)
		VelocityZ = 1;

	Waypoint = Points;
	switch (Waypoint)
	{
	case 1:
		pos.z += VelocityZ;
		break;
	case 2:
		pos.x -= VelocityZ;
		break;
	case 3:
		pos.z += VelocityZ;
		pos.x -= VelocityZ;
		break;
	case 4:
		pos.x -= VelocityZ;
		break;
	case 5:
		pos.z -= VelocityZ;
		break;
	case 6:
		pos.x += VelocityZ;
		break;
	case 7:
		pos.x += VelocityZ;
		pos.z -= VelocityZ;
		break;
	case 8:
		pos.x += VelocityZ;
		pos.z += VelocityZ;
		break;
	case 9:
		pos.x += VelocityZ;
		break;
	case 10:
		pos.z -= VelocityZ;
		break;
	case 11:
		pos.x -= VelocityZ;
		break;
	case 12:
		pos.z -= VelocityZ;
		break;
	case 13:
		pos.x += VelocityZ;
		break;
	case 14:
		pos.z += VelocityZ;
		break;
	case 15:
		pos.x += VelocityZ;
		break;
	case 16:
		pos.z += VelocityZ;
		break;
	case 17:
		pos.z += VelocityZ;
		pos.x += VelocityZ;
		break;
	case 18:
		pos.z += VelocityZ;
		break;
	}
}

void c_AI::Turn(double dt, int dir)
{
	ToTurn = dir;
	switch (ToTurn)
	{
	case 1:
		Turning = 0;
		if (Turning <= 0)
		{
			Turning = 0;
		}
		break;
	case 2:
		Turning -= 3;
		if (Turning <= -45)
			Turning = -45;
		break;
	case 4:
		Turning -= 3;
		if (Turning <= -90)
			Turning = -90;
		break;
	case 5:
		Turning -= 3;
		if (Turning <= -180)
			Turning = -180;
		break;
	case 6:
		Turning -= 3;
		if (Turning <= -270)
			Turning = -270;
		break;
	case 7:
		Turning += 3;
		if (Turning >= -225)
			Turning = -225;
		break;
	case 8:
		Turning -= 3;
		if (Turning <= -315)
			Turning = -315;
		break;
	case 9:
		Turning += 3;
		if (Turning >= -270)
			Turning = -270;
		break;
	case 10:
		Turning += 3;
		if (Turning >= -180)
			Turning = -180;
		break;
	case 11:
		Turning += 3;
		if (Turning >= -90)
			Turning = -90;
		break;
	case 12:
		Turning -= 3;
		if (Turning <= -180)
			Turning = -180;
		break;
	case 13:
		Turning -= 3;
		if (Turning <= -270)
			Turning = -270;
		break;
	case 14:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	case 15:
		Turning += 3;
		if (Turning >= -270)
			Turning = -270;
		break;
	case 16:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	case 17:
		Turning += 3;
		if (Turning >= -315)
			Turning = -315;
		break;
	case 18:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
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