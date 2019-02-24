#include "c_SceneManager.h"

c_SceneManager* c_SceneManager::instance = 0;

c_SceneManager::c_SceneManager()
{
	game_State[CONTINUE] = "CONTINUE";
	game_State[NPC] = "NPC";
	game_State[GARAGE] = "GARAGE";
	game_State[CHANGED] = "CHANGED";
	game_State[SLEVELONE] = "SLEVELONE";
	game_State[SLEVELTWO] = "SLEVELTWO";
	game_State[SLEVELTHREE] = "SLEVELTHREE";
	game_State[MLEVELONE] = "MLEVELONE";
	game_State[MLEVELTWO] = "MLEVELTWO";
	game_State[MLEVELTHREE] = "MLEVELTHREE";
	game_State[FINISHED] = "FINISHED";
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
	for (std::map<GAMESTATE, std::string>::iterator i = game_State.begin(); i != game_State.end(); i++)
	{
		if (game_State[i->first] == state)
			currentState = i->second;
	}
}
void c_SceneManager::updateLevel(std::string level)
{
	this->level = level;
}
bool c_SceneManager::checkState(std::string state)
{
	return (currentState == state);
}
bool c_SceneManager::checkLevel(std::string level)
{
	return (this->level == level);
}
std::string c_SceneManager::getLevel()
{
	return level;
}
bool c_SceneManager::singleOrMulti(char sOrm)
{
	return (sOrm == level[0]);
}