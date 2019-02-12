#ifndef VARIABLES_H
#define VARIABLES_H

#include "MatrixStack.h"
#include "Scene.h"
#include "Mesh.h"
#include "Camera.h"
#include "FirstPersonCamera.h"
#include "MatrixStack.h"
#include "Light.h"

#define RED Color(1,0,0)
#define GREEN Color(0,1,0)
#define BLUE Color(0,0,1)
#define WHITE Color(1,1,1)

enum GEOMETRY_TYPE
{
	//MainMenu
	NEWGAME,
	CONTINUE,
	OPTIONS,
	EXIT,

	//Garage
	TESTCUBE1,
	TESTCUBE2,
	TESTCOLOR1,


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
extern unsigned m_vertexArrayID;
extern Mesh* meshList[NUM_GEOMETRY];

extern unsigned m_programID;
extern unsigned m_parameters[U_TOTAL];
extern FirstPersonCamera camera;

extern MS modelStack, viewStack, projectionStack;

extern Light light[3];

#endif