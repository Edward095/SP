#ifndef C_OBJECTMANAGER_H
#define C_OBJECTMANAGER_H

#include "c_Entity.h"

class c_ObjectManager
{
public:
	~c_ObjectManager();
	void addOBJ(c_Entity* OBJ);
	static c_ObjectManager* getInstance();
	std::vector <c_Entity*> getObjects();
private: 
	c_ObjectManager();
	std::vector <c_Entity*> objects;
	static c_ObjectManager* instance;
};
#endif
