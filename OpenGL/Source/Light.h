#pragma once
#include "Vertex.h"
struct Light
{
	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};
	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;

	Position position;
	Color color;
	float power;
	float kC, kL, kQ;
	Light()
	{
		type = LIGHT_SPOT;
		spotDirection.Set(0, 1, 0);
		cosCutoff = cos(Math::DegreeToRadian(45));
		cosInner = cos(Math::DegreeToRadian(30));
		exponent = 3.f;
		color.Set(1, 1, 1);
		power = 1.0f;
		kC = 1.0f;
		kL = 0.0f;
		kQ = 0.0f;
	}
};