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
	/*bool AABB(c_Collision Car);
	std::vector<Vector3> getBoxes();
	std::vector<Vector3> getCornervec();*/
	//void updateHighLow();

	//Function
	void defaultData();
	void calcNewDimensions(float xScale, float yScale, float zScale);
	void calcNewAxis(float rotateAmt, Vector3 Axis);
	void setHighLow(const char *file_path);
	void setPos(Vector3 pos );
	bool getSeparatingPlane(const Vector3& RPos, const Vector3& Plane, c_Collision& other);
	bool OBB(c_Collision& other);
	bool checkSurroundingOBJ(c_Collision& other);
	Vector3 getDimensions();
	//Vector3 getHalfDimensions();
	Vector3 getXAxis();
	Vector3 getYAxis();
	Vector3 getZAxis();
	//Variables
	Vector3 pos;

private:
	//Function
	//void calcData();
	//Variables
	float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;
	Vector3 dimensions;
	//Vector3 halfDimensions;
	Vector3 localX, localY, localZ;
	std::string type;

	/*Vector3 frontTopLeft, frontTopRight, frontBottomLeft, frontBottomRight;
	Vector3 backTopLeft, backTopRight, backBottomLeft, backBottomRight;
	Vector3 max, min;
	std::vector <Vector3> BB;
	std::vector <Vector3> cornerVec;*/
};

#endif