#include "c_Npc.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"
#include "Utility.h"
#include "LoadTGA.h"


bool c_Npc::MultiPlayer = false;
bool c_Npc::SinglePlayer = false;
bool c_Npc::Level1 = false;
bool c_Npc::Level2 = false;
bool c_Npc::Level3 = false;


c_Npc::c_Npc()
{
}


c_Npc::~c_Npc()
{
}

void c_Npc::Init()
{
	Garage.Init();
	//LevelOne.Init();
	e_GameState_NPC = _NPC;

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
	camera.Init(Vector3(0, 0, 220));

	//Initialize all meshes to NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	//Set projection to Perspective and load projection matrix
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);
	

	//initialization of the Enums
	meshList[FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[FRONT]->textureID = LoadTGA("Image//NpcFront.tga");

	meshList[TOP] = MeshBuilder::GenerateQuad("Top", Color(1, 1, 1), 1.f);
	meshList[TOP]->textureID = LoadTGA("Image//NpcTop.tga");

	meshList[BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[BOTTOM]->textureID = LoadTGA("Image//NpcBottom.tga");

	meshList[LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[LEFT]->textureID = LoadTGA("Image//NpcLeft.tga");

	meshList[RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[RIGHT]->textureID = LoadTGA("Image//NpcRight.tga");

	meshList[BACK] = MeshBuilder::GenerateQuad("Back", Color(1, 1, 1), 1.f);
	meshList[BACK]->textureID = LoadTGA("Image//NpcBack.tga");

	meshList[NPC] = MeshBuilder::GenerateOBJ("Npc", "OBJ//NpcHuman.obj");
	//texutre for obj later

	meshList[TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[TEXT]->textureID = LoadTGA("Image//calibri.tga");





	//Initialization of Variables
	ElapsedTime = 0;
	TimePassed = 0;
	ArrowY = 7;
	BounceTime = 0;
	
	//booleans
	AbleToPress = false;
	Talk = false;
	LevelSelection = false;
	Level1 = false;
	Level2 = false;
	Level3 = false;
	SinglePlayer = false;
	MultiPlayer = false;

}
void c_Npc::Update(double dt)
{
	ElapsedTime += dt;
	camera.Update(dt);
	

	switch (e_GameState_NPC)
	{
	case _NPC:
		UpdateNpc(dt);
		break;
	case GARAGE:
		Garage.Update(dt);
		break;
	case LEVEL1:
		LevelOne.Update(dt);
	/*case LEVEL2:
		LevelTwo.Update(dt);*/
	}
	
}


static const float SKYBOXSIZE = 500.f;

void c_Npc::Render()
{
	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	//Define the view/ camera lookat and load the view matrix
	viewStack.LoadIdentity();
	//viewStack
	viewStack.LoadMatrix(camera.LookAt());
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() *viewStack.Top()*modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	
	if (e_GameState_NPC == _NPC)
	{
		RenderNpc();
	}
	else if (e_GameState_NPC == GARAGE)
	{
		Garage.Render();
	}
	else if (e_GameState_NPC == LEVEL1)
	{
		LevelOne.Render();
	}
	/*else if (e_GameState_NPC == LEVEL2)
	{
		LevelTwo.Render();
	}*/


	


}

void c_Npc::UpdateNpc(double dt)
{
	if (Application::IsKeyPressed('F') && camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == false)
	{
		Talk = true;
		LevelSelection = false;
	}
	
	
	if (Application::IsKeyPressed(VK_DOWN) && BounceTime < ElapsedTime && camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168)
	{
		ArrowY--;
		if (LevelSelection == false)
		{
			if (ArrowY < 6)
			{
				ArrowY = 7;
			}
		}
		if (LevelSelection == true)
		{
			if (ArrowY < 5)
			{
				ArrowY = 7;
			}
		}
		BounceTime = ElapsedTime + 0.125;
	}
	if (Application::IsKeyPressed(VK_UP) && BounceTime < ElapsedTime && camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168)
	{
		ArrowY++;
		if (LevelSelection == false)
		{
			if (ArrowY > 7)
			{
				ArrowY = 6;
			}
		}
		if (LevelSelection == true)
		{
			if (ArrowY > 7)
			{
				ArrowY = 5;
			}
		}
		BounceTime = ElapsedTime + 0.125;
	}
	if (Application::IsKeyPressed(VK_SPACE) && BounceTime < ElapsedTime && camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && AbleToPress == true)
	{
		BounceTime = ElapsedTime + 0.125;
		if (LevelSelection == false)
		{
			if (ArrowY == 7)
			{
				SinglePlayer = true;
				LevelSelection = true;
			}
			else if (ArrowY == 6)
			{
				MultiPlayer = true;
				LevelSelection = true;
			}
		}
		else if (LevelSelection == true)
		{
			if (ArrowY == 7)
			{
				Level1 = true;
				e_GameState_NPC = GARAGE;
				Garage.Init();
				Garage.Update(dt);
			}
			else if (ArrowY == 6)
			{
				Level2 = true;
				e_GameState_NPC = GARAGE;
				Garage.Init();
				Garage.Update(dt);
			}
			else if (ArrowY == 5)
			{
				Level3 = true;
				e_GameState_NPC = GARAGE;
				Garage.Init();
				Garage.Update(dt);
			}
		}
	}


}

void c_Npc::RenderNpc()
{
	//Human OBJ
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, -80);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(150, -50, -80);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-150, -50, -80);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();


	//Skybox
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 250);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 250, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -250, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-250, 0, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(250, 0, 0);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -250);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	//modelStack.Rotate(-180, 0, 1, 0);
	RenderMesh(meshList[BACK], false);
	modelStack.PopMatrix();

	//Text for name of NPC
	modelStack.PushMatrix();
	modelStack.Translate(-180, 55, -75);
	modelStack.Scale(20, 20, 20);
	RenderText(meshList[TEXT], "New Game", Color(0, 0, 0), 1);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(-35, 55, -75);
	modelStack.Scale(20, 20, 20);
	RenderText(meshList[TEXT], "Continue", Color(0, 0, 0), 1);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(125, 55, -75);
	modelStack.Scale(20, 20, 20);
	RenderText(meshList[TEXT], "Options", Color(0, 0, 0), 1);
	modelStack.PopMatrix();

	//text for talking to NPC
	modelStack.PushMatrix();
	int cameraX = static_cast<int>(camera.position.x);
	int cameraY = static_cast<int>(camera.position.y);
	int cameraZ = static_cast<int>(camera.position.z);
	RenderTextOnScreen(meshList[TEXT], std::to_string(cameraX), Color(0, 0, 1), 3, 1, 19);
	RenderTextOnScreen(meshList[TEXT], std::to_string(cameraY), Color(0, 0, 1), 3, 1, 18);
	RenderTextOnScreen(meshList[TEXT], std::to_string(cameraZ), Color(0, 0, 1), 3, 1, 17);

	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == false)
	{
		RenderTextOnScreen(meshList[TEXT], "Press 'F' to talk to NPC", Color(1, 0, 0), 3, 6, 10);
	}
	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == true && LevelSelection == false)
	{
		RenderTextOnScreen(meshList[TEXT], "Choose a GameMode", Color(1, 0, 0), 3, 9, 13);
		AbleToPress = true;
	}
	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == true && LevelSelection == false)
	{
		RenderTextOnScreen(meshList[TEXT], "SinglePlayer", Color(1, 0, 0), 5, 7, 7);
		AbleToPress = true;
	}
	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == true && LevelSelection == false)
	{
		RenderTextOnScreen(meshList[TEXT], "MultiPlayer", Color(1, 0, 0), 5, 7, 6);
		AbleToPress = true;
	}
	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == true && LevelSelection == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Choose a Level", Color(1, 0, 0), 3, 9, 13);
		AbleToPress = true;
	}
	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == true)
	{
		RenderTextOnScreen(meshList[TEXT], ">", Color(1, 0, 0), 5, 5, ArrowY);
		AbleToPress = true;
	}
	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == true && LevelSelection == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Level 1", Color(1, 0, 0), 5, 7, 7);
		AbleToPress = true;
	}
	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == true && LevelSelection == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Level 2", Color(1, 0, 0), 5, 7, 6);
		AbleToPress = true;
	}
	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < -116 && camera.position.x > -168 && Talk == true && LevelSelection == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Level 3", Color(1, 0, 0), 5, 7, 5);
		AbleToPress = true;
	}

	if (camera.position.z < 80 && camera.position.z > -10 && camera.position.y > -10 && camera.position.y < 40 && camera.position.x < 40 && camera.position.x > -40)
	{
		RenderTextOnScreen(meshList[TEXT], "Continue Game?", Color(1, 0, 0), 3, 9, 13);
	}


}
void c_Npc::Exit()
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
void c_Npc::RenderMesh(Mesh *mesh, bool enableLight)
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
void c_Npc::initLights()
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
	light[0].position.Set(-5, 7, 0);
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
void c_Npc::renderLights()
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
void c_Npc::updateLights(int num)
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


void c_Npc::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void c_Npc::RenderText(Mesh* mesh, std::string text, Color color, float spacing)
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
		characterSpacing.SetToTranslation(i * 0.6f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}





bool c_Npc::GetLevel1()
{
	return Level1;
}


bool c_Npc::GetLevel2()
{
	return Level2;
}



bool c_Npc::GetLevel3()
{
	return Level3;
}



bool c_Npc::GetSinglePlayer()
{
	return SinglePlayer;
}


bool c_Npc::GetMultiPlayer()
{
	return MultiPlayer;
}

