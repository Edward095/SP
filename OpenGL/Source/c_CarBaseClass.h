#ifndef C_CARBASECLASS_H
#define C_CARBASECLASS_H

#include "c_Entity.h"
#include "c_ObjectManager.h"

class c_CarBaseClass : public c_Entity
{
public:
	c_CarBaseClass();
	~c_CarBaseClass();
	bool gotCollide();
	void updateAppearance(const char* meshPath, const char* TGApath);
	virtual void Movement(double dt) = 0;
protected:
	float VelocityZ;
	float SteeringAngle;
	float Acceleration;

	bool Driving;
	bool Backwards;
	c_ObjectManager* objectManager = c_ObjectManager::getInstance();
	
};


#endif

