#include "c_MultiplayerLevelTwo.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"
#include "Utility.h"
#include "LoadTGA.h"

#include "c_FirstCar.h"
#include "c_SecondCar.h"
#include "c_ThirdCar.h"


c_MultiplayerLevelTwo::c_MultiplayerLevelTwo()
{
}


c_MultiplayerLevelTwo::~c_MultiplayerLevelTwo()
{
}
void c_MultiplayerLevelTwo::Init()
{
	OBJmanager = c_ObjectManager::getInstance();
	offRoadManager = c_OffRoadManager::getInstance();

	c_Entity* car = OBJmanager->getCanCollide("player1");

	c_FirstCar* first = dynamic_cast <c_FirstCar*>(car);
	if (first)
		playerOne = first;
	c_SecondCar* second = dynamic_cast <c_SecondCar*>(car);
	if (second)
		playerOne = second;
	c_ThirdCar* third = dynamic_cast <c_ThirdCar*>(car);
	if (third)
		playerOne = third;

	car = OBJmanager->getCanCollide("player2");

	first = dynamic_cast <c_FirstCar*>(car);
	if (first)
		playerTwo = first;
	second = dynamic_cast <c_SecondCar*>(car);
	if (second)
		playerTwo = second;
	third = dynamic_cast <c_ThirdCar*>(car);
	if (third)
		playerTwo = third;
	playerTwo->updatePos(-10, 0, 0);

	playerOneCamPosX = playerOne->getPos().x + 1;
	playerOneCamPosY = playerOne->getPos().y + 1;
	playerOneCamPosZ = playerOne->getPos().z + 1;
	playerOneCamTargetX = playerOne->getPos().x;
	playerOneCamTargetY = playerOne->getPos().y;
	playerOneCamTargetZ = playerOne->getPos().z;

	playerTwoCamPosX = playerTwo->getPos().x + 1;
	playerTwoCamPosY = playerTwo->getPos().y + 1;
	playerTwoCamPosZ = playerTwo->getPos().z + 1;
	playerTwoCamTargetX = playerTwo->getPos().x;
	playerTwoCamTargetY = playerTwo->getPos().y;
	playerTwoCamTargetZ = playerTwo->getPos().z;

	// Set background color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);
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

	// Get a handle for our "MVP" uniform
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
	playerOneCam.Init(Vector3(0, 8, 5), Vector3(0, 1, 0), Vector3(0, 1, 0));
	playerTwoCam.Init(Vector3(0, 8, 5), Vector3(0, 1, 0), Vector3(0, 1, 0));

	//Initialize all meshes to NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	//Set projection to Perspective and load projection matrix
	Mtx44 projection;
	projection.SetToPerspective(60.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);

	meshList[TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1), 1.f);
	meshList[TOP]->textureID = LoadTGA("Image//SunnyTop.tga");
	meshList[BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[BOTTOM]->textureID = LoadTGA("Image//SunnyBottom.tga");
	meshList[TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[LIGHT1] = MeshBuilder::GenerateSphere("environment light", Color(1, 1, 1), 18, 36, 1.f);
	meshList[LIGHT2] = MeshBuilder::GenerateSphere("street light", Color(1, 1, 1), 18, 36, 1.f);
	meshList[RACEBANNER] = MeshBuilder::GenerateOBJ("race banner", "OBJ//RaceBanner.obj");
	meshList[STREETLIGHT] = MeshBuilder::GenerateOBJ("street light", "OBJ//Streetlamp.obj");
	meshList[STREETLIGHT]->textureID = LoadTGA("Image//Streetlamp.tga");

	front.init("front", "quad", "Image//SunnyFront.tga", (0, 0, 0), true);
	left.init("left", "quad", "Image//SunnyLeft.tga", (0, 0, 0), true);
	right.init("right", "quad", "Image//SunnyRight.tga", (0, 0, 0), true);
	back.init("back", "quad", "Image/SunnyBack.tga", (0, 0, 0), true);
	track.init("track", "OBJ//RaceTrack2.obj", "Image//RaceTrack.tga", Vector3(0, 0, 0), false);
	PickUp.init("Pickup", "OBJ//Pad.obj", "Image//Car1Blue.tga", Vector3(0, 1, 50), false);
	speedometer.init("speedometer", "quad", "Image//speedometer.tga", (float)(1, 1, 1), false);
	needle.init("needle", "quad", "Image//needle.tga", (float)(1, 1, 1), false);
	circle.init("circle", "quad", "Image//circle.tga", (float)(1, 1, 1), false);

	meshList[CARAXIS] = MeshBuilder::GenerateAxes("Axis", 100, 100, 100);
}
void c_MultiplayerLevelTwo::Update(double dt)
{
	playerOneCamPosX = (playerOne->getPos().x - (sin(Math::DegreeToRadian(playerOne->GetSteeringAngle()))) * 10);
	playerOneCamPosY = playerOne->getPos().y + 8;
	playerOneCamPosZ = (playerOne->getPos().z - (cos(Math::DegreeToRadian(playerOne->GetSteeringAngle()))) * 10);
	playerOneCamTargetX = playerOne->getPos().x;
	playerOneCamTargetY = playerOne->getPos().y + 5;
	playerOneCamTargetZ = playerOne->getPos().z;

	playerTwoCamPosX = (playerTwo->getPos().x - (sin(Math::DegreeToRadian(playerTwo->GetSteeringAngle()))) * 10);
	playerTwoCamPosY = playerTwo->getPos().y + 8;
	playerTwoCamPosZ = (playerTwo->getPos().z - (cos(Math::DegreeToRadian(playerTwo->GetSteeringAngle()))) * 10);
	playerTwoCamTargetX = playerTwo->getPos().x;
	playerTwoCamTargetY = playerTwo->getPos().y + 5;
	playerTwoCamTargetZ = playerTwo->getPos().z;

	playerOneCam.Update(dt);
	playerTwoCam.Update(dt);

	playerOne->updatePos(playerOne->getPos().x, playerOne->getPos().y, playerOne->getPos().z);
	playerTwo->updatePos(playerTwo->getPos().x, playerTwo->getPos().y, playerTwo->getPos().z);

	playerOne->Movement(dt);
	playerTwo->Movement(dt);

	//if (Countdown <= 0)
	//{
	//	elapsedTime += (float)dt;
	//	playerOne->Movement(dt);
	//	playerTwo->Movement(dt);
	//	playerOne->Ability(dt);
	//	playerTwo->Ability(dt);

	//}

	if (Application::IsKeyPressed('Q') && checkFO)
	{
		OFreeze = true;
	}

	if (OFreeze && Oduration <= 150)
	{
		Oduration++;
		playerTwo->SetTSlowed(true);
	}

	if (Oduration >= 150) // 3 sec/dt
	{
		OFreeze = false;
		playerTwo->SetTSlowed(false);
		Oduration = 0;
	}

	//--------------------------------------------------//
	if (Application::IsKeyPressed('P') && checkFT)
	{
		TFreeze = true;
	}

	if (TFreeze && Tduration <= 150)
	{
		Tduration++;
		playerOne->SetOSlowed(true);
	}

	if (Tduration >= 150) // 3 sec/dt
	{
		TFreeze = false;
		playerOne->SetOSlowed(false);
		Tduration = 0;
	}

	/*if ((playerOne->gotCollide("Pickup", false)) || (playerTwo->gotCollide("Pickup", false)))
	{
		pick = true;
		Raining = false;
		Snowing = false;
	}

	if (!pick)
	{
		rain.update(dt);
		snow.update(dt);
	}*/

}
void c_MultiplayerLevelTwo::Render()
{
	glEnable(GL_SCISSOR_TEST);

	glViewport(0, 0, 960, 1080);
	glScissor(0, 0, 960, 1080);
	renderPlayerOne();

	glViewport(960, 0, 960, 1080);
	glScissor(960, 0, 960, 1080);
	renderPlayerTwo();

	glDisable(GL_SCISSOR_TEST);

	elapedTimeCut = std::to_string(elapsedTime);
	elapedTimeCut.resize(5);
	RenderTextOnScreen(meshList[TEXT], elapedTimeCut, Color(1, 0, 0), 3, 1, 19);

}
void c_MultiplayerLevelTwo::Exit()
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
void c_MultiplayerLevelTwo::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE,
			&modelView_inverse_transpose.a[0]);
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
void c_MultiplayerLevelTwo::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void c_MultiplayerLevelTwo::initLights()
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
	lights[0].power = 0.f;
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
void c_MultiplayerLevelTwo::renderLights()
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
void c_MultiplayerLevelTwo::updateLights(int num)
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

void c_MultiplayerLevelTwo::renderPlayerOne()
{
	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	//Define the view/ camera lookat and load the view matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(playerOneCamPosX, playerOneCamPosY, playerOneCamPosZ, playerOneCamTargetX, playerOneCamTargetY, playerOneCamTargetZ, 0, 1, 0);
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() *viewStack.Top()*modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	renderLights();
	renderEnviroment();
	updateEnviromentCollision();
	/****************************************************	PlayerOne	*****************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(playerOne->getPos().x, playerOne->getPos().y, playerOne->getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(playerOne->GetSteeringAngle(), 0, 1, 0);
	RenderMesh(playerOne->getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	playerOne->updatePos(playerOne->getPos().x, playerOne->getPos().y, playerOne->getPos().z);
	playerOne->getOBB()->calcNewAxis(90, 0, 1, 0);
	playerOne->getOBB()->calcNewAxis(playerOne->GetSteeringAngle(), 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Translate(playerOne->getPos().x, playerOne->getPos().y, playerOne->getPos().z);
	modelStack.Rotate(playerOne->GetSteeringAngle(), 0, 1, 0);
	RenderMesh(meshList[CARAXIS], false);
	modelStack.PopMatrix();

	/****************************************************	PlayerTwo	*****************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(playerTwo->getPos().x, playerTwo->getPos().y, playerTwo->getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(playerTwo->GetSteeringAngle(), 0, 1, 0);
	RenderMesh(playerTwo->getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	playerTwo->updatePos(playerTwo->getPos().x, playerTwo->getPos().y, playerTwo->getPos().z);
	playerTwo->getOBB()->calcNewAxis(90, 0, 1, 0);
	playerTwo->getOBB()->calcNewAxis(playerTwo->GetSteeringAngle(), 0, 1, 0);

	//RenderTextOnScreen(meshList[TEXT], std::to_string(playerOne->GetSpeed()), Color(1, 0, 0), 3, 1, 3);
	//RenderTextOnScreen(meshList[TEXT], std::to_string(playerOne->GetAcceleration()), Color(1, 0, 0), 3, 1, 2);
	//RenderTextOnScreen(meshList[TEXT], std::to_string(playerOne->GetMaxAcceleration()), Color(1, 0, 0), 3, 1, 1);
	RenderSpeedometerOne();
}
void c_MultiplayerLevelTwo::renderPlayerTwo()
{
	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	//Define the view/ camera lookat and load the view matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(playerTwoCamPosX, playerTwoCamPosY, playerTwoCamPosZ, playerTwoCamTargetX, playerTwoCamTargetY, playerTwoCamTargetZ, 0, 1, 0);
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() *viewStack.Top()*modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	renderLights();
	renderEnviroment();
	updateEnviromentCollision();
	/****************************************************	PlayerOne	*****************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(playerOne->getPos().x, playerOne->getPos().y, playerOne->getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(playerOne->GetSteeringAngle(), 0, 1, 0);
	RenderMesh(playerOne->getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	playerOne->updatePos(playerOne->getPos().x, playerOne->getPos().y, playerOne->getPos().z);
	playerOne->getOBB()->calcNewAxis(90, 0, 1, 0);
	playerOne->getOBB()->calcNewAxis(playerOne->GetSteeringAngle(), 0, 1, 0);

	/****************************************************	PlayerTwo	*****************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(playerTwo->getPos().x, playerTwo->getPos().y, playerTwo->getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(playerTwo->GetSteeringAngle(), 0, 1, 0);
	RenderMesh(playerTwo->getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	playerTwo->updatePos(playerTwo->getPos().x, playerTwo->getPos().y, playerTwo->getPos().z);
	playerTwo->getOBB()->calcNewAxis(90, 0, 1, 0);
	playerTwo->getOBB()->calcNewAxis(playerTwo->GetSteeringAngle(), 0, 1, 0);

	//RenderTextOnScreen(meshList[TEXT], std::to_string(playerTwo->GetSpeed()), Color(1, 0, 0), 3, 1, 3);
	//RenderTextOnScreen(meshList[TEXT], std::to_string(playerTwo->GetAcceleration()), Color(1, 0, 0), 3, 1, 2);
	//RenderTextOnScreen(meshList[TEXT], std::to_string(playerTwo->GetMaxAcceleration()), Color(1, 0, 0), 3, 1, 1);
	RenderSpeedometerTwo();
}

static const float SKYBOXSIZE = 1500.f;
static const float translateLength = SKYBOXSIZE / 2;
void c_MultiplayerLevelTwo::renderEnviroment()
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
	modelStack.Translate(-310.951f, 0, -135.453f);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(track.getMesh(), false);
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
void c_MultiplayerLevelTwo::updateEnviromentCollision()
{
	front.getOBB()->defaultData();
	left.getOBB()->defaultData();
	right.getOBB()->defaultData();
	back.getOBB()->defaultData();
	track.getOBB()->defaultData();
	playerOne->getOBB()->defaultData();
	playerTwo->getOBB()->defaultData();
	//AI.getOBB()->defaultData();
	//boost.getOBB()->defaultData();
	//slow.getOBB()->defaultData();

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

	//track
	track.updatePos(-310.951f, 0, -135.453f);
	track.getOBB()->calcNewAxis(90, 0, 1, 0);
}

void c_MultiplayerLevelTwo::RenderSpeedometerOne()
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(9, 11, 0);
	modelStack.Scale(12, 12, 12);
	RenderMesh(speedometer.getMesh(), false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(9, 11, 1);
	modelStack.Scale(9, 9, 9);
	RenderMesh(circle.getMesh(), false);

	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(9, 11, 2);
	modelStack.Rotate(220, 0, 0, 1); //Velocity 0 = 220, Ve20 = 198, Ve40 = 176 etc.
	modelStack.Rotate(-playerOne->GetSpedoSpeed(), 0, 0, 1);
	modelStack.Scale(7, 7, 7);
	RenderMesh(needle.getMesh(), false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void c_MultiplayerLevelTwo::RenderSpeedometerTwo()
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(9, 11, 0);
	modelStack.Scale(12, 12, 12);
	RenderMesh(speedometer.getMesh(), false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(9, 11, 1);
	modelStack.Scale(9, 9, 9);
	RenderMesh(circle.getMesh(), false);

	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(9, 11, 2);
	modelStack.Rotate(220, 0, 0, 1); //Velocity 0 = 220, Ve20 = 198, Ve40 = 176 etc.
	modelStack.Rotate(-playerTwo->GetSpedoSpeed(), 0, 0, 1);
	modelStack.Scale(7, 7, 7);
	RenderMesh(needle.getMesh(), false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}