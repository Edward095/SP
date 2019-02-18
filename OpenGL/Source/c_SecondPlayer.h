#ifndef C_SECONDPLAYER_H
#define C_SECONDPLAYER_H

#include "c_CarBaseClass.h"

class c_SecondPlayer : public c_CarBaseClass
{
public:
	c_SecondPlayer();
	~c_SecondPlayer();
	virtual void Movement(double dt);
};

#endif