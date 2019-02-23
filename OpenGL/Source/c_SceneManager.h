#ifndef C_SCENEMANAGER_H
#define C_SCENEMANAGER_H

#include "Scene.h"
#include <string>

class c_SceneManager
{
private://Functions and Enums
	c_SceneManager();
	enum GAMESTATE
{
		NPC,
		GARAGE,
		CHANGED,
		SLEVELONE,
		SLEVELTWO,
		SLEVELTHREE,
		MLEVELONE,
		MLEVELTWO,
		MLEVELTHREE,
};

public:
	~c_SceneManager();
	c_SceneManager* getInstance();
	void updateState(std::string state);


private:
	//Variables
	static c_SceneManager* instance;
	GAMESTATE gameState;
};

#endif