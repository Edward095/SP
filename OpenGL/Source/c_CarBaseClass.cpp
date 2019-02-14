#include "c_CarBaseClass.h"

c_CarBaseClass::c_CarBaseClass()
{
	objectManager->addOBJ(this);
}
c_CarBaseClass::c_CarBaseClass(const char* fileName, Vector3 pos) : c_Entity(fileName, pos)
{
}
c_CarBaseClass::~c_CarBaseClass()
{

}
bool c_CarBaseClass::gotCollide()
{
	for (int i = 0; i < objectManager->getObjects().size(); i++)
	{
		c_Collision collide = objectManager->getObjects().at(i)->getOBB();
		if (objectManager->getObjects().at(i)->getUnique() != this->uniqueID)
		{
			if (OBB.OBB(collide))
				return true;
		}
		
	}
	return false;
}

float c_CarBaseClass::GetSteeringAngle()
{
	return SteeringAngle;
}