#ifndef C_MAINMENU_H
#define C_MAINMENU_H

#include "Scene.h"
#include "Mesh.h"
#include "Camera.h"
#include "FirstPersonCamera.h"
#include "MatrixStack.h"
#include "Light.h"

class c_MainMenu : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		TEXT,

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
		MENU,
		GAME,
	};
	c_MainMenu();
	~c_MainMenu();
	void Init();
	void Update(double dt);
	void Render();
	void Exit();

private:
	GAME_STATE e_GameState;
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];


	//Week 4
	FirstPersonCamera camera;

	//Week 5
	MS modelStack, viewStack, projectionStack;

	Light light[3];

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color, float spacing);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
};

#endif C_MAINMENU_H