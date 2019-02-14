#ifndef C_CARBASECLASS_H
#define C_CARBASECLASS_H


#include "c_Collision.h"
#include  "Vector3.h"

class c_CarBaseClass
{
public:
	c_CarBaseClass();
	~c_CarBaseClass();
	virtual void Movement(double dt) = 0;

	virtual void F_PowerUp(bool check) = 0;

	virtual Position GetPos();
protected:
	float VelocityZ;
	float SteeringAngle;
	float Acceleration;

	bool Driving;
	bool Backwards;

	c_Collision collision;
	Position pos;
	
};


#endif

