#include "c_Entity.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "c_ObjectManager.h"


c_Entity::c_Entity()
{
}


c_Entity::~c_Entity()
{
	delete OBB;
}

void c_Entity::init(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos)
{
	//Add Object to the List
	c_ObjectManager* objectManager = c_ObjectManager::getInstance();
	objectManager->addOBJ(this);

	this->pos = pos;
	this->meshPath = meshPath;
	this->TGApath = TGApath;
	this->uniqueName = uniqueName;
	quadORobject();//if Quad generate Quad else generate Obj
	mesh->textureID = LoadTGA(TGApath);
	OBB = new c_Collision();
	OBB->setHighLow(meshPath);
}

Mesh* c_Entity::getMesh()
{
	return mesh;
}
Vector3 c_Entity::getPos()
{
	return pos;
}
c_Collision* c_Entity::getOBB()
{
	return OBB;
}
c_Entity* c_Entity::getEntity(std::string uniqueName)
{
	c_ObjectManager* OBJmanager = c_ObjectManager::getInstance();

	for (int i = 0; i < OBJmanager->getObjects().size(); i++)
	{
		if (OBJmanager->getObjects().at(i)->uniqueName == uniqueName)
		{
			return OBJmanager->getObjects().at(i);
		}
	}

}
bool c_Entity::gotCollide(float x, float y, float z)
{
	c_ObjectManager* objectManager = c_ObjectManager::getInstance();

	updatePos(pos.x + x, pos.y + y, pos.z + z);

	for (int i = 0; i < objectManager->getObjects().size(); i++)
	{
		c_Collision* collide = objectManager->getObjects().at(i)->getOBB();

		if (objectManager->getObjects().at(i)->getUniqueName() != this->uniqueName &&
			objectManager->getObjects().at(i)->getUniqueName() != "Nitro")
		{
			if (OBB->OBB(collide))
			{
				updatePos(pos.x - x, pos.y - y, pos.z - z);
				return true;
			}
		}
	}
	return false;
}
void c_Entity::quadORobject()
{
	if (meshPath == "quad")
		mesh = MeshBuilder::GenerateQuad(meshPath, Color(1, 1, 1), 1.f);
	else
		mesh = mesh = MeshBuilder::GenerateOBJ("Mesh", meshPath);
}
void c_Entity::updatePos(float xPos, float yPos, float zPos)
{
	this->pos.x = xPos;
	this->pos.y = yPos;
	this->pos.z = zPos;
	OBB->setPos(this->pos);
}
std::string c_Entity::getUniqueName()
{
	return uniqueName;
}