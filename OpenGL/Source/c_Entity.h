#ifndef C_ENTITY_H
#define C_ENTITY_H

#include "Mesh.h"
#include "c_Collision.h"

class c_Entity
{
public:
	c_Entity();
	c_Entity(const char* meshPath);
	~c_Entity();

	Mesh* getMesh();

protected:
	Mesh* mesh;
	std::string meshPath;
	c_Collision OBB;
};

#endif