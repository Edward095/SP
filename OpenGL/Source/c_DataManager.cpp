#include "c_DataManager.h"
#include <fstream>
#include <iostream>

#include "c_SceneManager.h"



c_DataManager* c_DataManager::instance = 0;

c_DataManager::c_DataManager()
{
	leaderBoardsFile = "Data//LeaderBoards.txt";
	saveFile1 = "Data//SaveFile1.txt";
	saveFile2 = "Data//SaveFile2.txt";
	saveFile3 = "Data//SaveFile3.txt";
	soundFile = "Data//SoundOptions.txt";
}


c_DataManager::~c_DataManager()
{
}

c_DataManager* c_DataManager::getInstance()
{
	if (!instance)
	{
		instance = new c_DataManager;
		c_DataManager();
		return instance;
	}
	else
		return instance;
}
void c_DataManager::clearFile()
{
	std::ofstream file;
	file.open(currentFile, std::ofstream::out | std::ofstream::trunc);
	file.close();
}
void c_DataManager::selectFile(int fileNum)
{
	if (fileNum == 1)
		currentFile = saveFile1;
	else if (fileNum == 2)
		currentFile = saveFile2;
	else if (fileNum == 3)
		currentFile = saveFile3;
}
void c_DataManager::saveCustomization(std::string OBJpath,std::string TGApath)
{
	std::ofstream file(currentFile);
	if (file.is_open())
	{
		file << "OBJpath: " << OBJpath << "\n";
		file << "TGApath: " << TGApath;
		file.close();
	}
	else
		std::cout << "File cannot be open.Is it in the correct directory and did u add the file extension?" << std::endl;
}
void c_DataManager::updateLeaderBoards(float lapTime, std::string name)
{
	std::ofstream file(leaderBoardsFile,std::ios::app);
	if (file.is_open())
	{
		file << "Player's Name: " << name << "\n";
		file << "lapTime: " << lapTime << "\n";
		file.close();
	}
	else
		std::cout << "File cannot be open.Is it in the correct directory and did u add the file extension?" << std::endl;
}

void c_DataManager::saveCurrentLevel(int levelNum)
{
	std::vector <std::string> lines;
	std::string line;

	std::ifstream readFile(currentFile);
	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			std::getline(readFile, line);
			lines.push_back(line);
		}
		readFile.close();
	}
	else
		std::cout << "File cannot be open.Is it in the correct directory and did u add the file extension?" << std::endl;
	/************************************************************************************************************************************/
	if (lines.size() >= 3)
		lines[2] = "Level: " + std::to_string(levelNum);

	std::ofstream writeFile(currentFile);
	if (writeFile.is_open())
	{
		for (int i = 0; i < lines.size(); i++)
		{
			writeFile << lines[i] << "\n";
			if (i == 1 && lines.size() < 3)
			{
				writeFile << "Level: " + std::to_string(levelNum);
			}
		}
		writeFile.close();
	}
	else
		std::cout << "File cannot be open.Is it in the correct directory and did u add the file extension?" << std::endl;
}

void c_DataManager::saveSoundOptions(float volume)
{
	std::ofstream writeFile(soundFile);
	writeFile << "Volume: " << std::to_string(volume);
	writeFile.close();
}

void c_DataManager::readFromFile(std::string& OBJpath,std::string& TGApath)
{
	std::string line;
	int counter = 1;

	std::ifstream file(currentFile);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::getline(file, line);
			if (counter == 1)
				setPath(line, OBJpath);
			else if (counter == 2)
				setPath(line, TGApath);
			else if (counter == 3)
				setLevel(line);
			counter++;
		}
		file.close();
	}
	else
		std::cout << "File cannot be open.Is it in the correct directory and did u add the file extension?" << std::endl;
}
void c_DataManager::getLeaderBoards(std::vector <float>& data, std::vector <std::string>& name)
{
	std::string line;
	int counter = 1;

	std::ifstream file(leaderBoardsFile);
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::getline(file, line);
			if (line[0] == 'P')
				name.push_back(getName(line));
			if (line[0] == 'l')
				data.push_back(getTiming(line));
			counter++;
		}
		file.close();
	}
	else
		std::cout << "File cannot be open.Is it in the correct directory and did u add the file extension?" << std::endl;
}
float c_DataManager::getSoundOptions()
{
	std::string line;
	std::string volume;
	std::ifstream readFile(soundFile);
	
	std::getline(readFile, line);
	for (int i = 8; i < line.size(); i++)
	{
		volume.push_back(line[i]);
	}

	return std::stof(volume);
}
bool c_DataManager::isEmpty(int fileNum)
{
	std::vector < std::string> lines;
	std::string line;
	std::ifstream readFile;
	if (fileNum == 1)
		readFile.open(saveFile1);
	else if (fileNum == 2)
		readFile.open(saveFile2);
	else if (fileNum == 3)
		readFile.open(saveFile3);

	if (readFile.is_open())
	{
		while (!readFile.eof())
		{
			std::getline(readFile, line);
			lines.push_back(line);
		}
		readFile.close();
	}

	return lines.empty();
}


void c_DataManager::setLevel(std::string line)
{
	c_SceneManager* scene = c_SceneManager::getInstance();
	if (line[7] == '1')
		scene->updateLevel("SLEVELONE");
	else if (line[7] == '2')
		scene->updateLevel("SLEVELTWO");
}
void c_DataManager::setPath(std::string line, std::string& path)
{
	std::string filePath;
	for (int i = 9; i < line.size(); i++)
	{
		filePath.push_back(line[i]);
	}
	path = filePath.c_str();
}

float c_DataManager::getTiming(std::string line)
{
	std::string timeString;
	
	for (int i = 9; i < line.size(); i++)
	{
		timeString.push_back(line[i]);
	}
	return std::stof(timeString);
}
std::string c_DataManager::getName(std::string line)
{
	std::string name;

	for (int i = 15; i < line.size(); i++)
	{
		name.push_back(line[i]);
	}
	return name;
}