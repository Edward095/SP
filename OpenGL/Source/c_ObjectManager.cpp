#include "c_ObjectManager.h"

#include "c_OffRoadManager.h"
#include "c_FirstCar.h"
#include "c_SecondCar.h"
#include "c_ThirdCar.h"

c_ObjectManager* c_ObjectManager::instance = 0;
c_OffRoadManager* ORmanager = c_OffRoadManager::getInstance();

c_ObjectManager::c_ObjectManager()
{
}


c_ObjectManager::~c_ObjectManager()
{
}

void c_ObjectManager::addCannotCollide(c_Entity* OBJ)
{
	cannotCollideOBJ.push_back(OBJ);
}
void c_ObjectManager::addCanCollide(c_Entity* OBJ)
{
	canCollideOBJ.push_back(OBJ);
}
void c_ObjectManager::addCanCollide(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos)
{
	std::string temp = meshPath;

	if (temp == "OBJ//Car1.obj")
		canCollideOBJ.push_back(new c_FirstCar(uniqueName, meshPath, TGApath, pos, true));
	else if (temp == "OBJ//Car2.obj")
		canCollideOBJ.push_back(new c_SecondCar(uniqueName, meshPath, TGApath, pos, true));
	else if (temp == "OBJ//Car3.obj")
		canCollideOBJ.push_back(new c_ThirdCar(uniqueName, meshPath, TGApath, pos, true));

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
std::vector <c_Entity*> c_ObjectManager::getCannotCollide()
{
	return cannotCollideOBJ;
}
c_Entity* c_ObjectManager::getCannotCollide(std::string uniqueName)
{
	for (int i = 0; i < cannotCollideOBJ.size(); i++)
	{
		if (cannotCollideOBJ[i]->getUniqueName() == uniqueName)
			return cannotCollideOBJ[i];
	}
}
std::vector <c_Entity*> c_ObjectManager::getCanCollide()
{
	return canCollideOBJ;
}
c_Entity* c_ObjectManager::getCanCollide(std::string uniqueName)
{
	for (int i = 0; i < canCollideOBJ.size(); i++)
	{
		if (canCollideOBJ[i]->getUniqueName() == uniqueName)
			return canCollideOBJ[i];
	}	
}
void c_ObjectManager::getCanCollide(std::string uniqueName,c_Entity* entity)
{
	c_Entity* entity1;
	for (int i = 0; i < canCollideOBJ.size(); i++)
	{
		if (canCollideOBJ[i]->getUniqueName() == uniqueName)
			entity1= canCollideOBJ[i];
	}

	c_FirstCar* first = dynamic_cast <c_FirstCar*>(entity1);
	if (first)
		entity = first;
	c_SecondCar* second = dynamic_cast <c_SecondCar*>(entity1);
	if (second)
		entity = second;
	c_ThirdCar* third = dynamic_cast <c_ThirdCar*>(entity1);
	if (third)
		entity = third;
}
void c_ObjectManager::delInstance()
{
	delete instance;
}
void c_ObjectManager::delNullOBJ()
{
	std::vector <int> elements;

	for (int i = 0; i < canCollideOBJ.size(); i++)
	{
		if (canCollideOBJ[i]->getOBB() == nullptr)
			elements.push_back(i);
	}
	for (int i = elements.size() - 1; i >= 0; i++)
	{
		canCollideOBJ.erase(canCollideOBJ.begin() + elements[i]);
	}

	for (int i = 0; i < cannotCollideOBJ.size(); i++)
	{
		if (cannotCollideOBJ[i]->getOBB() == nullptr)
			elements.push_back(i);
	}
	for (int i = elements.size() - 1; i >= 0; i++)
	{
		cannotCollideOBJ.erase(cannotCollideOBJ.begin() + elements[i]);
	}
}