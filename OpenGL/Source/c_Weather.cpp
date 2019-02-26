#include "c_Weather.h"



c_Weather::c_Weather()
{
	translateX = 0.f;
	translateY = 0.f;
	translateZ = 0.f;
}


c_Weather::~c_Weather()
{
}

void c_Weather::init()
{
	for (int i = 0; i < 4000; i++)
	{
		xPos.push_back(rand() % 500 - 250);
		yPos.push_back(rand() % 401 - 200);
		zPos.push_back(rand() % 500 - 250);
	}
}
void c_Weather::update(double dt)
{
	translateX += 35 * 1.5 * dt;
	translateY -= 70 * 1.5 * dt;
	if (translateX >= 109 && translateY <= -54)
	{
		translateX = 0;
		translateY = 0;
	}
}
float c_Weather::getTranslateX()
{
	return translateX;
}
float c_Weather::getTranslateY()
{
	return translateY;
}
float c_Weather::getTranslateZ()
{
	return translateZ;
}
std::vector<float> c_Weather::getX()
{
	return xPos;
}
std::vector<float> c_Weather::getY()
{
	return yPos;
}
std::vector<float> c_Weather::getZ()
{
	return zPos;
}