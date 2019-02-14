#ifndef C_ENTITY_H
#define C_ENTITY_H

#include "Mesh.h"
#include "c_Collision.h"


class c_Entity
{
public:
	c_Entity();
	~c_Entity();
	void init(const char* meshPath, const char* TGApath, Vector3 pos);
	void updatePos(Vector3 pos);
	bool gotCollide();
	int getUnique();
	Mesh* getMesh();
	Vector3 getPos();
	c_Collision getOBB();

protected:
	static int ID;
	int uniqueID;
	const char* meshPath;
	const char* TGApath;
	Mesh* mesh;
	c_Collision OBB;
	Vector3 pos;

private:
	void quadORobject();
};

#endif