 #ifndef C_FIRSTCAR_H
#define C_FIRSTCAR_H

#include "c_CarBaseClass.h"

class c_FirstCar : public c_CarBaseClass
{
public:
	c_FirstCar();
	c_FirstCar(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos);
	~c_FirstCar();
	virtual void Ability(double dt);
	virtual void PowerUp(bool check);
	virtual void isOffRoad();
};


#endif
