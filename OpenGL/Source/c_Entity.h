#ifndef C_ENTITY_H
#define C_ENTITY_H

#include "Mesh.h"
#include "c_Collision.h"

class c_Entity
{
public:
	c_Entity();
	c_Entity(const char* meshPath,Position pos);
	~c_Entity();

	Mesh* getMesh();
	Position getPos();
	c_Collision getOBB();

protected:
	const char* meshPath;
	Mesh* mesh;
	c_Collision OBB;
	Position pos;
};

#endif