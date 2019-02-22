#include "c_OffRoadManager.h"
#include "c_Entity.h"
#include "c_ObjectManager.h"

#include <fstream>

c_OffRoadManager* c_OffRoadManager::instance = 0;

c_ObjectManager* OBJmanager = c_ObjectManager::getInstance();

c_OffRoadManager::c_OffRoadManager()
{
}


c_OffRoadManager::~c_OffRoadManager()
{
}


c_OffRoadManager* c_OffRoadManager::getInstance()
{
	if (!instance)
	{
		instance = new c_OffRoadManager();
		return instance;
	}
	else
		return instance;
}

bool c_OffRoadManager::toIgnore(std::string uniqueName)
{
	for (int i = 0; i < listToIgnore.size(); i++)
	{
		if (uniqueName == listToIgnore[i])
			return true;
	}
	return false;
}

std::vector<std::string> c_OffRoadManager::getList()
{
	return listToIgnore;
}

void c_OffRoadManager::addOffRoad(std::string offRoadOBJtxt)
{
	std::string offRoad = "offRoad";
	const char* meshPath;
	int counter = 0;

	std::string line;
	std::fstream file(offRoadOBJtxt);

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			meshPath = line.c_str();
			new c_Entity(offRoad + std::to_string(counter), meshPath, "Image//Test.tga", (0, 0, 0), false);
			counter++;
		}
	}
	else
		std::cout << "OBJfile cannot be Open. Wrong Directory?or no .txt" << std::endl;
}

void c_OffRoadManager::updateCollision(std::string posLocation,std::string rotateLocation)
{
	std::string offRoad = "offroad";
	std::string uniqueName;
	int counter = 0;
	float posX;
	float posZ;
	int axisCounter = 0;

	std::string previousLine;
	std::string posLine;
	std::fstream posFile(posLocation);
	std::string rotateLine;
	std::fstream rotateFile(rotateLocation);


	if (posFile.is_open())
	{
		while (std::getline(posFile, posLine))
		{
			uniqueName = offRoad + std::to_string(counter);
			if (axisCounter == 0)
			{
				posX = std::stof(posLine);
				OBJmanager->getCannotCollide("offRoad" + std::to_string(counter))->updatePos(posX, 0, 0);
				axisCounter++;
			}
			else if (axisCounter == 1)
			{
				posZ = std::stof(posLine);
				OBJmanager->getCannotCollide("offRoad" + std::to_string(counter))->updatePos(posX, 0, posZ);
				axisCounter = 0;
				counter++;
			}

		}
		counter = 0;
	}
	else
		std::cout << "posFile cannot be Open. Wrong Directory?or no .txt" << std::endl;
	if (rotateFile.is_open())
	{
		while (std::getline(rotateFile, rotateLine))
		{
			OBJmanager->getCannotCollide("offRoad" + std::to_string(counter))->getOBB()->calcNewAxis(std::stof(rotateLine), 0, 1, 0);
			counter++;
		}
	}
	else
		std::cout << "rotateFile cannot be Open. Wrong Directory? or no .txt" << std::endl;
}

void c_OffRoadManager::addToList(std::string uniqueName)
{
	listToIgnore.push_back(uniqueName);
}

void c_OffRoadManager::defaultData()
{
	int counter = 0;
	std::string offRoad = "offRoad";

	for (int i = 0; i < listToIgnore.size(); i++)
	{
		OBJmanager->getCannotCollide(offRoad + std::to_string(counter))->getOBB()->defaultData();
		counter++;
	}
}