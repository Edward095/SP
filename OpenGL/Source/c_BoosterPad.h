#ifndef C_BOOSTERPAD_H
#define C_BOOSTERPAD_H
#include "c_Entity.h"

class c_BoosterPad : public c_Entity
{
public:
	c_BoosterPad();
	~c_BoosterPad();

	float f_Boost(float v_BoostVel); // In game update() just do v_Boost -= (float)(insertNumberHere * dt) for decaying boost
									 // and put it inside an if(v_Veloctiy > 0) to prevent the boost from becoming < 0
};

#endif