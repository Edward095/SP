#ifndef C_NPC_H
#define C_NPC_H

#include "Scene.h"
#include "Scene.h"
#include "Mesh.h"
#include "NPCCamera.h"
#include "MatrixStack.h"
#include "Light.h"

#include "c_Garage.h"
#include "c_LevelOne.h"
#include "c_Sound.h"

class c_Npc :
	public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		TEXT,
		FRONT,
		TOP,
		BACK,
		LEFT,
		RIGHT,
		BOTTOM,
		NPC,
		HOUSE,
		INSTRUCTIONS,
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

	c_Npc();
	~c_Npc();
	void Init();
	void Update(double dt);
	void Render();
	void Exit();
	void UpdateNpc(double dt);
	void RenderNpc();

private:

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	NPCCamera camera;
	MS modelStack, viewStack, projectionStack;

	//Light
	Light light[1];
	
	//Class
	//c_Garage Garage;
	//c_LevelOne LevelOne;

	c_Sound* Audio;


	//Function
	void initLights();
	void renderLights();
	void updateLights(int num);

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color, float spacing);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void resetVar();


	//Variables
	double ElapsedTime;
	double TimePassed;
	float ArrowY;
	double BounceTime;

	bool AbleToPress;
	bool Talk;
	bool Talk1;
	bool Talk2;
	bool Talk3;
	bool Talk4;
	bool Talk5;
	bool LevelSelection;
	bool SinglePlayer;
	bool MultiPlayer;
	bool StartGame;
	bool Continue;
	bool Options;
	bool Instructions;
	bool Override;

	bool LeaderBoard;
};

#endif
