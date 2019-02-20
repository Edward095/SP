#include "c_ObjectManager.h"
#include "c_FirstCar.h"
#include "c_SecondCar.h"
#include "c_ThirdCar.h"

c_ObjectManager* c_ObjectManager::instance = 0;

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
void c_ObjectManager::addOBJ(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos)
{
	if (meshPath == "OBJ//Car1.obj")
		objects.push_back(new c_FirstCar(uniqueName, meshPath, TGApath, pos));
	else if(meshPath =="OBJ//Car2.obj")
		objects.push_back(new c_SecondCar(uniqueName, meshPath, TGApath, pos));
	else if (meshPath == "OBJ//Car3.obj")
		objects.push_back(new c_ThirdCar(uniqueName, meshPath, TGApath, pos));
}
c_ObjectManager* c_ObjectManager::getInstance()
{
	if (!instance)
	{
		instance = new c_ObjectManager();
		return instance;
	}
	else
		return instance;
}
std::vector <c_Entity*> c_ObjectManager::getObjects()
{
	return objects;
}
c_Entity* c_ObjectManager::getObjects(std::string uniqueName)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->getUniqueName() == uniqueName)
			return objects[i];
	}
}
void c_ObjectManager::delInstance()
{
	delete instance;
}
void c_ObjectManager::delNullOBJ()
{
	std::vector <int> elements;

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->getOBB() == nullptr)
			elements.push_back(i);
	}
	for (int i = elements.size() - 1; i >= 0; i++)
	{
		objects.erase(objects.begin() + elements[i]);
	}
}