#include "c_MultiplayerLevel.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"
#include "Utility.h"
#include "LoadTGA.h"


c_MultiplayerLevel::c_MultiplayerLevel()
{
}


c_MultiplayerLevel::~c_MultiplayerLevel()
{
}
void c_MultiplayerLevel::Init()
{
	playerOneCamPosX = playerOne.getPos().x + 1;
	playerOneCamPosY = playerOne.getPos().y + 1;
	playerOneCamPosZ = playerOne.getPos().z + 1;
	playerOneCamTargetX = playerOne.getPos().x;
	playerOneCamTargetY = playerOne.getPos().y;
	playerOneCamTargetZ = playerOne.getPos().z;

	playerTwoCamPosX = playerTwo.getPos().x + 1;
	playerTwoCamPosY = playerTwo.getPos().y + 1;
	playerTwoCamPosZ = playerTwo.getPos().z + 1;
	playerTwoCamTargetX = playerTwo.getPos().x;
	playerTwoCamTargetY = playerTwo.getPos().y;
	playerTwoCamTargetZ = playerTwo.getPos().z;

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

	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID,
		"material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID,
		"material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID,
		"material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID,
		"material.kShininess");

	initLights();

	//Text
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID,
		"textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
		"textColor");
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

	meshList[TRACK] = MeshBuilder::GenerateOBJ("race track", "OBJ//RaceTrack.obj");
	meshList[TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1), 1.f);
	meshList[TOP]->textureID = LoadTGA("Image//NpcTop.tga");
	meshList[BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[BOTTOM]->textureID = LoadTGA("Image//NpcBottom.tga");

	front.init("front", "quad", "Image//NpcFront.tga", (0, 0, 0));
	left.init("left", "quad", "Image//NpcLeft.tga", (0, 0, 0));
	right.init("right", "quad", "Image//NpcRight.tga", (0, 0, 0));
	back.init("back", "quad", "Image//NpcBack.tga", (0, 0, 0));

	playerOne.init("player1");
	playerTwo.init("player2");
	playerTwo.updatePos(10, 0, 5);
	meshList[CARAXIS] = MeshBuilder::GenerateAxes("Axis", 100, 100, 100);
}
void c_MultiplayerLevel::Update(double dt)
{
	playerOneCamPosX = (playerOne.getPos().x - (sin(Math::DegreeToRadian(playerOne.GetSteeringAngle()))) * 10);
	playerOneCamPosY = playerOne.getPos().y + 8;
	playerOneCamPosZ = (playerOne.getPos().z - (cos(Math::DegreeToRadian(playerOne.GetSteeringAngle()))) * 10);
	playerOneCamTargetX = playerOne.getPos().x;
	playerOneCamTargetY = playerOne.getPos().y + 5;
	playerOneCamTargetZ = playerOne.getPos().z;

	playerTwoCamPosX = (playerTwo.getPos().x - (sin(Math::DegreeToRadian(playerTwo.GetSteeringAngle()))) * 10);
	playerTwoCamPosY = playerTwo.getPos().y + 8;
	playerTwoCamPosZ = (playerTwo.getPos().z - (cos(Math::DegreeToRadian(playerTwo.GetSteeringAngle()))) * 10);
	playerTwoCamTargetX = playerTwo.getPos().x;
	playerTwoCamTargetY = playerTwo.getPos().y + 5;
	playerTwoCamTargetZ = playerTwo.getPos().z;

	playerOneCam.Update(dt);
	playerTwoCam.Update(dt);

	playerOne.updatePos(playerOne.getPos().x, playerOne.getPos().y, playerOne.getPos().z);
	playerTwo.updatePos(playerTwo.getPos().x, playerTwo.getPos().y, playerTwo.getPos().z);

	playerOne.Movement(dt);
	playerTwo.Movement(dt);
}
void c_MultiplayerLevel::Render()
{
	glEnable(GL_SCISSOR_TEST);

	glViewport(0, 0, 960, 1080);
	glScissor(0, 0, 960, 1080);
	renderPlayerOne();

	glViewport(960, 0, 960, 1080);
	glScissor(960, 0, 960, 1080);
	renderPlayerTwo();

	glDisable(GL_SCISSOR_TEST);
}
void c_MultiplayerLevel::Exit()
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
void c_MultiplayerLevel::RenderMesh(Mesh *mesh, bool enableLight)
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
void c_MultiplayerLevel::initLights()
{
	//Light1
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID,
		"lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID,
		"lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID,
		"lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID,
		"lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID,
		"lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID,
		"lights[0].exponent");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID,
		"colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, -0.5f, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1.f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(25));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	//********************************************************************************************************************
}
void c_MultiplayerLevel::renderLights()
{
	//Lights 1

	modelStack.LoadIdentity();
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	//********************************************************************************************************************
}
void c_MultiplayerLevel::updateLights(int num)
{
	if (num == 0)
	{
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[num].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[num].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[num].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[num].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[num].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[num].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[num].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[num].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[num].exponent);
	}
}

void c_MultiplayerLevel::renderPlayerOne()
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

	renderEnviroment();
	updateEnviromentCollision();
	/****************************************************	PlayerOne	*****************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(playerOne.getPos().x, playerOne.getPos().y, playerOne.getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(playerOne.GetSteeringAngle(), 0, 1, 0);
	RenderMesh(playerOne.getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	playerOne.updatePos(playerOne.getPos().x, playerOne.getPos().y, playerOne.getPos().z);
	playerOne.getOBB()->calcNewAxis(90, 0, 1, 0);
	playerOne.getOBB()->calcNewAxis(playerOne.GetSteeringAngle(), 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Translate(playerOne.getPos().x, playerOne.getPos().y, playerOne.getPos().z);
	modelStack.Rotate(playerOne.GetSteeringAngle(), 0, 1, 0);
	RenderMesh(meshList[CARAXIS], false);
	modelStack.PopMatrix();

	/****************************************************	PlayerTwo	*****************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(playerTwo.getPos().x, playerTwo.getPos().y, playerTwo.getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(playerTwo.GetSteeringAngle(), 0, 1, 0);
	RenderMesh(playerTwo.getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	playerTwo.updatePos(playerTwo.getPos().x, playerTwo.getPos().y, playerTwo.getPos().z);
	playerTwo.getOBB()->calcNewAxis(90, 0, 1, 0);
	playerTwo.getOBB()->calcNewAxis(playerTwo.GetSteeringAngle(), 0, 1, 0);
}
void c_MultiplayerLevel::renderPlayerTwo()
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

	renderEnviroment();
	updateEnviromentCollision();
	/****************************************************	PlayerOne	*****************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(playerOne.getPos().x, playerOne.getPos().y, playerOne.getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(playerOne.GetSteeringAngle(), 0, 1, 0);
	RenderMesh(playerOne.getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	playerOne.updatePos(playerOne.getPos().x, playerOne.getPos().y, playerOne.getPos().z);
	playerOne.getOBB()->calcNewAxis(90, 0, 1, 0);
	playerOne.getOBB()->calcNewAxis(playerOne.GetSteeringAngle(), 0, 1, 0);

	/****************************************************	PlayerTwo	*****************************************************/
	modelStack.PushMatrix();
	modelStack.Translate(playerTwo.getPos().x, playerTwo.getPos().y, playerTwo.getPos().z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(playerTwo.GetSteeringAngle(), 0, 1, 0);
	RenderMesh(playerTwo.getMesh(), true);
	modelStack.PopMatrix();

	//UpdateCollisions
	playerTwo.updatePos(playerTwo.getPos().x, playerTwo.getPos().y, playerTwo.getPos().z);
	playerTwo.getOBB()->calcNewAxis(90, 0, 1, 0);
	playerTwo.getOBB()->calcNewAxis(playerTwo.GetSteeringAngle(), 0, 1, 0);
}

static const float SKYBOXSIZE = 1500.f;
static const float translateLength = SKYBOXSIZE / 2;
void c_MultiplayerLevel::renderEnviroment()
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
	modelStack.Scale(6, 1, 6);
	RenderMesh(meshList[TRACK], false);
	modelStack.PopMatrix();
}
void c_MultiplayerLevel::updateEnviromentCollision()
{
	front.getOBB()->defaultData();
	left.getOBB()->defaultData();
	right.getOBB()->defaultData();
	back.getOBB()->defaultData();
	playerOne.getOBB()->defaultData();
	playerTwo.getOBB()->defaultData();

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