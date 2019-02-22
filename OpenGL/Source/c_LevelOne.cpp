#include "c_LevelOne.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <iomanip>

#include <Windows.h>
#include <time.h>


c_LevelOne::c_LevelOne()
{
}


c_LevelOne::~c_LevelOne()
{
}

void c_LevelOne::Init()
{
	//Seed Generation For rand() function
	srand(time(NULL));
	//Initialization Of Variables//

        //----Setting Car Variables------//
	car.SetFriction(0.1);
	car.SetSteering(5);
	//-------------------------------//

	//----Setting Up Camera Coordinates--------//
	CamPosX = car.getPos().x + 1;
	CamPosY = car.getPos().y + 1;
	CamPosZ = car.getPos().z + 1;
	CamTargetX = car.getPos().x;
	CamTargetY = car.getPos().y;
	CamTargetZ = car.getPos().z;
	//-----------------------------------------//

	//----Time Related Variables-----//
	elapsedTime = 0;
	FreezeTime = 0;
	duration = 0;
	Cooldown = 0;
	Countdown = 3;
	Timer = 0;
	laps = 2;
	AIlaps = 2;
	FPS = 0;
	//-------------------------------//

	//----Random Number Gen----------//
	Random = rand() % 3 + 1;
	//-------------------------------//


	// Set background color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//Enable back face culling
	glEnable(GL_CULL_FACE);
	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Use our shader
	glUseProgram(m_programID);

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	initLights();

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	glUniform1i(m_parameters[U_NUMLIGHTS], 6);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	//Text
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	//Initialize camera settings
	camera.Init(Vector3(0, 8, 5), Vector3(0, 1, 0), Vector3(0, 1, 0));

	//Initialize all meshes to NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	//Set projection to Perspective and load projection matrix
	Mtx44 projection;
	projection.SetToPerspective(60.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);
	

	//initialization of the Enums & Entity for skybox//

	//----Rendering Skybox For Different Weathers-------------------------------------//
	if (Random == 1)
	{
		meshList[TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1), 1.f);
		meshList[TOP]->textureID = LoadTGA("Image//RainTop.tga");
		meshList[BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
		meshList[BOTTOM]->textureID = LoadTGA("Image//RainBottom.tga");
		front.init("front", "quad", "Image//RainFront.tga", (float)(0, 0, 0));
		left.init("left", "quad", "Image//RainLeft.tga", (float)(0, 0, 0));
		right.init("right", "quad", "Image//RainRight.tga", (float)(0, 0, 0));
		back.init("back", "quad", "Image//RainBack.tga", (float)(0, 0, 0));
	}
	if (Random == 2)
	{
		meshList[TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1), 1.f);
		meshList[TOP]->textureID = LoadTGA("Image//SnowTop.tga");
		meshList[BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
		meshList[BOTTOM]->textureID = LoadTGA("Image//SnowBottom.tga");
		front.init("front", "quad", "Image//SnowFront.tga", (float)(0, 0, 0));
		left.init("left", "quad", "Image//SnowLeft.tga", (float)(0, 0, 0));
		right.init("right", "quad", "Image//SnowRight.tga", (float)(0, 0, 0));
		back.init("back", "quad", "Image//SnowBack.tga", (float)(0, 0, 0));
	}
	if (Random == 3)
	{
		meshList[TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1), 1.f);
		meshList[TOP]->textureID = LoadTGA("Image//SunnyTop.tga");
		meshList[BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
		meshList[BOTTOM]->textureID = LoadTGA("Image//SunnyBottom.tga");
		front.init("front", "quad", "Image//SunnyFront.tga", (float)(0, 0, 0));
		left.init("left", "quad", "Image//SunnyLeft.tga", (float)(0, 0, 0));
		right.init("right", "quad", "Image//SunnyRight.tga", (float)(0, 0, 0));
		back.init("back", "quad", "Image//SunnyBack.tga", (float)(0, 0, 0));
	}
	//---------------------------------------------------------------------------------//

	//----Rendering Text-----------------------------------------//
	meshList[TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[TEXT]->textureID = LoadTGA("Image//calibri.tga");
	//-----------------------------------------------------------//

	//----Rendering Light Orbs------------------------------------------------------------------------//
	meshList[LIGHT1] = MeshBuilder::GenerateSphere("environment light", Color(1, 1, 1), 18, 36, 1.f);
	meshList[LIGHT2] = MeshBuilder::GenerateSphere("street light", Color(1, 1, 1), 18, 36, 1.f);
	//------------------------------------------------------------------------------------------------//

	//----Rendering Race Track And Stuff On Race Track----------------------------------------//
	meshList[TRACK] = MeshBuilder::GenerateOBJ("racetrack", "OBJ//RaceTrack1.obj");
	meshList[TRACK]->textureID = LoadTGA("Image//RaceTrack.tga");

	meshList[RACEBANNER] = MeshBuilder::GenerateOBJ("race banner", "OBJ//RaceBanner.obj");
	meshList[STREETLIGHT] = MeshBuilder::GenerateOBJ("street light", "OBJ//Streetlamp.obj");
	meshList[STREETLIGHT]->textureID = LoadTGA("Image//Streetlamp.tga");
	//----------------------------------------------------------------------------------------//


	//----Rendering Weather Conditions--------------------------------------------------------//
        meshList[RAIN] = MeshBuilder::GenerateSphere("Rain", Color(0,0,1), 18, 18, 2);
	meshList[SNOW] = MeshBuilder::GenerateSphere("Snow", Color(1, 1, 1), 18, 18, 2);
    //----------------------------------------------------------------------------------------//




	//Init Entities//
	car.init("player1");
	boost.init("Boostpad", "OBJ//Pad.obj", "Image//BoostPad.tga", Vector3(20, 1.f, 0));
	slow.init("Slowpad", "OBJ//Pad.obj", "Image//SlowPad.tga", Vector3(-20, 1.f, 0));
	FinishLine.init("FinishLine", "quad", "Image//Test.tga", Vector3(0, 0, -20));
	AI.init("AI", "OBJ//Car1Body.obj", "Image//Car1Blue.tga", Vector3(-5, 0, 0));



	//---- Enabling Light------------//
	bLightEnabled = true;
	//-------------------------------//

	//Initialization Of Weather Functions//
	rain.init();
	snow.init();
}

void c_LevelOne::Update(double dt)
{s
	//----Setting Of Time And FPS-------//
	Timer += (float)dt;
	Countdown -= (float)Timer * dt;
	FPS = 1 / dt;
	//----------------------------------//

	//----Power Up Timer------------------// 
	FreezeTime = (float)(dt + (dt * 0));
	//------------------------------------//

	//----Updating Camera Position---------------------------------------------------------------//
	CamPosX = (car.getPos().x - (sin(Math::DegreeToRadian(car.GetSteeringAngle()))) * 10);
	CamPosY = car.getPos().y + 8;
	CamPosZ = (car.getPos().z - (cos(Math::DegreeToRadian(car.GetSteeringAngle()))) * 10);
	CamTargetX = car.getPos().x;
	CamTargetY = car.getPos().y + 5;
	CamTargetZ = car.getPos().z;
	//-------------------------------------------------------------------------------------------//

	//----KeyPress to Enable and Disable Light-------//
	if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}
	//-----------------------------------------------//

	//----KeyPress to enable PowerUps----------------//
	if (Application::IsKeyPressed('F'))
	{
		Freeze = true;
	}
	if (Freeze && duration <= 150)
	{
		duration++;
		elapsedTime -= FreezeTime;

		if (duration >= 150) // 3 sec/dt
		{
			Freeze = false;
			duration = 0;
		}
	}

	if (car.getPos().x == nitro.getPos().x && car.getPos().z == nitro.getPos().z)
	{
		car.PowerUp(true);
	}
	//-------------------------------------------------//
	
	//----Collision For Finishing Line---------------------------//
	if (car.gotCollide("FinishLine"))
	{
		Finish = true;
	}
	else
	{
		Finish = false;
	}

	if (Finish)
	{
		if (elapsedTime <= 36)
			elapsedTime += (dt + 2);

		if (elapsedTime >= 37 && elapsedTime <= 80)
			laps = 1;
		if (elapsedTime >= 81 && elapsedTime <= 140)
			laps = 0;
	}

	if (AI.gotCollide("FinishLine"))
	{
		AIFinish = true;
	}
	else
	{
		AIFinish = false;
	}

	if (AIFinish)
	{
		if (elapsedTime >= 59 && elapsedTime <= 65)
			AIlaps = 1;
		if (elapsedTime >= 119 && elapsedTime <= 1128)
			AIlaps = 0;
	}

	if (laps == 0 || AIlaps == 0)
	{
		if (laps < AIlaps)
			Win = true;
		else
			Lose = true;
	}
	//-----------------------------------------------------------//

	//----Weather and Environment Effects-------//
	if (Raining)
	{
		car.SetSteering(9);
	}
	if (Snowing)
	{
		car.SetFriction(0.01);
	}
	if (OffRoad)
	{
		car.SetFriction(0.5);
		car.SetMaxSpeed(0.1);
	}

	rain.update(dt);
	snow.update(dt);
	//-------------------------------------------//

	//----Countdown to Start Of the Game---------//
	if (Countdown <= 0)
	{
		elapsedTime += (float)dt;
		car.Movement(dt);
		AI.LevelOne(dt);
	}
	//-------------------------------------------//

    //Updating Car Position for Player and AI
	car.updatePos(car.getPos().x, car.getPos().y, car.getPos().z);
	//AI.updatePos(AI.getPos().x, AI.getPos().y, AI.getPos().z);

	//Update Camera
	camera.Update(dt);
}

static const float SKYBOXSIZE = 1500.f;
static const float translateLength = SKYBOXSIZE / 2;
void c_LevelOne::updateEnviromentCollision()
{
	front.getOBB()->defaultData();
	left.getOBB()->defaultData();
	right.getOBB()->defaultData();
	back.getOBB()->defaultData();
	car.getOBB()->defaultData();
	AI.getOBB()->defaultData();
	boost.getOBB()->defaultData();
	slow.getOBB()->defaultData();

	//Front Skybox
	front.updatePos(0, 0, translateLength);
	front.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	front.getOBB()->calcNewAxis(180.f, 1, 0, 0);
	front.getOBB()->calcNewAxis(180.f, 0, 0, 1);

	//Left Skybox
	left.updatePos(-translateLength, 0, 0);
	left.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	left.getOBB()->calcNewAxis(90.f, 0, 1, 0);

	//Right Skybox
	right.updatePos(translateLength, 0, 0);
	right.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	right.getOBB()->calcNewAxis(-90.f, 0, 1, 0);

	//Back Skybox
	back.updatePos(0, 0, -translateLength);
	back.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
}

void c_LevelOne::Render()
{
	front.getOBB()->defaultData();
	left.getOBB()->defaultData();
	right.getOBB()->defaultData();
	back.getOBB()->defaultData();
	car.getOBB()->defaultData();
	AI.getOBB()->defaultData();
	boost.getOBB()->defaultData();
	slow.getOBB()->defaultData();
	FinishLine.getOBB()->defaultData();

	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	//Define the view/ camera lookat and load the view matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(CamPosX, CamPosY , CamPosZ, CamTargetX, CamTargetY , CamTargetZ, 0, 1, 0);
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() *viewStack.Top()*modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	//----Render Functions----------------------------------//
	renderLights();
	renderEnviroment();
	updateEnviromentCollision();
	if (Random == 1)
	{
		renderRain();
	}
	if (Random == 2)
	{
		RenderSnow();
	}
	//------------------------------------------------------//
	/**************************************************************		CAR		***************************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(car.getPos().x, car.getPos().y, car.getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(car.GetSteeringAngle(), 0, 1, 0);
	RenderMesh(car.getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	car.updatePos(car.getPos().x, car.getPos().y, car.getPos().z);
	car.getOBB()->calcNewAxis(90, 0, 1, 0);
	car.getOBB()->calcNewAxis(car.GetSteeringAngle(), 0, 1, 0);
	
	/**************************************************************		AI		***************************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(AI.getPos().x, AI.getPos().y, AI.getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(AI.GetTurning(), 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(AI.getMesh(), true);
	modelStack.PopMatrix();

	AI.updatePos(AI.getPos().x, AI.getPos().y, AI.getPos().z);
	AI.getOBB()->calcNewAxis(AI.GetSteeringAngle(), 0, 1, 0);

	/**************************************************************		BoostPad		***************************************************************/

	modelStack.PushMatrix();
	modelStack.Translate(boost.getPos().x, boost.getPos().y, boost.getPos().z);
	modelStack.Scale(3, 1, 3);
	RenderMesh(boost.getMesh(), true);
	modelStack.PopMatrix();

	boost.updatePos(boost.getPos().x, boost.getPos().y, boost.getPos().z);
	boost.getOBB()->calcNewDimensions(3, 1, 3);

	/**************************************************************		SlowPad		***************************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(FinishLine.getPos().x, FinishLine.getPos().y, FinishLine.getPos().z);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(50, 15, 50);
	RenderMesh(FinishLine.getMesh(), true);
	modelStack.PopMatrix();

	FinishLine.updatePos(FinishLine.getPos().x, FinishLine.getPos().y, FinishLine.getPos().z);
	FinishLine.getOBB()->calcNewDimensions(50, 15, 50);

	modelStack.PushMatrix();
	modelStack.Translate(slow.getPos().x, slow.getPos().y, slow.getPos().z);
	modelStack.Scale(3, 1, 3);
	RenderMesh(slow.getMesh(), true);
	modelStack.PopMatrix();

	slow.updatePos(slow.getPos().x, slow.getPos().y, slow.getPos().z);
	slow.getOBB()->calcNewDimensions(3, 1, 3);
    
	CountdownCut = std::to_string(Countdown);
	CountdownCut.resize(1);

	//----Render Text On Screen-----------------------------------------------------------------------------------//
	RenderTextOnScreen(meshList[TEXT], std::to_string(car.GetSpeed()), Color(1, 0, 0), 3, 1, 3);
	RenderTextOnScreen(meshList[TEXT], std::to_string(car.GetAcceleration()), Color(1, 0, 0), 3, 1, 2);
	RenderTextOnScreen(meshList[TEXT], std::to_string(car.GetMaxAcceleration()), Color(1, 0, 0), 3, 1, 1);

	if (Countdown >= 0)
		RenderTextOnScreen(meshList[TEXT], CountdownCut, Color(1, 0, 0), 4, 10, 14);
	else
	{
		Cooldown++;
		elapedTimeCut = std::to_string(elapsedTime);
		elapedTimeCut.resize(5);

		if (Cooldown <= 50)
			RenderTextOnScreen(meshList[TEXT], "START", Color(1, 0, 0), 4, 10, 14);
		else
			RenderTextOnScreen(meshList[TEXT], elapedTimeCut, Color(1, 0, 0), 4, 10, 14);
	}

	RenderTextOnScreen(meshList[TEXT], std::to_string(laps), Color(1, 0, 0), 3, 9, 3);

	if (Win)
		RenderTextOnScreen(meshList[TEXT], "YOU WIN", Color(1, 0, 0), 4, 10, 10);
	if (Lose)
		RenderTextOnScreen(meshList[TEXT], "YOU LOSE", Color(1, 0, 0), 4, 10, 10);

	elapedTimeCut = std::to_string(elapsedTime);
	elapedTimeCut.resize(5);
	RenderTextOnScreen(meshList[TEXT], elapedTimeCut, Color(1, 0, 0), 3, 1, 19);
	//RenderTextOnScreen(meshList[TEXT], std::to_string(car.GetSpeed()), Color(1, 0, 0), 3, 1, 3);
//	RenderTextOnScreen(meshList[TEXT], std::to_string(car.GetAcceleration()), Color(1, 0, 0), 3, 1, 2);
	//RenderTextOnScreen(meshList[TEXT], std::to_string(car.GetMaxAcceleration()), Color(1, 0, 0), 3, 1, 1);
	RenderTextOnScreen(meshList[TEXT], std::to_string(FPS), Color(1, 0, 0), 3, 15, 15);
	//----------------------------------------------------------------------------------------------------------//
}

void c_LevelOne::renderRain()
{
	for (int i = 0; i < rain.getX().size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(rain.getX().at(i), rain.getY().at(i), rain.getZ().at(i));
		modelStack.Translate(rain.getTranslateX(), rain.getTranslateY(), rain.getTranslateZ());
		modelStack.Translate(car.getPos().x, car.getPos().y, car.getPos().z);
		modelStack.Rotate(45, 0, 0, 1);
		modelStack.Scale(0.1f, 0.5f, 0.1f);
		RenderMesh(meshList[RAIN], true);
		modelStack.PopMatrix();
	}
	Raining = true;
}

void c_LevelOne::RenderSnow()
{
	for (int i = 0; i < snow.getX().size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(snow.getX().at(i), snow.getY().at(i), snow.getZ().at(i));
		modelStack.Translate(snow.getTranslateX() / 3, snow.getTranslateY() / 3, snow.getTranslateZ() / 3);
		modelStack.Translate(car.getPos().x, car.getPos().y, car.getPos().z);
		modelStack.Rotate(45, 0, 0, 1);
		modelStack.Scale(0.1f, 0.5f, 0.1f);
		RenderMesh(meshList[SNOW], true);
		modelStack.PopMatrix();
	}
	Snowing = true;
}


void c_LevelOne::renderEnviroment()
{
	/****************************************************Skybox*****************************************************/

	//Front Skybox
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, translateLength);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(front.getMesh(), false);
	modelStack.PopMatrix();

	//Top Skybox
	modelStack.PushMatrix();
	modelStack.Translate(0, translateLength, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[TOP], false);
	modelStack.PopMatrix();

	//Bottom Skybox
	modelStack.PushMatrix();
	modelStack.Translate(0, -translateLength, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[BOTTOM], false);
	modelStack.PopMatrix();

	//Left Skybox
	modelStack.PushMatrix();
	modelStack.Translate(-translateLength, 0, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(left.getMesh(), false);
	modelStack.PopMatrix();

	//Right Skybox
	modelStack.PushMatrix();
	modelStack.Translate(translateLength, 0, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(right.getMesh(), false);
	modelStack.PopMatrix();

	//Back Skybox
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -translateLength);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(back.getMesh(), false);
	modelStack.PopMatrix();
	/***************************************************************************************************************/

	//Track
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(6, 1, 6);
	RenderMesh(meshList[TRACK], false);
	modelStack.PopMatrix();

	//RaceBanner
	modelStack.PushMatrix();
	modelStack.Translate(0, -3, 0);
	modelStack.Scale(5, 5, 6);
	RenderMesh(meshList[RACEBANNER], true);
	modelStack.PopMatrix();

	//StreetLight
	modelStack.PushMatrix();
	modelStack.Translate(0, -3, 0);
	modelStack.Scale(6, 5, 6);
	RenderMesh(meshList[STREETLIGHT], true);
	modelStack.PopMatrix();
}



void c_LevelOne::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    /*modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);*/
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
void c_LevelOne::RenderText(Mesh* mesh, std::string text, Color color, float spacing)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.45f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}
void c_LevelOne::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
	{
		return;
	}

	glDisable(GL_DEPTH_TEST);

	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.67f, 0, 0); // 1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6); // always 6 because a quad is made of 6 vertices
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
void c_LevelOne::initLights()
{
	/***********************************************	Light 1		***************************************************************************/
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(1.f, -16.f, 25.f);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	/***********************************************	Light 2		***************************************************************************/
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	
	lights[1].type = Light::LIGHT_POINT;
	lights[1].position.Set(59.f, 30.8f, -22.3f);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 2;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(30));
	lights[1].cosInner = cos(Math::DegreeToRadian(20));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	/***********************************************	Light 3		***************************************************************************/
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	lights[2].type = Light::LIGHT_POINT;
	lights[2].position.Set(398.f, 30.8, -169.3f);
	lights[2].color.Set(1, 1, 1);
	lights[2].power = 2;
	lights[2].kC = 1.f;
	lights[2].kL = 0.01f;
	lights[2].kQ = 0.001f;
	lights[2].cosCutoff = cos(Math::DegreeToRadian(30));
	lights[2].cosInner = cos(Math::DegreeToRadian(20));
	lights[2].exponent = 3.f;
	lights[2].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], lights[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], lights[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], lights[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], lights[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], lights[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], lights[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], lights[2].exponent);

	/***********************************************	Light 4		***************************************************************************/
	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

	lights[3].type = Light::LIGHT_POINT;
	lights[3].position.Set(387.f, 30.8f, -563.3f);
	lights[3].color.Set(1, 1, 1);
	lights[3].power = 2;
	lights[3].kC = 1.f;
	lights[3].kL = 0.01f;
	lights[3].kQ = 0.001f;
	lights[3].cosCutoff = cos(Math::DegreeToRadian(30));
	lights[3].cosInner = cos(Math::DegreeToRadian(20));
	lights[3].exponent = 3.f;
	lights[3].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], lights[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &lights[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], lights[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], lights[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], lights[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], lights[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], lights[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], lights[3].exponent);
	
	/***********************************************	Light 5		***************************************************************************/
	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");

	lights[4].type = Light::LIGHT_POINT;
	lights[4].position.Set(92.f, 29.8f, -215.3f);
	lights[4].color.Set(1, 1, 1);
	lights[4].power = 2;
	lights[4].kC = 1.f;
	lights[4].kL = 0.01f;
	lights[4].kQ = 0.001f;
	lights[4].cosCutoff = cos(Math::DegreeToRadian(30));
	lights[4].cosInner = cos(Math::DegreeToRadian(20));
	lights[4].exponent = 3.f;
	lights[4].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT4_TYPE], lights[4].type);
	glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &lights[4].color.r);
	glUniform1f(m_parameters[U_LIGHT4_POWER], lights[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], lights[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], lights[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], lights[4].kQ);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], lights[4].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], lights[4].cosInner);
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], lights[4].exponent);

	/***********************************************	Light 6		***************************************************************************/
	m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
	m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
	m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
	m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
	m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
	m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
	m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");
	m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
	m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
	m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
	m_parameters[U_LIGHT5_EXPONENT] = glGetUniformLocation(m_programID, "lights[5].exponent");

	lights[5].type = Light::LIGHT_POINT;
	lights[5].position.Set(-249.f, 30.8f, -266.3f);
	lights[5].color.Set(1, 1, 1);
	lights[5].power = 2;
	lights[5].kC = 1.f;
	lights[5].kL = 0.01f;
	lights[5].kQ = 0.001f;
	lights[5].cosCutoff = cos(Math::DegreeToRadian(30));
	lights[5].cosInner = cos(Math::DegreeToRadian(20));
	lights[5].exponent = 3.f;
	lights[5].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT5_TYPE], lights[5].type);
	glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &lights[5].color.r);
	glUniform1f(m_parameters[U_LIGHT5_POWER], lights[5].power);
	glUniform1f(m_parameters[U_LIGHT5_KC], lights[5].kC);
	glUniform1f(m_parameters[U_LIGHT5_KL], lights[5].kL);
	glUniform1f(m_parameters[U_LIGHT5_KQ], lights[5].kQ);
	glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], lights[5].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT5_COSINNER], lights[5].cosInner);
	glUniform1f(m_parameters[U_LIGHT5_EXPONENT], lights[5].exponent);

	/*******************************************************************************************************************************************/
}
void c_LevelOne::renderLights()
{
	/***********************************************	Light 1		***************************************************************************/
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	
	/***********************************************	Light 2		***************************************************************************/
	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	
	/***********************************************	Light 3		***************************************************************************/
	if (lights[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[2].position.x, lights[2].position.y, lights[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
			Position lightPosition_cameraspace = viewStack.Top() * lights[2].position;
			glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	}

	/***********************************************	Light 4		***************************************************************************/
	if (lights[3].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[3].position.x, lights[3].position.y, lights[3].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[3].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[3].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
	}
	
	/***********************************************	Light 5		***************************************************************************/
	if (lights[4].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[4].position.x, lights[4].position.y, lights[4].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[4].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[4].position;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[4].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[4].position;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
	}
	
	/***********************************************	Light 6		***************************************************************************/
	if (lights[5].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[5].position.x, lights[5].position.y, lights[5].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[5].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[5].position;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[5].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[5].position;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
	}
	/******************************************************************************************************************************************/
	//Enviroment Light
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	modelStack.Scale(0.001f, 0.001f, 0.001f);
	RenderMesh(meshList[LIGHT1], false);
	modelStack.PopMatrix();

	//Street Lights
	for (int i = 1; i < 6; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(lights[i].position.x, lights[i].position.y, lights[i].position.z);
		modelStack.Scale(1.6f, 0.7f, 1.6f);
		RenderMesh(meshList[LIGHT2], false);
		modelStack.PopMatrix();
	}
}
void c_LevelOne::updateLights(int num)
{
	if (num == 0)
	{
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[num].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[num].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[num].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], lights[num].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], lights[num].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], lights[num].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[num].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[num].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[num].exponent);
	}
	if (num == 1)
	{
		glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[num].type);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[num].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], lights[num].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], lights[num].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], lights[num].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], lights[num].kQ);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[num].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[num].cosInner);
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[num].exponent);
	}
	if (num == 2)
	{
		glUniform1i(m_parameters[U_LIGHT2_TYPE], lights[num].type);
		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[num].color.r);
		glUniform1f(m_parameters[U_LIGHT2_POWER], lights[num].power);
		glUniform1f(m_parameters[U_LIGHT2_KC], lights[num].kC);
		glUniform1f(m_parameters[U_LIGHT2_KL], lights[num].kL);
		glUniform1f(m_parameters[U_LIGHT2_KQ], lights[num].kQ);
		glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], lights[num].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT2_COSINNER], lights[num].cosInner);
		glUniform1f(m_parameters[U_LIGHT2_EXPONENT], lights[num].exponent);
	}
	if (num == 3)
	{
		glUniform1i(m_parameters[U_LIGHT3_TYPE], lights[num].type);
		glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &lights[num].color.r);
		glUniform1f(m_parameters[U_LIGHT3_POWER], lights[num].power);
		glUniform1f(m_parameters[U_LIGHT3_KC], lights[num].kC);
		glUniform1f(m_parameters[U_LIGHT3_KL], lights[num].kL);
		glUniform1f(m_parameters[U_LIGHT3_KQ], lights[num].kQ);
		glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], lights[num].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT3_COSINNER], lights[num].cosInner);
		glUniform1f(m_parameters[U_LIGHT3_EXPONENT], lights[num].exponent);
	}
	if (num == 4)
	{
		glUniform1i(m_parameters[U_LIGHT4_TYPE], lights[num].type);
		glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &lights[num].color.r);
		glUniform1f(m_parameters[U_LIGHT4_POWER], lights[num].power);
		glUniform1f(m_parameters[U_LIGHT4_KC], lights[num].kC);
		glUniform1f(m_parameters[U_LIGHT4_KL], lights[num].kL);
		glUniform1f(m_parameters[U_LIGHT4_KQ], lights[num].kQ);
		glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], lights[num].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT4_COSINNER], lights[num].cosInner);
		glUniform1f(m_parameters[U_LIGHT4_EXPONENT], lights[num].exponent);
	}
	if (num == 5)
	{
		glUniform1i(m_parameters[U_LIGHT5_TYPE], lights[num].type);
		glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &lights[num].color.r);
		glUniform1f(m_parameters[U_LIGHT5_POWER], lights[num].power);
		glUniform1f(m_parameters[U_LIGHT5_KC], lights[num].kC);
		glUniform1f(m_parameters[U_LIGHT5_KL], lights[num].kL);
		glUniform1f(m_parameters[U_LIGHT5_KQ], lights[num].kQ);
		glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], lights[num].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT5_COSINNER], lights[num].cosInner);
		glUniform1f(m_parameters[U_LIGHT5_EXPONENT], lights[num].exponent);
	}
}

void c_LevelOne::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
