#ifndef GARAGE_H
#define GARAGE_H

#include <iostream>
#include "Scene.h"
#include "MatrixStack.h"
#include "Application.h"
#include "FirstPersonCamera.h"
#include "Light.h"
#include "Mesh.h"


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
		Pink,
		Yellow,
		White,
		Grey,
		Black,
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
	c_Colour *v_ColourList[9];
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

	enum GEOMETRY_TYPE
	{
		TEXT,
		ARROW,
		GARAGEBG,
		// Colour pallete
		COLOUR_BLUE,
		COLOUR_RED,
		COLOUR_PURPLE,
		COLOUR_GREEN,
		COLOUR_PINK,
		COLOUR_YELLOW,
		COLOUR_WHITE,
		COLOUR_GREY,
		COLOUR_BLACK,
		// Cars w/o colour
		CAR1PREVIEW,
		CAR2PREVIEW,
		CAR3PREVIEW,
		// Final Cars
		car1,
		car2,
		car3,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		//Light1
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		//***********************
		U_LIGHTENABLED,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_NUMLIGHTS,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	void Init();
	void Update(double dt);
	void Render();
	void Exit();

private:

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	FirstPersonCamera camera;
	MS modelStack, viewStack, projectionStack;
	double v_BounceTime;
	double v_ElapsedTime;
	float v_RotateCar;
	float v_ScaleBlue, v_ScaleRed, v_ScalePurple, v_ScaleGreen, v_ScalePink, v_ScaleYellow, v_ScaleWhite, v_ScaleGrey, v_ScaleBlack;
	float v_MoveBlue, v_MoveRed, v_MovePurple, v_MoveGreen, v_MovePink, v_MoveYellow, v_MoveWhite, v_MoveGrey, v_MoveBlack;
	float v_ScalePreview1, v_ScalePreview2, v_ScalePreview3;
	const char *v_TGAName;

	Light light[1];

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	void initLights();
	void renderLights();
	void updateLights(int num);
	void f_RenderPallet();
	void f_UpdateCurColour();
	void f_RenderPreviews();
	void f_UpdateCurCar();
	void f_RenderFinal();

	c_List v_CarList;
	c_List v_ColourList;
};

#endif