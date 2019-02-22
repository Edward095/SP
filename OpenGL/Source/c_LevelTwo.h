#ifndef C_LEVELTWO_H
#define C_LEVELTWO_H

#include "Scene.h"
#include "Mesh.h"
#include "FirstPersonCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "c_FirstCar.h"
#include "c_Entity.h"
#include "c_AI.h"
#include "c_Weather.h"


class c_LevelTwo : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		TEST,
		TEXT,
		TOP,
		BOTTOM,
		CAR1,
		LIGHT1,
		LIGHT2,
		TRACK,
		RACEBANNER,
		STREETLIGHT,
		NITRO,
		BOOSTPAD,
		SLOWPAD,
		RAIN,
		SNOW,
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

		// Lights
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

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,

		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,

		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,

		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		U_LIGHT4_POSITION,
		U_LIGHT4_COLOR,
		U_LIGHT4_POWER,
		U_LIGHT4_KC,
		U_LIGHT4_KL,
		U_LIGHT4_KQ,

		U_LIGHT4_TYPE,
		U_LIGHT4_SPOTDIRECTION,
		U_LIGHT4_COSCUTOFF,
		U_LIGHT4_COSINNER,
		U_LIGHT4_EXPONENT,

		U_LIGHT5_POSITION,
		U_LIGHT5_COLOR,
		U_LIGHT5_POWER,
		U_LIGHT5_KC,
		U_LIGHT5_KL,
		U_LIGHT5_KQ,

		U_LIGHT5_TYPE,
		U_LIGHT5_SPOTDIRECTION,
		U_LIGHT5_COSCUTOFF,
		U_LIGHT5_COSINNER,
		U_LIGHT5_EXPONENT,
		//***********************
		U_LIGHTENABLED,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_NUMLIGHTS,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};


	c_LevelTwo();
	~c_LevelTwo();
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

	Light lights[6];

	void initLights();
	void renderLights();
	void updateLights(int num);

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color, float spacing);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	void renderEnviroment();
	void updateEnviromentCollision();

	c_FirstCar car;

	c_Entity front;
	c_Entity back;
	c_Entity left;
	c_Entity right;
	c_Entity nitro;
	c_AI AI;

	c_Entity boost;
	c_Entity slow;
	c_Entity FinishLine;
	//Variables

	bool talk;
	float elapsedTime;
	float FreezeTime;
	float TimePassed;
	bool AbletoPress;
	float CamPosX;
	float CamPosY;
	float CamPosZ;
	float CamTargetX;
	float CamTargetY;
	float CamTargetZ;
	bool Freeze = false;
	bool Raining = false;
	bool Snowing = false;
	bool OffRoad = false;
	float FPS;

	int duration;
	int Random;
	float bLightEnabled;
	std::string elapedTimeCut;
	std::string CountdownCut;
	int Cooldown;
	float Countdown;
	float Timer;
	bool Finish = false;
	int laps;
	int AIlaps;
	bool AIFinish = false;
	bool CheckEnd = false;
	bool Win = false;
	bool Lose = false;

	c_Weather rain;
	c_Weather snow;
	void renderRain();
	void RenderSnow();


};



#endif