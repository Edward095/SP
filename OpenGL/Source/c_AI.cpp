#include "c_AI.h"
#include "Application.h"

c_AI::c_AI()
{
	Waypoint = 0;
	Direction = 0;
	Turning = 0;
	ToTurn = 0;
	VelocityZ = 0;
	Acceleration = 0;
	offRoad = false;
}

c_AI::~c_AI()
{
}

void c_AI::LevelOne(double dt)
{
	if ((pos.x == -15 && pos.z == 0) && (Waypoint == 0 || Waypoint == 18))
	{
		Waypoint = 1;
		ToTurn = 1;
	}

	if (pos.x == -15 && pos.z == 317 && Waypoint == 1)
		Waypoint = 2;
		if (pos.x == -15 && pos.z == 307)
			ToTurn = 2;

	if (pos.x == -616 && pos.z == 317 && Waypoint == 2)
		Waypoint = 3;
		if (pos.x == -610 && pos.z == 317)
			ToTurn = 3;

	if (pos.x == -616 && pos.z == 114 && Waypoint == 3)
		Waypoint = 4;
		if (pos.x == -616 && pos.z == 124)
			ToTurn = 4;

	if (pos.x == -530 && pos.z == 114 && Waypoint == 4)
		Waypoint = 5;
		if (pos.x == -540 && pos.z == 114)
			ToTurn = 5;

	if (pos.x == -470 && pos.z == 54 && Waypoint == 5)
		Waypoint = 6;
		if (pos.x == -475 && pos.z == 59)
			ToTurn = 6;

	if (pos.x == -431 && pos.z == 93 && Waypoint == 6)
		Waypoint = 7;
		if (pos.x == -436 && pos.z == 88)
			ToTurn = 7;

	if (pos.x == -270 && pos.z == 93 && Waypoint == 7)
		Waypoint = 8;
		if (pos.x == -280 && pos.z == 93)
			ToTurn = 8;

	if (pos.x == -270 && pos.z == -167 && Waypoint == 8)
		Waypoint = 9;
		if (pos.x == -270 && pos.z == -157)
			ToTurn = 9;

	if (pos.x == -590 && pos.z == -167 && Waypoint == 9)
		Waypoint = 10;
		if (pos.x == -580 && pos.z == -167)
			ToTurn = 10;

	if (pos.x == -590 && pos.z == -580 && Waypoint == 10)
		Waypoint = 11;
		if (pos.x == -590 && pos.z == -570)
			ToTurn = 11;

	if (pos.x == -382 && pos.z == -580 && Waypoint == 11)
		Waypoint = 12;
		if (pos.x == -387 && pos.z == -580)
			ToTurn = 12;

	if (pos.x == -382 && pos.z == -381 && Waypoint == 12)
		Waypoint = 13;
		if (pos.x == -382 && pos.z == -392)
			ToTurn = 13;

	if (pos.x == -77 && pos.z == -381 && Waypoint == 13)
		Waypoint = 14;
		if (pos.x == -94 && pos.z == -381)
			ToTurn = 14;

	if (pos.x == -28 && pos.z == -332 && Waypoint == 14)
		Waypoint = 15;
		if (pos.x == -33 && pos.z == -337)
			ToTurn = 15;

	if (pos.x == -28 && pos.z == -218 && Waypoint == 15)
		Waypoint = 16;
		if (pos.x == -28 && pos.z == -228)
			ToTurn = 16;

	if (pos.x == -59 && pos.z == -187 && Waypoint == 16)
		Waypoint = 17;
		if (pos.x == -55 && pos.z == -191)
			ToTurn = 17;

	if (pos.x == -15 && pos.z == -143 && Waypoint == 17)
		Waypoint = 18;
		if (pos.x == -20 && pos.z == -148)
			ToTurn = 18;

	MoveOne(dt, Waypoint);
	TurnOne(dt, ToTurn);
}

void c_AI::MoveOne(double dt, int Points)
{
	Acceleration += 0.1f;
	VelocityZ += Acceleration;

	if (Acceleration > 1)
		Acceleration = 3;

	if (VelocityZ > 1)
		VelocityZ = 1;

	if (freeze)
		VelocityZ = 0;

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
		pos.z -= VelocityZ;
		break;
	case 4:
		pos.x += VelocityZ;
		break;
	case 5:
		pos.x += VelocityZ;
		pos.z -= VelocityZ;
		break;
	case 6:
		pos.x += VelocityZ;
		pos.z += VelocityZ;
		break;
	case 7:
		pos.x += VelocityZ;
		break;
	case 8:
		pos.z -= VelocityZ;
		break;
	case 9:
		pos.x -= VelocityZ;
		break;
	case 10:
		pos.z -= VelocityZ;
		break;
	case 11:
		pos.x += VelocityZ;
		break;
	case 12:
		pos.z += VelocityZ;
		break;
	case 13:
		pos.x += VelocityZ;
		break;
	case 14:
		pos.z += VelocityZ;
		pos.x += VelocityZ;
		break;
	case 15:
		pos.z += VelocityZ;
		break;
	case 16:
		pos.z += VelocityZ;
		pos.x -= VelocityZ;
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

void c_AI::TurnOne(double dt, int dir)
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
		if (Turning <= -90)
			Turning = -90;
		break;
	case 3:
		Turning -= 3;
		if (Turning <= -180)
			Turning = -180;
		break;
	case 4:
		Turning -= 3;
		if (Turning <= -270)
			Turning = -270;
		break;
	case 5:
		Turning += 3;
		if (Turning >= -225)
			Turning = -225;
		break;
	case 6:
		Turning -= 3;
		if (Turning <= -315)
			Turning = -315;
		break;
	case 7:
		Turning += 3;
		if (Turning >= -270)
			Turning = -270;
		break;
	case 8:
		Turning += 3;
		if (Turning >= -180)
			Turning = -180;
		break;
	case 9:
		Turning += 3;
		if (Turning >= -90)
			Turning = -90;
		break;
	case 10:
		Turning -= 3;
		if (Turning <= -180)
			Turning = -180;
		break;
	case 11:
		Turning -= 3;
		if (Turning <= -270)
			Turning = -270;
		break;
	case 12:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	case 13:
		Turning += 3;
		if (Turning >= -270)
			Turning = -270;
		break;
	case 14:
		Turning -= 3;
		if (Turning <= -315)
			Turning = -315;
		break;
	case 15:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	case 16:
		Turning -= 3;
		if (Turning <= -405)
			Turning = -405;
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

void c_AI::LevelTwo(double dt)
{
	if ((pos.x == -15 && pos.z == 0) && (Waypoint == 0 || Waypoint == 23))
	{
		Waypoint = 1;
		ToTurn = 1;
	}

	if (pos.x == -15 && pos.z == 230 && Waypoint == 1)
		Waypoint = 2;
		if (pos.x == -15 && pos.z == 220)
			ToTurn = 2;

	if (pos.x == -280 && pos.z == 230 && Waypoint == 2)
		Waypoint = 3;
		if (pos.x == -270 && pos.z == 230)
			ToTurn = 3;

	if (pos.x == -309 && pos.z == 201 && Waypoint == 3)
		Waypoint = 4;
		if (pos.x == -297 && pos.z == 213)
			ToTurn = 4;

	if (pos.x == -353 && pos.z == 245 && Waypoint == 4)
		Waypoint = 5;
		if (pos.x == -340 && pos.z == 232)
			ToTurn = 5;

	if (pos.x == -363 && pos.z == 235 && Waypoint == 5)
		Waypoint = 6;
		if (pos.x == -361 && pos.z == 237)
			ToTurn = 6;

	if (pos.x == -445 && pos.z == 235 && Waypoint == 6)
		Waypoint = 7;
		if (pos.x == -427 && pos.z == 235)
			ToTurn = 7;

	if (pos.x == -486 && pos.z == 276 && Waypoint == 7)
		Waypoint = 8;
		if (pos.x == -476 && pos.z == 266)
			ToTurn = 8;

	if (pos.x == -486 && pos.z == 323 && Waypoint == 8)
		Waypoint = 9;
		if (pos.x == -486 && pos.z == 313)
			ToTurn = 9;

	if (pos.x == -616 && pos.z == 323 && Waypoint == 9)
		Waypoint = 10;
		if (pos.x == -606 && pos.z == 323)
			ToTurn = 10;

	if (pos.x == -616 && pos.z == 100 && Waypoint == 10)
		Waypoint = 11;
		if (pos.x == -616 && pos.z == 110)
			ToTurn = 11;


	if (pos.x == -320 && pos.z == 100 && Waypoint == 11)
		Waypoint = 12;
		if (pos.x == -330 && pos.z == 100)
			ToTurn = 12;

	if (pos.x == -270 && pos.z == 50 && Waypoint == 12)
		Waypoint = 13;
		if (pos.x == -277 && pos.z == 57)
			ToTurn = 13;

	if (pos.x == -270 && pos.z == -167 && Waypoint == 13)
		Waypoint = 14;
		if (pos.x == -270 && pos.z == -157)
			ToTurn = 14;

	if (pos.x == -590 && pos.z == -167 && Waypoint == 14)
		Waypoint = 15;
		if (pos.x == -580 && pos.z == -167)
			ToTurn = 15;

	if (pos.x == -590 && pos.z == -580 && Waypoint == 15)
		Waypoint = 16;
		if (pos.x == -590 && pos.z == -570)
			ToTurn = 16;

	if (pos.x == -382 && pos.z == -580 && Waypoint == 16)
		Waypoint = 17;
		if (pos.x == -387 && pos.z == -580)
			ToTurn = 17;

	if (pos.x == -382 && pos.z == -381 && Waypoint == 17)
		Waypoint = 18;
		if (pos.x == -382 && pos.z == -392)
			ToTurn = 18;

	if (pos.x == -77 && pos.z == -381 && Waypoint == 18)
		Waypoint = 19;
		if (pos.x == -94 && pos.z == -381)
			ToTurn = 19;

	if (pos.x == -28 && pos.z == -332 && Waypoint == 19)
		Waypoint = 20;
		if (pos.x == -33 && pos.z == -337)
			ToTurn = 20;

	if (pos.x == -28 && pos.z == -218 && Waypoint == 20)
		Waypoint = 21;
		if (pos.x == -28 && pos.z == -228)
			ToTurn = 21;

	if (pos.x == -59 && pos.z == -187 && Waypoint == 21)
		Waypoint = 22;
		if (pos.x == -55 && pos.z == -191)
			ToTurn = 22;

	if (pos.x == -15 && pos.z == -143 && Waypoint == 22)
		Waypoint = 23;
		if (pos.x == -20 && pos.z == -148)
			ToTurn = 23;

	MoveTwo(dt, Waypoint);
	TurnTwo(dt, ToTurn);
}

void c_AI::MoveTwo(double dt, int Points)
{
	Acceleration += 0.1f;
	VelocityZ += Acceleration;

	if (Acceleration > 1)
		Acceleration = 1;

	if (VelocityZ > 1)
		VelocityZ = 1;

	if (freeze)
		VelocityZ = 0;

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
		pos.x -= VelocityZ;
		pos.z -= VelocityZ;
		break;
	case 4:
		pos.x -= VelocityZ;
		pos.z += VelocityZ;
		break;
	case 5:
		pos.x -= VelocityZ;
		pos.z -= VelocityZ;
		break;
	case 6:
		pos.x -= VelocityZ;
		break;
	case 7:
		pos.x -= VelocityZ;
		pos.z += VelocityZ;
		break;
	case 8:
		pos.z += VelocityZ;
		break;
	case 9:
		pos.x -= VelocityZ;
		break;
	case 10:
		pos.z -= VelocityZ;
		break;
	case 11:
		pos.x += VelocityZ;
		break;
	case 12:
		pos.x += VelocityZ;
		pos.z -= VelocityZ;
		break;
	case 13:
		pos.z -= VelocityZ;
		break;
	case 14:
		pos.x -= VelocityZ;
		break;
	case 15:
		pos.z -= VelocityZ;
		break;
	case 16:
		pos.x += VelocityZ;
		break;
	case 17:
		pos.z += VelocityZ;
		break;
	case 18:
		pos.x += VelocityZ;
		break;
	case 19:
		pos.z += VelocityZ;
		pos.x += VelocityZ;
		break;
	case 20:
		pos.z += VelocityZ;
		break;
	case 21:
		pos.z += VelocityZ;
		pos.x -= VelocityZ;
		break;
	case 22:
		pos.z += VelocityZ;
		pos.x += VelocityZ;
		break;
	case 23:
		pos.z += VelocityZ;
		break;
	}
}

void c_AI::TurnTwo(double dt, int dir)
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
		if (Turning <= -90)
			Turning = -90;
		break;
	case 3:
		Turning -= 3;
		if (Turning <= -135)
			Turning = -135;
		break;
	case 4:
		Turning += 3;
		if (Turning >= -45)
			Turning = -45;
		break;
	case 5:
		Turning -= 3;
		if (Turning <= -135)
			Turning = -135;
		break;
	case 6:
		Turning += 3;
		if (Turning >= -90)
			Turning = -90;
		break;
	case 7:
		Turning += 3;
		if (Turning >= -45)
			Turning = -45;
		break;
	case 8:
		Turning += 3;
		if (Turning >= 0)
			Turning = 0;
		break;
	case 9:
		Turning -= 3;
		if (Turning <= -90)
			Turning = -90;
		break;
	case 10:
		Turning -= 3;
		if (Turning <= -180)
			Turning = -180;
		break;
	case 11:
		Turning -= 3;
		if (Turning <= -270)
			Turning = -270;
		break;
	case 12:
		Turning += 3;
		if (Turning >= -225)
			Turning = -225;
		break;
	case 13:
		Turning += 3;
		if (Turning >= -180)
			Turning = -180;
		break;
	case 14:
		Turning += 3;
		if (Turning >= -90)
			Turning = -90;
		break;
	case 15:
		Turning -= 3;
		if (Turning <= -180)
			Turning = -180;
		break;
	case 16:
		Turning -= 3;
		if (Turning <= -270)
			Turning = -270;
		break;
	case 17:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	case 18:
		Turning += 3;
		if (Turning >= -270)
			Turning = -270;
		break;
	case 19:
		Turning -= 3;
		if (Turning <= -315)
			Turning = -315;
		break;
	case 20:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	case 21:
		Turning -= 3;
		if (Turning <= -405)
			Turning = -405;
		break;
	case 22:
		Turning += 3;
		if (Turning >= -315)
			Turning = -315;
		break;
	case 23:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	}
}

void c_AI::LevelThree(double dt)
{
	if ((pos.x == -15 && pos.z == 0) && (Waypoint == 0 || Waypoint == 12))
	{
		Waypoint = 1;
		ToTurn = 1;
	}

	if (pos.x == -15 && pos.z == 314 && Waypoint == 1)
		Waypoint = 2;
		if (pos.x == -15 && pos.z == 304)
			ToTurn = 2;

	if (pos.x == -610 && pos.z == 314 && Waypoint == 2)
		Waypoint = 3;
		if (pos.x == -600 && pos.z == 314)
			ToTurn = 3;

	if (pos.x == -610 && pos.z == -550 && Waypoint == 3)
		Waypoint = 4;
		if (pos.x == -610 && pos.z == -540)
			ToTurn = 4;

	if (pos.x == -584 && pos.z == -576 && Waypoint == 4)
		Waypoint = 5;
		if (pos.x == -590 && pos.z == -570)
			ToTurn = 5;

	if (pos.x == -381 && pos.z == -576 && Waypoint == 5)
		Waypoint = 6;
		if (pos.x == -391 && pos.z == -576)
			ToTurn = 6;

	if (pos.x == -381 && pos.z == -381 && Waypoint == 6)
		Waypoint = 7;
		if (pos.x == -381 && pos.z == -392)
			ToTurn = 7;

	if (pos.x == -77 && pos.z == -381 && Waypoint == 7)
		Waypoint = 8;
		if (pos.x == -94 && pos.z == -381)
			ToTurn = 8;

	if (pos.x == -28 && pos.z == -332 && Waypoint == 8)
		Waypoint = 9;
		if (pos.x == -33 && pos.z == -337)
			ToTurn = 9;

	if (pos.x == -28 && pos.z == -218 && Waypoint == 9)
		Waypoint = 10;
		if (pos.x == -28 && pos.z == -228)
			ToTurn = 10;

	if (pos.x == -59 && pos.z == -187 && Waypoint == 10)
		Waypoint = 11;
		if (pos.x == -55 && pos.z == -191)
			ToTurn = 11;

	if (pos.x == -15 && pos.z == -143 && Waypoint == 11)
		Waypoint = 12;
		if (pos.x == -20 && pos.z == -148)
			ToTurn = 12;

	MoveThree(dt, Waypoint);
	TurnThree(dt, ToTurn);
}

void c_AI::MoveThree(double dt, int Points)
{
	Acceleration += 0.1f;
	VelocityZ += Acceleration;

	if (Acceleration > 1)
		Acceleration = 1;

	if (VelocityZ > 1)
		VelocityZ = 1;

	if (freeze)
		VelocityZ = 0;

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
		pos.z -= VelocityZ;
		break;
	case 4:
		pos.z -= VelocityZ;
		pos.x += VelocityZ;
		break;
	case 5:
		pos.x += VelocityZ;
		break;
	case 6:
		pos.z += VelocityZ;
		break;
	case 7:
		pos.x += VelocityZ;
		break;
	case 8:
		pos.z += VelocityZ;
		pos.x += VelocityZ;
		break;
	case 9:
		pos.z += VelocityZ;
		break;
	case 10:
		pos.z += VelocityZ;
		pos.x -= VelocityZ;
		break;
	case 11:
		pos.z += VelocityZ;
		pos.x += VelocityZ;
		break;
	case 12:
		pos.z += VelocityZ;
		break;
	}
}
void c_AI::TurnThree(double dt, int dir)
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
		if (Turning <= -90)
			Turning = -90;
		break;
	case 3:
		Turning -= 3;
		if (Turning <= -180)
			Turning = -180;
		break;
	case 4:
		Turning -= 3;
		if (Turning <= -225)
			Turning = -225;
		break;
	case 5:
		Turning -= 3;
		if (Turning <= -270)
			Turning = -270;
		break;
	case 6:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	case 7:
		Turning += 3;
		if (Turning >= -270)
			Turning = -270;
		break;
	case 8:
		Turning -= 3;
		if (Turning <= -315)
			Turning = -315;
		break;
	case 9:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	case 10:
		Turning -= 3;
		if (Turning <= -405)
			Turning = -405;
		break;
	case 11:
		Turning += 3;
		if (Turning >= -315)
			Turning = -315;
		break;
	case 12:
		Turning -= 3;
		if (Turning <= -360)
			Turning = -360;
		break;
	}
}

float c_AI::GetTurning()
{
	return (float)Turning;
}

void c_AI::Ability(double dt)
{
}

void c_AI::isOffRoad()
{
}

void c_AI::Speed(int speed)
{
	if (speed == 0)
		freeze = true;
	else
		freeze = false;
}

void c_AI::resetVar()
{
}
