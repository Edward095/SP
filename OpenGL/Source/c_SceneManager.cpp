#include "c_SceneManager.h"

c_SceneManager* c_SceneManager::instance = 0;

c_SceneManager::c_SceneManager()
{
	gameState = NPC;
}


c_SceneManager::~c_SceneManager()
{
}

c_SceneManager* c_SceneManager::getInstance()
{
	if (!instance)
	{
		instance = new c_SceneManager();
		return instance;
	}
	else
		return instance;
}

void c_SceneManager::updateState(std::string state)
{
	if (state == "NPC")
		gameState = NPC;
	else if (state == "GARAGE")
		gameState = GARAGE;
	else if (state == "CHANGED")
		gameState = CHANGED;
	else if (state == "SLEVELONE")
		gameState = SLEVELONE;
	else if (state == "SLEVELTWO")
		gameState = SLEVELTWO;
	else if (state == "SLEVELTHREE")
		gameState = SLEVELTHREE;
	else if (state == "MLEVELONE")
		gameState = MLEVELONE;
	else if (state == "MLEVELTWO")
		gameState = MLEVELTWO;
	else if (state == "MLEVELTHREE")
		gameState = MLEVELTHREE;
}