#include "c_Continue.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"
#include "Utility.h"
#include "LoadTGA.h"

#include "c_ObjectManager.h"
#include "c_DataManager.h"
#include "c_SceneManager.h"

#include "c_FirstCar.h"
#include "c_SecondCar.h"
#include "c_ThirdCar.h"


c_Continue::c_Continue()
{
}


c_Continue::~c_Continue()
{
}

void c_Continue::Init()
{
	ArrowY = 1;
	bounceTime = 0;
	elapsedTime = 0;
	levelNum = 0;
	noFile = false;

	Save1 = LoadTGA("Image//Save1.tga");
	Save2 = LoadTGA("Image//Save2.tga");
	Save3 = LoadTGA("Image//Save3.tga");

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
	camera.Init(Vector3(0, 1, 15), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//Initialize all meshes to NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	//Set projection to Perspective and load projection matrix
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);

	meshList[TEXT] = MeshBuilder::GenerateText("text", (unsigned int)16, (unsigned int)16);
	meshList[TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[FILESELECTION] = MeshBuilder::GenerateQuad("File Selection", Color(1.f, 0.f, 0.f), 10.f);
	meshList[FILESELECTION]->textureID = Save1;
	meshList[NOSAVEFILE] = MeshBuilder::GenerateQuad("NoSaveFile", Color(1.f, 0.f, 0.f), 10.f);
	meshList[NOSAVEFILE]->textureID = LoadTGA("Image//NoSave.tga");
}
void c_Continue::Update(double dt)
{		

	c_SceneManager* scene = c_SceneManager::getInstance();
	elapsedTime += dt;

	if (Application::IsKeyPressed(VK_RETURN))
		scene->updateState("NPC");
	else 
		updateSelection(dt);

}
void c_Continue::Render()
{
	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	//Define the view/ camera lookat and load the view matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() *viewStack.Top()*modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	renderLights();
	if (!checkEmpty())
	{
		renderSelection();
		if(noFile)
			RenderTextOnScreen(meshList[TEXT], "Nothing to load", Color(0.f, 1.f, 0.f), 3.f, 9.f, 18.f);
	}
	else
		RenderMesh(meshList[NOSAVEFILE], false);

}
void c_Continue::Exit()
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
void c_Continue::RenderMesh(Mesh *mesh, bool enableLight)
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
void c_Continue::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void c_Continue::initLights()
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
	light[0].position.Set(-5.f, -4.f, 4.f);
	light[0].color.Set(1, 1, 1);
	light[0].power = 2.f;
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
void c_Continue::renderLights()
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
void c_Continue::updateLights(int num)
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

void c_Continue::renderSelection()
{
	RenderMesh(meshList[FILESELECTION], false);
}
void c_Continue::updateSelection(double dt)
{
	c_ObjectManager* OBJmanager = c_ObjectManager::getInstance();
	c_DataManager* data = c_DataManager::getInstance();
	c_SceneManager* scene = c_SceneManager::getInstance();

	if (Application::IsKeyPressed(VK_UP) && bounceTime < elapsedTime)
	{
		noFile = false;
		ArrowY -= 1;
		if (ArrowY < 1)
			ArrowY = 3;

		if (ArrowY == 1)
			meshList[FILESELECTION]->textureID = Save1;
		else if (ArrowY == 2)
			meshList[FILESELECTION]->textureID = Save2;
		else if (ArrowY == 3)
			meshList[FILESELECTION]->textureID = Save3;

		bounceTime = elapsedTime + 0.125f;
	}
	if (Application::IsKeyPressed(VK_DOWN) && bounceTime < elapsedTime)
	{
		noFile = false;
		ArrowY += 1;
		if (ArrowY > 3)
			ArrowY = 1;

		if (ArrowY == 1)
			meshList[FILESELECTION]->textureID = Save1;
		else if (ArrowY == 2)
			meshList[FILESELECTION]->textureID = Save2;
		else if (ArrowY == 3)
			meshList[FILESELECTION]->textureID = Save3;

		bounceTime = elapsedTime + 0.125f;
	}
	if (Application::IsKeyPressed(VK_SPACE) && bounceTime < elapsedTime)
	{
		if (ArrowY == 1)
			data->selectFile(1);
		else if (ArrowY == 2)
			data->selectFile(2);
		else if (ArrowY == 3)
			data->selectFile(3);

		if (data->isEmpty())
			noFile = true;
		else
		{
			loadFile();
			const char* obj = OBJpath.c_str();
			OBJmanager->addCanCollide("player1", obj, TGApath.c_str(), (0.f, 0.f, 0.f));
			scene->getScene("GARAGE")->Init();
			scene->getScene(scene->getLevel())->Init();
			scene->updateState(scene->getLevel());
		}
		bounceTime = elapsedTime + 0.125f;
	}
}
void c_Continue::loadFile()
{
	c_DataManager* data = c_DataManager::getInstance();

	data->readFromFile(OBJpath,TGApath);
}
bool c_Continue::checkEmpty()
{
	c_DataManager* data = c_DataManager::getInstance();
	
	data->selectFile(1);
	if (data->isEmpty())
	{
		data->selectFile(2);
		if (data->isEmpty())
		{
			data->selectFile(3);
			if (data->isEmpty())
				return true;
		}
	}
	return false;
}

void c_Continue::resetVar()
{

}