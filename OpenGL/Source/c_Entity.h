#ifndef C_ENTITY_H
#define C_ENTITY_H

#include "Mesh.h"
#include "c_Collision.h"
#include "c_Sound.h"


class c_Entity
{
public:
	c_Entity();
	c_Entity(std::string uniqueName, const char* meshPath, const char* TGApath, Vector3 pos, bool canCollide);
	~c_Entity();
	virtual void init(std::string uniqueName,const char* meshPath, const char* TGApath, Vector3 pos, bool canCollide);
	void init(std::string uniqueName);
	void updatePos(float xPos,float yPos,float zPos);
	bool ignoreEntity(std::string uniqueName);
	bool gotCollide(float x, float y, float z);
	bool gotCollide(std::string uniqueName, bool canCollide);
	std::string getUniqueName();
	c_Entity* getEntity(std::string uniqueName, bool canCollide);
	Mesh* getMesh();
	Vector3 getPos();
	c_Collision* getOBB();
	const char* getMeshPath();
	const char* getTGApath();

	c_Sound* Audio = c_Sound::getInstance();

protected:
	std::string uniqueName;
	const char* meshPath;
	const char* TGApath;
	Mesh* mesh;
	c_Collision* OBB;
	Vector3 pos;

private:
	void quadORobject();
};

#endif