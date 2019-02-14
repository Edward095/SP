#include "c_Entity.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

int c_Entity::ID = 0;

c_Entity::c_Entity()
{
}


c_Entity::~c_Entity()
{
}

void c_Entity::init(const char* meshPath,const char* TGApath, Vector3 pos)
{
	this->pos = pos;
	this->meshPath = meshPath;
	this->TGApath = TGApath;
	mesh = MeshBuilder::GenerateOBJ("Mesh", meshPath);
	mesh->textureID = LoadTGA(TGApath);
	OBB.setHighLow(meshPath);
	ID += 1;
	uniqueID = ID;
}

Mesh* c_Entity::getMesh()
{
	return mesh;
}
Vector3 c_Entity::getPos()
{
	return pos;
}
c_Collision c_Entity::getOBB()
{
	return OBB;
}
int c_Entity::getUnique()
{
	return uniqueID;
}