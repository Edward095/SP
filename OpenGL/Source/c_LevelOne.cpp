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


c_LevelOne::c_LevelOne()
{
}


c_LevelOne::~c_LevelOne()
{
}

void c_LevelOne::Init()
{
	CamPosX = car.getPos().x + 1;
	CamPosY = car.getPos().y + 1;
	CamPosZ = car.getPos().z + 1;
	CamTargetX = car.getPos().x;
	CamTargetY = car.getPos().y;
	CamTargetZ = car.getPos().z;
	elapsedTime = 0;
	FreezeTime = 0;
	duration = 0;

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
	

	//initialization of the Enums
	/*meshList[FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[FRONT]->textureID = LoadTGA("Image//NpcFront.tga");

	

	

	meshList[LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[LEFT]->textureID = LoadTGA("Image//NpcLeft.tga");

	meshList[RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[RIGHT]->textureID = LoadTGA("Image//NpcRight.tga");

	meshList[BACK] = MeshBuilder::GenerateQuad("Back", Color(1, 1, 1), 1.f);
	meshList[BACK]->textureID = LoadTGA("Image//NpcBack.tga");*/

	meshList[TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1), 1.f);
	meshList[TOP]->textureID = LoadTGA("Image//NpcTop.tga");
	meshList[BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[BOTTOM]->textureID = LoadTGA("Image//NpcBottom.tga");
	//texutre for obj later

	meshList[TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[TRACK] = MeshBuilder::GenerateOBJ("race track", "OBJ//RaceTrack.obj");

	front.init("front", "quad", "Image//NpcFront.tga", (0, 0, 0));
	//top.init("top", "quad", "Image//NpcTop.tga", (0, 0, 0));
	//bottom.init("bottom", "quad", "Image//NpcBottom.tga", (0, 0, 0));
	left.init("left", "quad", "Image//NpcLeft.tga", (0, 0, 0));
	right.init("right", "quad", "Image//NpcRight.tga", (0, 0, 0));
	back.init("back", "quad", "Image//NpcBack.tga", (0, 0, 0));

	car.init("player1","OBJ//Car1Body.obj","Image//Car1Blue.tga", Vector3(0, 0, 0));
	nitro.init("Nitro","OBJ//Car1Body.obj", "Image//Car1Blue.tga", Vector3(6, 0, 6));
	//RenderMesh(car.getMesh(), true);

	//Initialization of Variables
	
}
void c_LevelOne::Update(double dt)
{
	elapsedTime += dt;
	FreezeTime  = (dt + (dt* 0));

	if (Application::IsKeyPressed('F'))
		Freeze = true;

	if (Freeze && duration <= 150)
	{
		duration++;
		elapsedTime -= FreezeTime;

		if (duration >= 150) // 3 sec/dt
			Freeze = false;
	}

	

	CamPosX = (car.getPos().x - (sin(Math::DegreeToRadian(car.GetSteeringAngle()))) * 5);
	CamPosY = car.getPos().y + 8;
	CamPosZ = (car.getPos().z - (cos(Math::DegreeToRadian(car.GetSteeringAngle()))) * 5);
	CamTargetX = car.getPos().x;
	CamTargetY = car.getPos().y + 5;
	CamTargetZ = car.getPos().z;

	camera.Update(dt); 
  
	car.updatePos(car.getPos().x, car.getPos().y, car.getPos().z);
	car.Movement(dt);
	
	if (car.getPos().x == nitro.getPos().x && car.getPos().z == nitro.getPos().z)
	{
		car.PowerUp(true);
	}
}


static const float SKYBOXSIZE = 1500.f;
static const float translateLength = SKYBOXSIZE / 2;

void c_LevelOne::Render()
{
	front.getOBB()->defaultData();
	left.getOBB()->defaultData();
	right.getOBB()->defaultData();
	back.getOBB()->defaultData();

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

	//Skybox
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, translateLength);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(front.getMesh(), false);
	modelStack.PopMatrix();

	//UpdateCollisions
	front.updatePos(0,0, translateLength);
	front.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	front.getOBB()->calcNewAxis(180.f, 1, 0, 0);
	front.getOBB()->calcNewAxis(180.f, 0, 0, 1);


	modelStack.PushMatrix();
	modelStack.Translate(0, translateLength, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[TOP], false);
	modelStack.PopMatrix();

	////UpdateCollisions
	//top.updatePos(0,100,0);
	//top.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	//top.getOBB()->calcNewAxis(90.f, (1, 0, 0));

	modelStack.PushMatrix();
	modelStack.Translate(0, -translateLength, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-translateLength, 0, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(left.getMesh(), false);
	modelStack.PopMatrix();

	//UpdateCollisions
	left.updatePos(-translateLength,0,0);
	left.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	left.getOBB()->calcNewAxis(90.f, 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Translate(translateLength, 0, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(right.getMesh(), false);
	modelStack.PopMatrix();

	//UpdateCollisions
	right.updatePos(translateLength,0,0);
	right.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	right.getOBB()->calcNewAxis(-90.f, 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -translateLength);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	//modelStack.Rotate(-180, 0, 1, 0);
	RenderMesh(back.getMesh(), false);
	modelStack.PopMatrix();

	//UpdateCollisions
	back.updatePos(0,0,-translateLength);
	back.getOBB()->calcNewDimensions(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(6, 1, 6);
	RenderMesh(meshList[TRACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(car.getPos().x, car.getPos().y, car.getPos().z);
	//modelStack.Scale(1, 1, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(car.GetSteeringAngle(), 0, 1, 0);
	RenderMesh(car.getMesh(), true);
	//RenderMesh(meshList[CAR1], false);
	modelStack.PopMatrix();

	//UpdateCollisions
	car.updatePos(car.getPos().x, car.getPos().y, car.getPos().z);
	car.getOBB()->calcNewAxis(90, 0, 1, 0);
	car.getOBB()->calcNewAxis(car.GetSteeringAngle(), 0, 1, 0);
	
	modelStack.PushMatrix();
	modelStack.Translate(nitro.getPos().x, 0, nitro.getPos().z);
	modelStack.Scale(0.8, 0.8, 0.8);
	RenderMesh(nitro.getMesh(), true);
	//RenderMesh(meshList[NITRO], false);
	modelStack.PopMatrix();
	
	elapedTimeCut = std::to_string(elapsedTime);
	elapedTimeCut.resize(5);
	RenderTextOnScreen(meshList[TEXT], elapedTimeCut, Color(1, 0, 0), 3, 1, 19);
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
void c_LevelOne::RenderMesh(Mesh *mesh, bool enableLight)
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
void c_LevelOne::initLights()
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
void c_LevelOne::renderLights()
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
void c_LevelOne::updateLights(int num)
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
/*
time+=dt;

std::string time2;

time2 = std::to_string(time);

4.54543553
4.5
rendertoscreen(time[0],);
rendertoscreen(time[1],);
rendertoscreen(time[2],);
*/