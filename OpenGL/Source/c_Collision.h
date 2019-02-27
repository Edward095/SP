#ifndef C_COLLLISION_H
#define C_COLLISION_H

#include <Vector3.h>
#include <vector>
#include "Utility.h"

class c_Collision
{
public:
	c_Collision();
	~c_Collision();
	//Function
	void defaultData();
	void calcNewDimensions(float xScale, float yScale, float zScale);
	void calcNewAxis(float rotateAmt, float xAxis, float yAxis, float zAxis);
	void setHighLow(const char *file_path);
	void setPos(Vector3 pos );
	bool getSeparatingPlane(const Vector3& RPos, const Vector3& Plane, c_Collision* other);
	bool OBB(c_Collision* other);
	bool checkSurroundingOBJ(c_Collision* other);
	Vector3 getDimensions();
	Vector3 getXAxis();
	Vector3 getYAxis();
	Vector3 getZAxis();
	//Variables
	Vector3 pos;

private:
	//Function
	//Variables
	float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;
	Vector3 dimensions;
	//Vector3 halfDimensions;
	Vector3 localX, localY, localZ;
	std::string type;

};

#endif