#include "c_DataManager.h"
#include <fstream>
#include <iostream>
#include <vector>

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


void c_DataManager::saveLapTime(float lapTime)
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
	if(lines.size()>=3)
		lines[2] = "Lap Timing:" + std::to_string(lapTime);

	std::ofstream writeFile(currentFile);
	if (writeFile.is_open())
	{
		for (int i = 0; i < lines.size(); i++)
		{
			writeFile << lines[i] << "\n";
			if (i == 1 && lines.size() < 3)
			{
				writeFile << "Lap Timing:" + std::to_string(lapTime);
			}
		}
		writeFile.close();
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
	if (lines.size() >= 4)
		lines[3] = "Level:" + std::to_string(levelNum);

	std::ofstream writeFile(currentFile);
	if (writeFile.is_open())
	{
		for (int i = 0; i < lines.size(); i++)
		{
			writeFile << lines[i] << "\n";
			if (i == 2 && lines.size() < 4)
			{
				writeFile << "Level:" + std::to_string(levelNum);
			}
		}
		writeFile.close();
	}
	else
		std::cout << "File cannot be open.Is it in the correct directory and did u add the file extension?" << std::endl;
}