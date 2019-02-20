#ifndef GARAGE_H
#define GARAGE_H

#include <iostream>
#include "Scene.h"
#include "MatrixStack.h"
#include "Application.h"
#include "FirstPersonCamera.h"
#include "Light.h"
#include "Mesh.h"
#include "c_Sound.h"

#include "c_LevelOne.h"
#include "c_MultiplayerLevel.h"

#include "c_FirstCar.h"
#include "c_SecondCar.h"
#include "c_ThirdCar.h"

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
		// Cars preview
		CAR1PREVIEW,
		CAR2PREVIEW,
		CAR3PREVIEW,
		// Final Cars
		car1,
		car2,
		car3,
		// Car stat board
		STATBOARD,
		// UI
		UI,
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
	enum  GAME_STATE_GARAGE
	{
		GARAGE_,
		CHANGED_,
		SLEVELONE_,
		SLEVELTWO_,
		SLEVELTHREE_,
		MLEVELONE_,
		MLEVELTWO_,
		MLEVELTHREE_,
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
	double v_BTPause;
	double v_ElapsedTime;
	float v_RotateCar, v_ConfirmRotation;
	float v_ScaleBlue, v_ScaleRed, v_ScalePurple, v_ScaleGreen, v_ScalePink, v_ScaleYellow, v_ScaleWhite, v_ScaleGrey, v_ScaleBlack;
	float v_MoveBlue, v_MoveRed, v_MovePurple, v_MoveGreen, v_MovePink, v_MoveYellow, v_MoveWhite, v_MoveGrey, v_MoveBlack;
	float v_ScalePreview1, v_ScalePreview2, v_ScalePreview3;
	unsigned v_Car1Blue, v_Car1Red, v_Car1Purple, v_Car1Green, v_Car1Pink, v_Car1Yellow, v_Car1White, v_Car1Grey, v_Car1Black;
	unsigned v_Car2Blue, v_Car2Red, v_Car2Purple, v_Car2Green, v_Car2Pink, v_Car2Yellow, v_Car2White, v_Car2Grey, v_Car2Black;
	unsigned v_Car3Blue, v_Car3Red, v_Car3Purple, v_Car3Green, v_Car3Pink, v_Car3Yellow, v_Car3White, v_Car3Grey, v_Car3Black;
	unsigned v_Car1Stats, v_Car2Stats, v_Car3Stats;
	GAME_STATE_GARAGE e_GameState_Garage;
	bool v_MusicPause;
	bool v_Car1Changed;
	bool v_Car2Changed;

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
	void f_RenderStats();

	void f_UpdateGarage(double dt);
	void f_UpdateGarage2(double dt);
	void f_RenderGarage();
	void f_RenderGarage2();

	c_List v_CarList;
	c_List v_ColourList;
	c_Sound v_Garage_SFX;

	const char* v_CarPaths[3];
	const char* v_CarColourPath1[9];
	const char* v_CarColourPath2[9];
	const char* v_CarColourPath3[9];

	c_LevelOne sLevelOne;
	c_FirstCar car1Type1;
	c_SecondCar car1Type2;
	c_ThirdCar car1Type3;
	c_FirstCar car2Type1;
	c_SecondCar car2Type2;
	c_ThirdCar car2Type3;


	c_MultiplayerLevel mLevelOne;

};

#endif