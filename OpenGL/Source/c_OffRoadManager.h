#ifndef C_OFFROADMANAGER_H
#define C_OFFROADMANAGER_H

#include <string>
#include <vector>

class c_OffRoadManager
{
public:
	~c_OffRoadManager();
	static c_OffRoadManager* getInstance();
	bool toIgnore(std::string uniqueName);
	std::vector<std::string> getList();
	void addToList(std::string uniqueName);
	void addOffRoad(std::string offRoadOBJtxt);
	void updateCollision(std::string posLocation,std::string rotateLocation);
	void defaultData();
	void clearList();

private:
	c_OffRoadManager();
	static c_OffRoadManager* instance;
	std::vector <std::string> listToIgnore;
};

#endif