#include "c_SlowPad.h"



c_SlowPad::c_SlowPad()
{
}


c_SlowPad::~c_SlowPad()
{
}


float c_SlowPad::f_SlowDown(float v_BoostVel)
{
	v_BoostVel = -0.5f;

	return v_BoostVel;
}