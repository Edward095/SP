#ifndef C_THIRDCAR_H
#define C_THIRDCAR_H

#include "c_CarBaseClass.h"
class c_ThirdCar : public c_CarBaseClass
{
public:
	c_ThirdCar();
	c_ThirdCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos,bool canCollide);
	~c_ThirdCar();
	virtual void Ability(double dt);
	virtual void PowerUp(bool check);
private:
	bool once;

};

#endif