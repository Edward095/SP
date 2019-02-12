#ifndef GARAGE_H
#define GARAGE_H

#include <iostream>
#include "Scene.h"
#include "c_Render.h"
#include "Application.h"


class c_Car
{
private:
	int v_CarNum;
public:
	c_Car();
	~c_Car();

	int f_GetCarNum();
	void f_SetCarNum(int carNumber);
};

class c_Colour
{
private:
	int v_ColourNum;
	enum v_Colours
	{
		Blue,
		Red,
		Purple,
		Green,
	};
public:
	c_Colour();
	~c_Colour();

	int f_GetColourNum();
	void f_SetColourNum(int colour);
};

class c_List
{
private:
	c_Car *v_CarList[3];
	c_Colour *v_ColourList[4];
	c_Car *v_CurCar;
	c_Colour *v_CurColour;
	int v_IndexCar;
	int v_IndexColour;

public:
	c_List();
	~c_List();

	void f_ChangeCurrentCar(char input);
	void f_ChangeCurrentColour(char input);
	c_Car* f_GetCurCar();
	c_Colour* f_GetCurColour();
};

class c_Garage : public Scene
{
public:
	c_Garage();
	~c_Garage();

	void Init();
	void Update(double dt);
	void Render();
	void Exit();

	c_List v_CarList;
	c_List v_ColourList;

private:

};

#endif