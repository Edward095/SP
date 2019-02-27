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
	game_State[TITLE] = "TITLE";

	npc = new c_Npc();
	garage = new c_Garage();
	sLevelOne = new c_LevelOne();
	sLevelTwo = new c_LevelTwo();
	sLevelThree = new c_LevelThree();
	mLevelOne = new c_MultiplayerLevel();
	mLevelTwo = new c_MultiplayerLevelTwo();
	mLevelThree = new c_MultiplayerLevelThree();

	continueGame = new c_Continue();
	gameEnd = new c_GameEnd();
	title = new c_Title();

	WinOrLose = false;
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
void c_SceneManager::cleanUp()
{
	garage->Exit();
	sLevelOne->Exit();
	sLevelTwo->Exit();
	sLevelThree->Exit();
	mLevelOne->Exit();
	mLevelTwo->Exit();
	mLevelThree->Exit();
	continueGame->Exit();
	gameEnd->Exit();
	title->Exit();


	delete npc;
	delete garage;
	delete sLevelOne;
	delete sLevelTwo;
	delete sLevelThree;
	delete mLevelOne;
	delete mLevelTwo;
	delete mLevelThree;

	delete continueGame;
	delete gameEnd;
	delete title;
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
bool c_SceneManager::getWinOrLose()
{
	return WinOrLose;
}
std::string c_SceneManager::getLevel()
{
	return level;
}
bool c_SceneManager::singleOrMulti(char sOrm)
{
	return (sOrm == level[0]);
}
void c_SceneManager::setWinOrLose(bool winorlose)
{
	WinOrLose = winorlose;
}
Scene* c_SceneManager::getScene(std::string sceneName)
{
	if (sceneName == "NPC")
		return npc;
	else if (sceneName == "GARAGE")
		return garage;
	else if (sceneName == "SLEVELONE")
		return sLevelOne;
	else if (sceneName == "SLEVELTWO")
		return sLevelTwo;
	else if (sceneName == "SLEVELTHREE")
		return sLevelThree;
	else if (sceneName == "MLEVELONE")
		return mLevelOne;
	else if (sceneName == "MLEVELTWO")
		return mLevelTwo;
	else if (sceneName == "MLEVELTHREE")
		return mLevelThree;
	else if (sceneName == "CONTINUE")
		return continueGame;
	else if (sceneName == "FINISHED")
		return gameEnd;
	else if (sceneName == "TITLE")
		return title;
	return nullptr;
}