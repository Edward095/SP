#ifndef C_DATAMANAGER
#define C_DATAMANAGER

#include <string>
#include "c_SceneManager.h"

class c_DataManager
{
public:
	~c_DataManager();
	static c_DataManager* getInstance();
	void saveCustomization(std::string OBJpath, std::string TGApath);
	void saveLapTime(float lapTime);
	void saveCurrentLevel(int levelNum);
	void saveSoundOptions();
	void updateLeaderBoards(float lapTime, std::string name);
	void selectFile(int fileNum);
	void readFromFile(const char* OBJpath,const char* TGApath);


private://Variables
	static c_DataManager* instance;
	c_SceneManager* scene;

	std::string leaderBoardsFile;
	std::string saveFile1;
	std::string saveFile2;
	std::string saveFile3;
	std::string soundFile;
	std::string currentFile;

private://Functions
	void setLevel(std::string line);
	void setPath(std::string line, const char* path);
	float getTiming(std::string line);
	c_DataManager();
};

#endif