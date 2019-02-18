#ifndef C_MULTIPLAYERLEVEL_H
#define C_MULTIPLAYERLEVEL_H

#include "Scene.h"
#include "Mesh.h"
#include "FirstPersonCamera.h"
#include "MatrixStack.h"
#include "Light.h"

#include "c_FirstCar.h"

class c_MultiplayerLevel : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		TOP,
		BOTTOM,
		TRACK,
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

	c_MultiplayerLevel();
	~c_MultiplayerLevel();
	void Init();
	void Update(double dt);
	void Render();
	void Exit();

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	MS modelStack, viewStack, projectionStack;

	Light light[1];

	void initLights();
	void renderLights();
	void updateLights(int num);
	void RenderMesh(Mesh *mesh, bool enableLight);

	void renderPlayerOne();
	void renderPlayerTwo();

	FirstPersonCamera playerOneCam;
	float playerOneCamPosX;
	float playerOneCamPosY;
	float playerOneCamPosZ;
	float playerOneCamTargetX;
	float playerOneCamTargetY;
	float playerOneCamTargetZ;

	FirstPersonCamera playerTwoCam;
	float playerTwoCamPosX;
	float playerTwoCamPosY;
	float playerTwoCamPosZ;
	float playerTwoCamTargetX;
	float playerTwoCamTargetY;
	float playerTwoCamTargetZ;


	c_Entity front;
	c_Entity back;
	c_Entity left;
	c_Entity right;

	c_FirstCar playerOne;
	c_FirstCar playerTwo;
};

#endif