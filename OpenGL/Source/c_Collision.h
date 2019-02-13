#ifndef C_COLLLISION_H
#define C_COLLISION_H

#include "Utility.h"
#include <vector>

class c_Collision
{
public:
	c_Collision();
	~c_Collision();
	void updateHighLow();
	void setHighLow(const char *file_path);
	bool AABB(c_Collision Car);
	std::vector<Position> getBoxes();
	Position pos;

private:
	float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;
	Position max, min;
	std::vector <Position> BB;
};

#endif