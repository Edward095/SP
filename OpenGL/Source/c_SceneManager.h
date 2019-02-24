#ifndef C_SCENEMANAGER_H
#define C_SCENEMANAGER_H

#include <string>
#include <map>

class c_SceneManager
{
private://Functions and Enums
	c_SceneManager();
	enum GAMESTATE
{
		CONTINUE,
		NPC,
		GARAGE,
		CHANGED,
		SLEVELONE,
		SLEVELTWO,
		SLEVELTHREE,
		MLEVELONE,
		MLEVELTWO,
		MLEVELTHREE,
		FINISHED,
};

public:
	~c_SceneManager();
	static c_SceneManager* getInstance();
	void updateState(std::string state);
	void updateLevel(std::string level);
	bool checkState(std::string state);
	bool checkLevel(std::string level);
	bool singleOrMulti(char sORm);
	std::string getLevel();

private:
	//Variables
	static c_SceneManager* instance;
	std::map <GAMESTATE, std::string> game_State;
	std::string currentState;
	std::string level;
};

#endif