#ifndef C_ENTITY_H
#define C_ENTITY_H

#include "Mesh.h"
#include "c_Collision.h"

class c_Entity
{
public:
	c_Entity();
	c_Entity(const char* meshPath, Vector3 pos);
	~c_Entity();

	void init(const char* meshPath, Vector3 pos);
	Mesh* getMesh();
	Vector3 getPos();
	c_Collision getOBB();
	int getUnique();

protected:
	static int ID;
	int uniqueID;
	const char* meshPath;
	Mesh* mesh;
	c_Collision OBB;
	Vector3 pos;
};

#endif