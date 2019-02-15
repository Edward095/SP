#ifndef C_CARBASECLASS_H
#define C_CARBASECLASS_H

#include "c_Entity.h"

class c_CarBaseClass : public c_Entity
{
public:
	c_CarBaseClass();
	~c_CarBaseClass();
	void updateAppearance(const char* meshPath, const char* TGApath);
	virtual void Movement(double dt) = 0;
	virtual float GetSteeringAngle();
	virtual void Ability(double dt) = 0;
	virtual void PowerUp(bool check) = 0;

protected:
	float VelocityZ;
	float MaxSpeed;
	float SteeringAngle;
	float Acceleration;
	int Duration;

	bool Driving = false;
	bool Backwards = false;
	bool PressQ = false;
	bool Nitro = false;

	bool frontCollide;
	bool backCollide;
};


#endif

