#include "c_Npc.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"
#include "Utility.h"
#include "LoadTGA.h"

#include "c_DataManager.h"
#include "c_SceneManager.h"


c_Npc::c_Npc()
{
}


c_Npc::~c_Npc()
{
}

void c_Npc::Init()
{
	//Seed Generation For rand() function
	srand(time(NULL));

	c_SceneManager* scene = c_SceneManager::getInstance();
	c_DataManager* data = c_DataManager::getInstance();
	Audio = c_Sound::getInstance();

	float volume = data->getSoundOptions();
	Audio->f_AdjustMusicVolume(volume);
	Audio->f_AdjustSFXVolume(volume);

	scene->updateState("NPC");

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
	camera.Init(Vector3(0, 0, 10));

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
	meshList[NPC]->textureID = LoadTGA("Image//NpcTex.tga");

	meshList[TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[TEXT]->textureID = LoadTGA("Image//calibri.tga");
	
	meshList[HOUSE] = MeshBuilder::GenerateOBJ("House", "OBJ//House.obj");
	meshList[HOUSE]->textureID = LoadTGA("Image//House.tga");

	meshList[INSTRUCTIONS] = MeshBuilder::GenerateQuad("Instructions", Color(1, 1, 1), 100);
	meshList[INSTRUCTIONS]->textureID = LoadTGA("Image//Instructiontex.tga");



	//Initialization of Variables
	ElapsedTime = 0;
	TimePassed = 0;
	ArrowY = 7;
	BounceTime = 0;
	

	Audio->f_Start_Menu_music();

	//booleans
	AbleToPress = false;
	Talk = false;
	Talk1 = false;
	Talk2 = false;
	Talk3 = false;
	Talk4 = false;
	Talk5 = false;
	LevelSelection = false;
	SinglePlayer = false;
	MultiPlayer = false;
	StartGame = false;
	Continue = false;
	Options = false;
	LeaderBoard = false;
	Override = false;
}
void c_Npc::Update(double dt)
{
	c_SceneManager* scene = c_SceneManager::getInstance();

	ElapsedTime += dt;
	camera.Update(dt);
	camera.WBmove(dt, 300.f, -450.f, 218.f, -150.f);

	if (scene->checkState("NPC"))
		UpdateNpc(dt);
	else if (scene->checkState("CONTINUE"))
		scene->getScene("CONTINUE")->Update(dt);
	else if (scene->checkState("FINISHED"))
		scene->getScene("FINISHED")->Update(dt);
	else
		scene->getScene("GARAGE")->Update(dt);
		//Garage.Update(dt);

	
}


static const float SKYBOXSIZE = 500.f;

void c_Npc::Render()
{
	c_SceneManager* scene = c_SceneManager::getInstance();

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

	
	if (scene->checkState("NPC"))
		RenderNpc();
	else if (scene->checkState("CONTINUE"))
		scene->getScene("CONTINUE")->Render();
	else if (scene->checkState("FINISHED"))
		scene->getScene("FINISHED")->Render();
	else
		scene->getScene("GARAGE")->Render();
}

void c_Npc::UpdateNpc(double dt)
{
	c_SceneManager* scene = c_SceneManager::getInstance();
	c_DataManager* data = c_DataManager::getInstance();

	if (camera.position.z < -10 && camera.position.z > -100 && camera.position.y < 200 && camera.position.x < -310 && camera.position.x > -380)
	{
		StartGame = true;
		Continue = false;
		SinglePlayer = true;
		MultiPlayer = false;
		Options = false;
		LeaderBoard = false;
		Instructions = false;
	
	}
	else
	{
		Override = false;
		StartGame = false;
		LevelSelection = false;
		Talk = false;
		SinglePlayer = false;
	}
	if (camera.position.z < -10 && camera.position.z > -100 && camera.position.y < 200 && camera.position.x < -50 && camera.position.x > -150)
	{
		StartGame = false;
		Continue = true;
		SinglePlayer = false;
		MultiPlayer = false;
		Options = false;
		LeaderBoard = false;
		Instructions = false;
	}
	else
	{
		Continue = false;
		Talk3 = false;
	}
	if (camera.position.z < -10 && camera.position.z > -100 && camera.position.y < 200 && camera.position.x < 200 && camera.position.x > 110)
	{
		StartGame = false;
		Continue = false;
		SinglePlayer = false;
		MultiPlayer = true;
		Options = false;
		LeaderBoard = false;
		Instructions = false;
	}
	else
	{
		MultiPlayer = false;
		Talk2 = false;
	}
	if (camera.position.z < 100 && camera.position.z > 13 && camera.position.y < 200 && camera.position.x < 240 && camera.position.x > 150)
	{
		StartGame = false;
		Continue = false;
		SinglePlayer = false;
		MultiPlayer = false;
		Options = true;
		LeaderBoard = false;
		Instructions = false;
	}
	else
	{
		Options = false;
		Talk1 = false;
	}
	if (camera.position.z < 100 && camera.position.z > 13 && camera.position.y < 200 && camera.position.x < -280 && camera.position.x > -370)
	{
		StartGame = false;
		Continue = false;
		SinglePlayer = false;
		MultiPlayer = false;
		Options = false;
		LeaderBoard = true;
		Instructions = false;
	}
	else
	{
		LeaderBoard = false;
		Talk4 = false;		
	}
	if (camera.position.z < 110 && camera.position.z > 40 && camera.position.y < 200 && camera.position.x < -66 && camera.position.x > -136)
	{
		StartGame = false;
		Continue = false;
		SinglePlayer = false;
		MultiPlayer = false;
		Options = false;
		LeaderBoard = false;
		Instructions = true;

	}
	else
	{
		Instructions = false;
		Talk5 = false;
	}

	if ((Application::IsKeyPressed('F') && StartGame == true && Talk == false) || (Application::IsKeyPressed('F') && Options == true && Talk1 == false) || (Application::IsKeyPressed('F') && MultiPlayer == true && Talk2 == false) || (Application::IsKeyPressed('F') && Continue == true && Talk3 == false) || (Application::IsKeyPressed('F') && LeaderBoard == true && Talk4 == false) || (Application::IsKeyPressed('F') && Instructions == true && Talk5 == false))
	{
		Talk = true;
		Talk1 = true;
		Talk2 = true;
		Talk3 = true;
		Talk4 = true;
		Talk5 = true;
	}
	
	
	if ((Application::IsKeyPressed(VK_DOWN) && BounceTime < ElapsedTime && StartGame == true) || (Application::IsKeyPressed(VK_DOWN) && BounceTime < ElapsedTime && Options == true) || (Application::IsKeyPressed(VK_DOWN) && BounceTime < ElapsedTime && Talk2 == true) || (Application::IsKeyPressed(VK_DOWN) && BounceTime < ElapsedTime && Continue == true) || (Application::IsKeyPressed(VK_DOWN) && BounceTime < ElapsedTime && Override == true))
	{
		Audio->f_Menu_MoveSelect();
		ArrowY--;
		if (Continue == true || Override == true)
		{
			if (ArrowY < 6)
			{
				ArrowY = 7;
			}
		}
		if ((StartGame == true) || MultiPlayer == true)
		{
			if (ArrowY < 5)
			{
				ArrowY = 7;
			}
		}
		if (Options == true)
		{
			if (ArrowY < 4)
			{
				ArrowY = 7;
			}
		}
		BounceTime = ElapsedTime + 0.125;
	}
	if ((Application::IsKeyPressed(VK_UP) && BounceTime < ElapsedTime && StartGame == true) || (Application::IsKeyPressed(VK_UP) && BounceTime < ElapsedTime && Options == true) || (Application::IsKeyPressed(VK_UP) && BounceTime < ElapsedTime && Talk2 == true) || (Application::IsKeyPressed(VK_UP) && BounceTime < ElapsedTime && Continue == true) || (Application::IsKeyPressed(VK_UP) && BounceTime < ElapsedTime && Override == true))
	{
	
		ArrowY++;
		Audio->f_Menu_MoveSelect();

        if (Continue == true || Override == true)
		{
			if (ArrowY > 7)
			{
				ArrowY = 6;
			}
		}
		if ((StartGame == true) || MultiPlayer == true)
		{
			if (ArrowY > 7)
			{
				ArrowY = 5;
			}
		}
		if (Options == true)
		{
			if (ArrowY > 7)
			{
				ArrowY = 4;
			}
		}
		BounceTime = ElapsedTime + 0.125;
	}
	if ((Application::IsKeyPressed(VK_SPACE) && BounceTime < ElapsedTime && StartGame == true && AbleToPress == true) || (Application::IsKeyPressed(VK_SPACE) && BounceTime < ElapsedTime && Options == true && AbleToPress == true) || (Application::IsKeyPressed(VK_SPACE) && BounceTime < ElapsedTime && Talk2 == true) || (Application::IsKeyPressed(VK_SPACE) && BounceTime < ElapsedTime && Continue == true && AbleToPress == true) || (Application::IsKeyPressed(VK_SPACE) && BounceTime < ElapsedTime && Override == true && AbleToPress == true))
	{

		BounceTime = ElapsedTime + 0.125;
		Audio->f_Menu_ConfirmSelect();

		if (LevelSelection == false && StartGame == true && Override == false)
		{
			if (ArrowY == 7)
			{
				data->selectFile(1);
				if(data->isEmpty() == true)
				{
					//if first data save is empty,
					LevelSelection = true;
					Override = false;
				}
				else
				{
					LevelSelection = false;
					Override = true;
				}
				
			}
			else if (ArrowY == 6)
			{
				data->selectFile(2);
				if (data->isEmpty() == true)
				{
					LevelSelection = true;
					Override = false;
				}
				else
				{
					LevelSelection = false;
					Override = true;
				}
			}
			else if (ArrowY == 5)
			{
				data->selectFile(3);
				if (data->isEmpty() == true)
				{
					LevelSelection = true;
					Override = false;
				}
				else
				{
					LevelSelection = false;
					Override = true;
				}
			}
		}
		else if (Override == true && LevelSelection == false && StartGame == true)
		{
			if (ArrowY == 7)
			{
				//yes
				Override = false;
				LevelSelection = true;
				data->clearFile();
			}
			else if (ArrowY == 6)
			{
				//no
				Override = false;
				LevelSelection = false;
				StartGame = true;
			}
		}
		else if ((LevelSelection == true && StartGame == true) || (Talk2 == true))
		{
			if (ArrowY == 7)
			{
				if (SinglePlayer)
					scene->updateLevel("SLEVELONE");
				else if (MultiPlayer)
					scene->updateLevel("MLEVELONE");
				scene->updateState("GARAGE");

				scene->getScene("GARAGE")->Init();
				scene->getScene("GARAGE")->Update(dt);
			}
			else if (ArrowY == 6)
			{
				if (SinglePlayer)
					scene->updateLevel("SLEVELTWO");
				else if (MultiPlayer)
					scene->updateLevel("MLEVELTWO");
				scene->updateState("GARAGE");

				scene->getScene("GARAGE")->Init();
				scene->getScene("GARAGE")->Update(dt);
			}
			else if (ArrowY == 5)
			{
				if (SinglePlayer)
					scene->updateLevel("SLEVELTHREE");
				else if (MultiPlayer)
					scene->updateLevel("MLEVELTHREE");
				scene->updateState("GARAGE");

				scene->getScene("GARAGE")->Init();
				scene->getScene("GARAGE")->Update(dt);
			}
		}
		
		if (Options)
		{
			if (ArrowY == 7)
			{
				//put music here 
				Audio->f_AdjustMusicVolume(1.0f);
				Audio->f_AdjustSFXVolume(1.0f);
				data->saveSoundOptions(1.0f);
			}
			else if (ArrowY == 6)
			{
				Audio->f_AdjustMusicVolume(0.75f);
				Audio->f_AdjustSFXVolume(0.75f);
				data->saveSoundOptions(0.75f);
			}
			else if (ArrowY == 5)
			{
				Audio->f_AdjustMusicVolume(0.5f);
				Audio->f_AdjustSFXVolume(0.5f);
				data->saveSoundOptions(0.5f);
			}
			else if (ArrowY == 4)
			{
				Audio->f_AdjustMusicVolume(0.0f);
				Audio->f_AdjustSFXVolume(0.0f);
				data->saveSoundOptions(0.0f);
			}
		}
		if (Continue)
		{
			if (ArrowY == 7)
			{
				//yes
				scene->updateState("CONTINUE");
				scene->getScene("CONTINUE")->Init();
			}
			else if (ArrowY == 6)
			{
				Continue = false;
				Talk3 = false;
			}
		}
		
		ArrowY = 7;
		BounceTime = ElapsedTime + 0.125;
	}

	
}

void c_Npc::RenderNpc()
{
	c_DataManager* dataManager = c_DataManager::getInstance();
	//Human OBJ
    //Newgame
	modelStack.PushMatrix();
	modelStack.Translate(-350, 10, -200);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();

	//Continue
	modelStack.PushMatrix();
	modelStack.Translate(-100, 10, -200);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();

	//Multiplayer
	modelStack.PushMatrix();
	modelStack.Translate(150, 10, -200);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();

	//Options
	modelStack.PushMatrix();
	modelStack.Translate(300, 10, 50);
	modelStack.Scale(0.5, 0.5, 0.5);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();

	//leaderboard
	modelStack.PushMatrix();
	modelStack.Translate(-450, 10, 50);
	modelStack.Scale(0.5, 0.5, 0.5);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();

	//Instructions
	modelStack.PushMatrix();
	modelStack.Translate(-100, 10, 200);
	modelStack.Scale(0.5, 0.5, 0.5);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[NPC], false);
	modelStack.PopMatrix();


	//Npc House
	RenderMesh(meshList[HOUSE], false);

	
	//Text for name of NPC
	modelStack.PushMatrix();
	modelStack.Translate(-375, 110, -195);
	modelStack.Scale(20, 20, 20);
	RenderText(meshList[TEXT], "New Game", Color(0, 0, 0), 1);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(-130, 110, -195);
	modelStack.Scale(20, 20, 20);
	RenderText(meshList[TEXT], "Continue", Color(0, 0, 0), 1);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(105, 110, -195);
	modelStack.Scale(20, 20, 20);
	RenderText(meshList[TEXT], "Multiplayer", Color(0, 0, 0), 1);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(300, 110, 20);
	modelStack.Scale(20, 20, 20);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderText(meshList[TEXT], "Options", Color(0, 0, 0), 1);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-450, 110, 100);
	modelStack.Scale(20, 20, 20);
	modelStack.Rotate(90, 0, 1, 0);
	RenderText(meshList[TEXT], "LeaderBoard", Color(0, 0, 0), 1);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(-45, 110, 195);
	modelStack.Scale(20, 20, 20);
	modelStack.Rotate(180, 0, 1, 0);
	RenderText(meshList[TEXT], "Instructions", Color(0, 0, 0), 1);
	modelStack.PopMatrix();

	//text for talking to NPC
	modelStack.PushMatrix();
	int cameraX = static_cast<int>(camera.position.x);
	int cameraY = static_cast<int>(camera.position.y);
	int cameraZ = static_cast<int>(camera.position.z);
	RenderTextOnScreen(meshList[TEXT], std::to_string(cameraX), Color(0, 0, 1), 3, 1, 19);
	RenderTextOnScreen(meshList[TEXT], std::to_string(cameraY), Color(0, 0, 1), 3, 1, 18);
	RenderTextOnScreen(meshList[TEXT], std::to_string(cameraZ), Color(0, 0, 1), 3, 1, 17);

	if ((StartGame && !Talk) ||  (Options && !Talk1) || (MultiPlayer && !Talk2) || (Continue && !Talk3) || (LeaderBoard && !Talk4) || (Instructions && !Talk5))
	{
		RenderTextOnScreen(meshList[TEXT], "Press 'F' to talk to NPC", Color(1, 0, 0), 3, 6, 10);
	}
	if (StartGame && Talk && !LevelSelection && !Override)
	{
		RenderTextOnScreen(meshList[TEXT], "Save Files", Color(1, 0, 0), 3, 9, 13);
		AbleToPress = true;
	}
	if ((StartGame && Talk && !LevelSelection && !Override))
	{
		RenderTextOnScreen(meshList[TEXT], "Save File 1", Color(1, 0, 0), 5, 7, 7);
		AbleToPress = true;
	}
	if ((StartGame == true && Talk == true && LevelSelection == false && Override == false))
	{
		RenderTextOnScreen(meshList[TEXT], "Save File 2", Color(1, 0, 0), 5, 7, 6);
		AbleToPress = true;
	}
	if ((StartGame == true && Talk == true && LevelSelection == false && Override == false))
	{
		RenderTextOnScreen(meshList[TEXT], "Save File 3", Color(1, 0, 0), 5, 7, 5);
		AbleToPress = true;
	}
	if ((Continue == true && Talk3 == true) || Override == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Yes", Color(1, 0, 0), 5, 7, 7);
		AbleToPress = true;
	}
	if ((Continue == true && Talk3 == true) || Override == true)
	{
		RenderTextOnScreen(meshList[TEXT], "No", Color(1, 0, 0), 5, 7, 6);
		AbleToPress = true;
	}
	if ((StartGame == true && Talk == true && LevelSelection == true) || (MultiPlayer == true && Talk2 == true))
	{
		RenderTextOnScreen(meshList[TEXT], "Choose a Level", Color(1, 0, 0), 3, 9, 13);
		AbleToPress = true;
	}
	if ((StartGame == true && Talk == true) || (Options == true && Talk1 == true) || (MultiPlayer == true && Talk2 == true) || (Continue == true && Talk3 == true))
	{
		RenderTextOnScreen(meshList[TEXT], ">", Color(1, 0, 0), 5, 5, ArrowY);
		AbleToPress = true;
	}
	if ((StartGame == true && Talk == true && LevelSelection == true) || ( MultiPlayer == true && Talk2 == true))
	{
		RenderTextOnScreen(meshList[TEXT], "Level 1", Color(1, 0, 0), 5, 7, 7);
		AbleToPress = true;
	}
	if ((StartGame == true && Talk == true && LevelSelection == true) || (MultiPlayer == true && Talk2 == true))
	{
		RenderTextOnScreen(meshList[TEXT], "Level 2", Color(1, 0, 0), 5, 7, 6);
		AbleToPress = true;
	}
	if ((StartGame == true && Talk == true && LevelSelection == true) || (MultiPlayer == true && Talk2 == true))
	{
		RenderTextOnScreen(meshList[TEXT], "Level 3", Color(1, 0, 0), 5, 7, 5);
		AbleToPress = true;
	}

	if (Continue == true && Talk3 == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Continue Game?", Color(1, 0, 0), 3, 9, 13);
	}

	if (LeaderBoard == true && Talk4 == true)
	{
		int counter = 13;
		std::vector <float> data;
		std::vector <std::string> name;
		dataManager->getLeaderBoards(data, name);

		RenderTextOnScreen(meshList[TEXT], "LeaderBoard", Color(1, 0, 0), 3, 9, 18);
		for (int i = 0; i < data.size(); i++)
		{
			RenderTextOnScreen(meshList[TEXT], name[i], Color(0, 0, 1), 3, 9, counter);
			RenderTextOnScreen(meshList[TEXT], std::to_string(data[i]), Color(0, 1, 0), 3, 19, counter);
			counter--;
		}
	}
	if (Instructions == true && Talk5 == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-93, 75, 175);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[INSTRUCTIONS], false);
		modelStack.PopMatrix();	
	}

	if (Options == true && Talk1 == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Adjust Volume Of Game Sound", Color(1, 0, 0), 3, 6, 13);
	}

	if (Options == true && Talk1 == true)
	{
		RenderTextOnScreen(meshList[TEXT], "100%", Color(1, 0, 0), 5, 7, 7);
	}
	if (Options == true && Talk1 == true)
	{
		RenderTextOnScreen(meshList[TEXT], "75%", Color(1, 0, 0), 5, 7, 6);
	}
	if (Options == true && Talk1 == true)
	{
		RenderTextOnScreen(meshList[TEXT], "50%", Color(1, 0, 0), 5, 7, 5);
	}
	if (Options == true && Talk1 == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Off", Color(1, 0, 0), 5, 7, 4);
	}
	if (Override == true && LevelSelection == false && StartGame == true)
	{
		RenderTextOnScreen(meshList[TEXT], "Override Data?", Color(1, 0, 0), 3, 9, 13);
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

void c_Npc::resetVar()
{
	AbleToPress = Talk = Talk1 = Talk2 = Talk3 = false;
	LevelSelection = SinglePlayer = MultiPlayer = StartGame = false;
	Continue  = Options = LeaderBoard= false;
}