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