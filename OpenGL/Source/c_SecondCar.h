#ifndef C_SECONDCAR_H
#define C_SECONDCAR_H

#include "c_CarBaseClass.h"
class c_SecondCar :	public c_CarBaseClass
{
public:
	c_SecondCar();
	c_SecondCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos, bool canCollide);
	~c_SecondCar();
	virtual void Ability(double dt);
	virtual void PowerUp(bool check);
	virtual void isOffRoad();

private:

};

#endif