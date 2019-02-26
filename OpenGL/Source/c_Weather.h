#ifndef C_WEATHER_H
#define C_WEATHER_H

#include "Vector"

class c_Weather
{
public:
	c_Weather();
	~c_Weather();
	void init();
	void update(double dt);
	float getTranslateX();
	float getTranslateY();
	float getTranslateZ();
	std::vector<float> getX();
	std::vector<float> getY();
	std::vector<float> getZ();

private:
	float translateX;
	float translateY;
	float translateZ;
	std::vector<float> xPos;
	std::vector<float> yPos;
	std::vector<float> zPos;
};

#endif