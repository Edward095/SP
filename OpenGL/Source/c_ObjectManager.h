#ifndef C_OBJECTMANAGER_H
#define C_OBJECTMANAGER_H

#include "c_Entity.h"

class c_ObjectManager
{
public:
	~c_ObjectManager();
	static c_ObjectManager* getInstance();
	void addCannotCollide(c_Entity* OBJ);
	void addCanCollide(c_Entity* OBJ);
	void addCanCollide(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos);
	std::vector <c_Entity*> getCannotCollide();
	c_Entity* getCannotCollide(std::string uniqueName);
	std::vector <c_Entity*> getCanCollide();
	c_Entity* getCanCollide(std::string uniqueName);
	void delInstance();
	void delNullOBJ();
private: 
	c_ObjectManager();
	std::vector <c_Entity*> cannotCollideOBJ;
	std::vector <c_Entity*> canCollideOBJ;
	static c_ObjectManager* instance;
};
#endif
