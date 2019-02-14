#ifndef C_SCENEMANAGER_H
#define C_SCENEMANAGER_H

#include "Scene.h"

class c_SceneManager
{
private:
	c_SceneManager();
	enum GAMESTATE
{
	//MainMenu
	MENU = 0,
	NEWGAME,
	CONTINUE,
	OPTIONS,
	EXIT,
	//NPC(Choose level)
	NPC,
	//Garage(Choose Car)
	GARAGE
};

public:
	~c_SceneManager();
	c_SceneManager* getInstance();
	Scene* selectScene(Scene* scene);

private:
	

	//Variables
	static c_SceneManager* instance;
	

	GAMESTATE gameState;
	Scene* scene;
};

#endif