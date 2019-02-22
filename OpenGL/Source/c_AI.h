#pragma once
//#include "c_Entity.h"
#include "c_CarBaseClass.h"
class c_AI : public c_CarBaseClass
{
public:
	c_AI();
	~c_AI();

	//void Points(double dt);
	virtual void Movement(double dt);
	virtual void Ability(double dt);
	virtual void PowerUp(bool check);
	virtual void isOffRoad();

	void Move(double dt, int Points);
	void Turn(double dt, int dir);
	float GetTurning();

protected:
	int Waypoint;
	int Direction;
	int Turning;
	int ToTurn;
};

