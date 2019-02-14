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

protected:
	float VelocityZ;
	float SteeringAngle;
	float Acceleration;

	bool Driving;
	bool Backwards;
	
};


#endif

