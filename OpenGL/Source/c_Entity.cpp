#include "c_Entity.h"
#include "MeshBuilder.h"


c_Entity::c_Entity()
{
}
c_Entity::c_Entity(const char* meshPath)
{
	this->meshPath = meshPath;
	mesh = MeshBuilder::GenerateOBJ("Mesh",meshPath);
	OBB.setHighLow(meshPath);
}


c_Entity::~c_Entity()
{
}

Mesh* c_Entity::getMesh()
{
	return mesh;
}