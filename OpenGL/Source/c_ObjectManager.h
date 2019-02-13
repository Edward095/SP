#ifndef C_OBJECTMANAGER_H
#define C_OBJECTMANAGER_H

#include "c_Entity.h"

class c_ObjectManager
{
public:
	c_ObjectManager();
	~c_ObjectManager();
	void addOBJ(c_Entity* OBJ);
	c_ObjectManager* getInstance();
	std::vector <c_Entity*> getObjects();
private: 
	std::vector <c_Entity*> objects;
	c_ObjectManager* instance;
};
#endif
