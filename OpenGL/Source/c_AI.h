#ifndef C_AI_H
#define C_AI_H

#include "c_CarBaseClass.h"
class c_AI : public c_CarBaseClass
{
public:
	c_AI();
	~c_AI();

	void LevelOne(double dt);
	void LevelTwo(double dt);
	void LevelThree(double dt);
	void MoveOne(double dt, int Points);
	void MoveTwo(double dt, int Points);
	void MoveThree(double dt, int Points);
	void TurnOne(double dt, int dir);
	void TurnTwo(double dt, int dir);
	void TurnThree(double dt, int dir);
	float GetTurning();
	virtual void Ability(double dt);
	virtual void PowerUp(bool check);

protected:
	int Waypoint;
	int Direction;
	int Turning;
	int ToTurn;
};
#endif
