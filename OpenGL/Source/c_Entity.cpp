#include "c_Entity.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "c_ObjectManager.h"

int c_Entity::ID = 0;

c_Entity::c_Entity()
{
}


c_Entity::~c_Entity()
{
}

void c_Entity::init(const char* meshPath,const char* TGApath, Vector3 pos)
{
	//Add Object to the List
	c_ObjectManager* objectManager = c_ObjectManager::getInstance();
	objectManager->addOBJ(this);

	this->pos = pos;
	this->meshPath = meshPath;
	this->TGApath = TGApath;
	quadORobject();//if Quad generate Quad else generate Obj
	mesh->textureID = LoadTGA(TGApath);
	OBB.setHighLow(meshPath);
	uniqueID = ++ID;
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
bool c_Entity::gotCollide()
{
	c_ObjectManager* objectManager = c_ObjectManager::getInstance();
	for (int i = 0; i < objectManager->getObjects().size(); i++)
	{
		c_Collision collide = objectManager->getObjects().at(i)->getOBB();

		if (objectManager->getObjects().at(i)->getUnique() != this->uniqueID)
		{
			if (OBB.OBB(collide))
				return true;
		}

	}
	return false;
}
void c_Entity::quadORobject()
{
	if (meshPath == "quad")
		mesh = MeshBuilder::GenerateQuad(meshPath, Color(1, 1, 1), 5);
	else
		mesh = mesh = MeshBuilder::GenerateOBJ("Mesh", meshPath);
}
void c_Entity::updatePos(Vector3 pos)
{
	this->pos = pos;
	OBB.setPos(pos);
}