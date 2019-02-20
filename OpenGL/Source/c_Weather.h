#ifndef C_WEATHER_H
#define C_WEATHER_H

#include "Vector"

class c_Weather
{
public:
	c_Weather();
	~c_Weather();
	void update(double dt);
	float getTranslateX();
	float getTranslateY();
	float getTranslateZ();

private:
	float translateX;
	float translateY;
	float translateZ;
};

#endif