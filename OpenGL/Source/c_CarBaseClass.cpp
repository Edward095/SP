#include "c_CarBaseClass.h"

c_CarBaseClass::c_CarBaseClass()
{
	objectManager = objectManager->getInstance();
	objectManager->addOBJ(this);
}
c_CarBaseClass::c_CarBaseClass(const char* fileName, Position pos) : c_Entity(fileName, pos)
{
	objectManager = objectManager->getInstance();
	objectManager->addOBJ(this);
}
c_CarBaseClass::~c_CarBaseClass()
{

}
bool c_CarBaseClass::gotCollide()
{
	for (int i = 0; i < objectManager->getObjects().size(); i++)
	{
		c_Collision collide = objectManager->getObjects().at(i)->getOBB();
		if (OBB.OBB(collide))
			return true;
	}
	return false;
}