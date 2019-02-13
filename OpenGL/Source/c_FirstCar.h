#ifndef C_FIRSTCAR_H
#define C_FIRSTCAR_H

#include "c_CarBaseClass.h"

class c_FirstCar : public c_CarBaseClass
{
public:
	c_FirstCar();
	~c_FirstCar();
	virtual void Movement(double dt);
	virtual float GetVelocityZ();
	virtual void F_PowerUp(bool check);
	
};


#endif
