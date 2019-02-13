#include "c_ObjectManager.h"



c_ObjectManager::c_ObjectManager()
{
}


c_ObjectManager::~c_ObjectManager()
{
}

void c_ObjectManager::addOBJ(c_Entity* OBJ)
{
	objects.push_back(OBJ);
}
c_ObjectManager* c_ObjectManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = this;
		return instance;
	}
	else
		return instance;
}
std::vector <c_Entity*> c_ObjectManager::getObjects()
{
	return objects;
}