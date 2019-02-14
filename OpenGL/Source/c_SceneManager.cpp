#include "c_SceneManager.h"

c_SceneManager* c_SceneManager::instance = 0;

c_SceneManager::c_SceneManager()
{
	gameState = MENU;
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
Scene* c_SceneManager::selectScene(Scene* scene)
{
	scene->Init();
	return scene;
}