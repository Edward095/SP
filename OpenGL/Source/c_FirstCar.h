#ifndef C_FIRSTCAR_H
#define C_FIRSTCAR_H

#include "c_CarBaseClass.h"

class c_FirstCar : public c_CarBaseClass
{
public:
	c_FirstCar();
	~c_FirstCar();
	virtual void Movement(double dt);

	virtual float GetPosX();
	virtual float GetPosY();
	virtual float GetPosZ();

	
};


#endif
