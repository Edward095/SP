#ifndef C_OBJECTMANAGER_H
#define C_OBJECTMANAGER_H

#include "c_Entity.h"

class c_ObjectManager
{
public:
	~c_ObjectManager();
	void addOBJ(c_Entity* OBJ);
	void addOBJ(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos);
	static c_ObjectManager* getInstance();
	std::vector <c_Entity*> getObjects();
	void getObjects(std::string uniqueName,c_Entity* enitty);
	c_Entity* getObjects(std::string uniqueName);
	void delInstance();
	void delNullOBJ();
private: 
	c_ObjectManager();
	std::vector <c_Entity*> objects;
	static c_ObjectManager* instance;
};
#endif
