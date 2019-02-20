#include "c_Entity.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "c_ObjectManager.h"


c_Entity::c_Entity()
{
	OBB = nullptr;
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
	OBB = new c_Collision;
	OBB->setHighLow(meshPath);
}
void c_Entity::init(std::string uniqueName)
{
	c_ObjectManager* objectManager = c_ObjectManager::getInstance();

	for (int i = 0; i < objectManager->getObjects().size(); i++)
	{
		if (objectManager->getObjects().at(i)->getUniqueName() == uniqueName)
		{
		c_Entity* other = objectManager->getObjects().at(i);

		this->pos = other->pos;
		this->meshPath = other->meshPath;
		this->TGApath = other->TGApath;
		this->uniqueName = uniqueName;
		this->mesh = other->mesh;
		this->mesh->textureID = LoadTGA(TGApath);
		this->OBB = other->OBB;
		this->OBB->setHighLow(meshPath);
		}
	}

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
	std::vector<c_Entity*> entity = objectManager->getObjects();

	updatePos(pos.x + x, pos.y + y, pos.z + z);

	for (int i = 0; i < objectManager->getObjects().size(); i++)
	{
		c_Collision* collide = entity[i]->getOBB();

		if (objectManager->getObjects().at(i)->getUniqueName() != this->uniqueName &&
			objectManager->getObjects().at(i)->getUniqueName() != "Nitro" &&
			objectManager->getObjects().at(i)->getUniqueName() != "Boostpad" &&
			objectManager->getObjects().at(i)->getUniqueName() != "Slowpad" &&
			objectManager->getObjects().at(i)->getUniqueName() != "FinishLine")

		if (ignoreEntity(entity[i]->uniqueName))

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
bool c_Entity::gotCollide(std::string uniqueName)
{
	c_ObjectManager* objectManager = c_ObjectManager::getInstance();
	c_Collision* objectToCollide = nullptr;

	for (int i = 0; i < objectManager->getObjects().size(); i++)
	{
		if (objectManager->getObjects().at(i)->getUniqueName() == uniqueName)
			objectToCollide = objectManager->getObjects().at(i)->getOBB();
	}
	if (objectToCollide != nullptr && this->OBB->OBB(objectToCollide))
		return true;

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
bool c_Entity::ignoreEntity(std::string uniqueName)
{
	return	(uniqueName != this->uniqueName &&
		uniqueName != "Nitro" &&
		uniqueName != "Boostpad" &&
		uniqueName != "Slowpad");
}