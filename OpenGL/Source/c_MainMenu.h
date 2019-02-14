#ifndef C_MAINMENU_H
#define C_MAINMENU_H

#include "Scene.h"
#include "Mesh.h"
#include "FirstPersonCamera.h"
#include "MatrixStack.h"
#include "Light.h"

#include "c_Npc.h"
#include "c_LevelOne.h"

class c_MainMenu : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		box1,
		box2,


		TEXT,
		ARROW,
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
	enum GAME_STATE
	{
		MENU = 0,
		NEWGAME,
		CONTINUE,
		OPTIONS,
		EXIT,
	};
	c_MainMenu();
	~c_MainMenu();
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

	Light light[1];
	GAME_STATE e_GameState;
	float ArrowX, ArrowY;
	double elapsedTime;
	double bounceTime;

	void RenderMesh(Mesh *mesh, bool enableLight);

	void initLights();
	void renderLights();
	void updateLights(int num);

	void renderSelection();
	void updateSelection(double dt);
	void renderNewGame();
	void updateNewGame(double dt);
	void renderContinue();
	void updateContinue(double dt);
	void renderOptions();
	void updateOption(double dt);

	c_Npc Npc;
	c_LevelOne LevelOne;
};

#endif C_MAINMENU_H