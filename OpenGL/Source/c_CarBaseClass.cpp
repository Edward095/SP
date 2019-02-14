#include "c_CarBaseClass.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

c_CarBaseClass::c_CarBaseClass()
{
}
c_CarBaseClass::~c_CarBaseClass()
{

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