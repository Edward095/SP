#include "c_CarBaseClass.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

c_CarBaseClass::c_CarBaseClass()
{
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
void c_CarBaseClass::updateAppearance(const char* meshPath, const char* TGApath)
{
	this->meshPath = meshPath;
	this->TGApath = TGApath;
	mesh = MeshBuilder::GenerateOBJ("Mesh", meshPath);
	mesh->textureID = LoadTGA(TGApath);
}