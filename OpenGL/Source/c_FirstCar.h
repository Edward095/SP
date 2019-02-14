#ifndef C_FIRSTCAR_H
#define C_FIRSTCAR_H

#include "c_CarBaseClass.h"

class c_FirstCar : public c_CarBaseClass
{
public:
	c_FirstCar();
	c_FirstCar(const char* fileName, Vector3 pos);
	~c_FirstCar();
	virtual void Movement(double dt);
};


#endif
