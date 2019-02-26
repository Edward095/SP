#ifndef C_DATAMANAGER
#define C_DATAMANAGER

#include <string>
#include <vector>

class c_DataManager
{
public:
	~c_DataManager();
	static c_DataManager* getInstance();
	void clearFile();
	void saveCustomization(std::string OBJpath, std::string TGApath);
	void saveCurrentLevel(int levelNum);
	void saveSoundOptions(float volume);
	void updateLeaderBoards(float lapTime, std::string name);
	void selectFile(int fileNum);
	void readFromFile(std::string& OBJpath, std::string& TGApath);
	void getLeaderBoards(std::vector <float>& data, std::vector <std::string>& name);
	float getSoundOptions();
	bool isEmpty(int fileNum);


private://Variables
	static c_DataManager* instance;

	std::string leaderBoardsFile;
	std::string saveFile1;
	std::string saveFile2;
	std::string saveFile3;
	std::string soundFile;
	std::string currentFile;

private://Functions
	void setLevel(std::string line);
	void setPath(std::string line, std::string& path);
	float getTiming(std::string line);
	std::string getName(std::string line);
	c_DataManager();
};

#endif