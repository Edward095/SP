#ifndef C_SLOWPAD_H
#define C_SLOWPAD_H
#include "c_Entity.h"

class c_SlowPad : public c_Entity
{
public:
	c_SlowPad();
	~c_SlowPad();

	float f_SlowDown(float v_BoostVel);
};

#endif