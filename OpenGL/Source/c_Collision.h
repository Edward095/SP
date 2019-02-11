#ifndef C_COLLISION_H
#define C_COLLISION_H

#include "Vertex.h"
#include "Variables.h"

class c_Collision
{
public:
	c_Collision();
	~c_Collision();
	void Init();

private:
	Vertex lowest, highest;
};

#endif