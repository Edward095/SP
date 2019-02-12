#include "c_Garage.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"
#include "Utility.h"
#include "LoadTGA.h"
// +++++++++++++++++++++++++ CAR CLASS +++++++++++++++++++++++++++

c_Car::c_Car()
{
}

c_Car::~c_Car()
{
}

int c_Car::f_GetCarNum()
{
	return v_CarNum;
}

void c_Car::f_SetCarNum(int carNumber)
{
	this->v_CarNum = carNumber;
}

// +++++++++++++++++++++++++ COLOUR CLASS ++++++++++++++++++++++++

c_Colour::c_Colour()
{

}

c_Colour::~c_Colour()
{

}

int c_Colour::f_GetColourNum()
{
	return v_ColourNum;
}

void c_Colour::f_SetColourNum(int colour)
{
	switch (colour)
	{
	case 0:
		v_ColourNum = Blue;
		break;
	case 1:
		v_ColourNum = Red;
		break;
	case 2:
		v_ColourNum = Purple;
		break;
	case 3:
		v_ColourNum = Green;
		break;
	case 4:
		v_ColourNum = Pink;
		break;
	case 5:
		v_ColourNum = Yellow;
		break;
	case 6:
		v_ColourNum = White;
		break;
	case 7:
		v_ColourNum = Grey;
		break;
	case 8:
		v_ColourNum = Black;
		break;
	default:
		break;
	}
}

// +++++++++++++++++++++ CAR LINK LIST CLASS +++++++++++++++++++++

c_List::c_List()
{
	for (int i = 0; i < 3; i++)
	{
		v_CarList[i] = new c_Car;
		v_CarList[i]->f_SetCarNum(i);
	}
	for (int i = 0; i < 9; i++)
	{
		v_ColourList[i] = new c_Colour;
		v_ColourList[i]->f_SetColourNum(i);
	}
	v_CurColour = v_ColourList[0];
	v_CurCar = v_CarList[0];
	v_IndexCar = 0;
	v_IndexColour = 0;
}

c_List::~c_List()
{

}

void c_List::f_ChangeCurrentCar(char input)
{
	switch (input)
	{
	case 'O':
		v_IndexCar--;
		if (v_IndexCar < 0)
		{
			v_IndexCar = 2;
		}
		break;
	case 'P':
		v_IndexCar++;
		if (v_IndexCar > 2)
		{
			v_IndexCar = 0;
		}
		break;
	default:
		break;
	}
	v_CurCar = v_CarList[v_IndexCar];
}

void c_List::f_ChangeCurrentColour(char input)
{
	switch (input)
	{
	case 'K':
		v_IndexColour--;
		if (v_IndexColour < 0)
		{
			v_IndexColour = 8;
		}
		break;
	case 'L':
		v_IndexColour++;
		if (v_IndexColour > 8)
		{
			v_IndexColour = 0;
		}
		break;
	default:
		break;
	}
	v_CurColour = v_ColourList[v_IndexColour];
}

c_Car* c_List::f_GetCurCar()
{
	return v_CurCar;
}

c_Colour* c_List::f_GetCurColour()
{
	return v_CurColour;
}


// +++++++++++++++++++++++++ GARAGE CLASS ++++++++++++++++++++++++

c_Garage::c_Garage()
{

}

c_Garage::~c_Garage()
{
}

void c_Garage::Init()
{
	v_TGAName = "Image//testColourBlue.tga";

	// Set background color to black
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
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
	//Text
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID,
		"textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
		"textColor");


	initLights();
	//Initialize camera settings
	camera.Init(Vector3(0, 0, 15));

	//Initialize all meshes to NULL
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	//Set projection to Perspective and load projection matrix
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 10000.f);
	projectionStack.LoadMatrix(projection);

	meshList[TEXT] = MeshBuilder::GenerateQuad("MenuText", Color(1, 0, 0), 10);
	meshList[TEXT]->textureID = LoadTGA("Image//Menu.tga");
	meshList[ARROW] = MeshBuilder::GenerateQuad("Arrow", Color(1, 0, 0), 0.7f);
	meshList[ARROW]->textureID = LoadTGA("Image//Arrow.tga");

	meshList[COLOUR_BLUE] = MeshBuilder::GenerateQuad("colourBlue", Color(0, 0, 1), 1);
	meshList[COLOUR_RED] = MeshBuilder::GenerateQuad("colourRed", Color(1, 0, 0), 1);
	meshList[COLOUR_PURPLE] = MeshBuilder::GenerateQuad("colourPurple", Color(1, 0, 1), 1);
	meshList[COLOUR_GREEN] = MeshBuilder::GenerateQuad("colourGreen", Color(0, 1, 0), 1);
	meshList[COLOUR_PINK] = MeshBuilder::GenerateQuad("colourPink", Color(1, 0.412, 0.706), 1);
	meshList[COLOUR_YELLOW] = MeshBuilder::GenerateQuad("colourYellow", Color(1, 1, 0), 1);
	meshList[COLOUR_WHITE] = MeshBuilder::GenerateQuad("colourPink", Color(1, 1, 1), 1);
	meshList[COLOUR_GREY] = MeshBuilder::GenerateQuad("colourGrey", Color(0.5, 0.5, 0.5), 1);
	meshList[COLOUR_BLACK] = MeshBuilder::GenerateQuad("colourBlack", Color(0, 0, 0), 1);

	meshList[CAR1PREVIEW] = MeshBuilder::GenerateQuad("Preview1", Color(1, 1, 1), 2);
	meshList[CAR1PREVIEW]->textureID = LoadTGA("Image//Car1Preview.tga");
	meshList[CAR2PREVIEW] = MeshBuilder::GenerateQuad("Preview2", Color(1, 1, 1), 2);
	meshList[CAR2PREVIEW]->textureID = LoadTGA("Image//Car2Preview.tga");
	meshList[CAR3PREVIEW] = MeshBuilder::GenerateQuad("Preview3", Color(1, 1, 1), 2);
	meshList[CAR3PREVIEW]->textureID = LoadTGA("Image//Car3Preview.tga");

	meshList[car1] = MeshBuilder::GenerateOBJ("car1", "OBJ//testCar1.obj");
	meshList[car1]->textureID = LoadTGA(v_TGAName);

	meshList[car2] = MeshBuilder::GenerateOBJ("car1", "OBJ//testCar2.obj");
	meshList[car2]->textureID = LoadTGA(v_TGAName);

	meshList[car3] = MeshBuilder::GenerateOBJ("car1", "OBJ//testCar3.obj");
	meshList[car3]->textureID = LoadTGA(v_TGAName);

	meshList[GARAGEBG] = MeshBuilder::GenerateQuad("background", Color(0, 0, 0), 17);
	meshList[GARAGEBG]->textureID = LoadTGA("Image//GarageBG.tga");

	v_RotateCar = 0;
	v_ElapsedTime = 0;
	v_BounceTime = 0;
	v_ScaleBlue = v_ScaleRed = v_ScalePurple = v_ScaleGreen = v_ScalePink = v_ScaleYellow = v_ScaleWhite = v_ScaleGrey = v_ScaleBlack = 1;
	v_MoveBlue = v_MoveRed = v_MovePurple = v_MoveGreen = v_MovePink = v_MoveYellow = v_MoveWhite = v_MoveGrey = v_MoveBlack = 0;
	v_ScalePreview1 = v_ScalePreview2 = v_ScalePreview3 = 1;
}

void c_Garage::Update(double dt)
{
	v_ElapsedTime += dt;
	if (Application::IsKeyPressed(VK_DOWN) && v_BounceTime < v_ElapsedTime)
	{
		v_CarList.f_ChangeCurrentCar('O');
		v_BounceTime = v_ElapsedTime + 0.250;
	}
	if (Application::IsKeyPressed(VK_UP) && v_BounceTime < v_ElapsedTime)
	{
		v_CarList.f_ChangeCurrentCar('P');
		v_BounceTime = v_ElapsedTime + 0.250;
	}
	if (Application::IsKeyPressed(VK_LEFT) && v_BounceTime < v_ElapsedTime)
	{
		v_ColourList.f_ChangeCurrentColour('K');
		v_BounceTime = v_ElapsedTime + 0.250;
	}
	if (Application::IsKeyPressed(VK_RIGHT) && v_BounceTime < v_ElapsedTime)
	{
		v_ColourList.f_ChangeCurrentColour('L');
		v_BounceTime = v_ElapsedTime + 0.250;
	}

	f_UpdateCurColour();
	f_UpdateCurCar();

	v_RotateCar += (float)(50 * dt);
}

void c_Garage::RenderMesh(Mesh *mesh, bool enableLight)
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

void c_Garage::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) // error check
	{
		return;
	}
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
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void c_Garage::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) // error check
	{
		return;
	}
	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

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
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void c_Garage::Render()
{
	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	//Define the view/ camera lookat and load the view matrix
	viewStack.LoadMatrix(camera.LookAt());
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() *viewStack.Top()*modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	renderLights();

	modelStack.PushMatrix();
	modelStack.Translate(0.5f, 0, -5);
	modelStack.Scale(1.26f, 1, 1);
	RenderMesh(meshList[GARAGEBG], false);
	modelStack.PopMatrix();

	f_RenderPallet();

	f_RenderPreviews();

	f_RenderFinal();

}

void c_Garage::initLights()
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
	light[0].position.Set(-5, 5, 5);
	light[0].color.Set(1, 1, 1);
	light[0].power = 2.5f;
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
void c_Garage::renderLights()
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
void c_Garage::updateLights(int num)
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

void c_Garage::f_RenderPallet()
{
	modelStack.PushMatrix();
	modelStack.Translate(-4, -4, v_MoveBlue);
	modelStack.Scale(v_ScaleBlue, v_ScaleBlue, v_ScaleBlue);
	RenderMesh(meshList[COLOUR_BLUE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3, -4, v_MoveRed);
	modelStack.Scale(v_ScaleRed, v_ScaleRed, v_ScaleRed);
	RenderMesh(meshList[COLOUR_RED], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2, -4, v_MovePurple);
	modelStack.Scale(v_ScalePurple, v_ScalePurple, v_ScalePurple);
	RenderMesh(meshList[COLOUR_PURPLE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1, -4, v_MoveGreen);
	modelStack.Scale(v_ScaleGreen, v_ScaleGreen, v_ScaleGreen);
	RenderMesh(meshList[COLOUR_GREEN], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -4, v_MovePink);
	modelStack.Scale(v_ScalePink, v_ScalePink, v_ScalePink);
	RenderMesh(meshList[COLOUR_PINK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1, -4, v_MoveYellow);
	modelStack.Scale(v_ScaleYellow, v_ScaleYellow, v_ScaleYellow);
	RenderMesh(meshList[COLOUR_YELLOW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2, -4, v_MoveWhite);
	modelStack.Scale(v_ScaleWhite, v_ScaleWhite, v_ScaleWhite);
	RenderMesh(meshList[COLOUR_WHITE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3, -4, v_MoveGrey);
	modelStack.Scale(v_ScaleGrey, v_ScaleGrey, v_ScaleGrey);
	RenderMesh(meshList[COLOUR_GREY], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(4, -4, v_MoveBlack);
	modelStack.Scale(v_ScaleBlack, v_ScaleBlack, v_ScaleBlack);
	RenderMesh(meshList[COLOUR_BLACK], false);
	modelStack.PopMatrix();

}

void c_Garage::f_UpdateCurColour()
{
	if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 0)
	{
		v_ScaleBlue = 1.5f;
		v_ScaleRed = 1.f;
		v_ScalePurple = 1.f;
		v_ScaleGreen = 1.f;
		v_ScalePink = 1.f;
		v_ScaleYellow = 1.f;
		v_ScaleWhite = 1.f;
		v_ScaleGrey = 1.f;
		v_ScaleBlack = 1.f;

		v_MoveBlue = 0.5f;
		v_MoveRed = 0;
		v_MovePurple = 0;
		v_MoveGreen = 0;
		v_MovePink = 0;
		v_MoveYellow = 0;
		v_MoveWhite = 0;
		v_MoveGrey = 0;
		v_MoveBlack = 0;

	}
	else if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 1)
	{
		v_ScaleBlue = 1.f;
		v_ScaleRed = 1.5f;
		v_ScalePurple = 1.f;
		v_ScaleGreen = 1.f;
		v_ScalePink = 1.f;
		v_ScaleYellow = 1.f;
		v_ScaleWhite = 1.f;
		v_ScaleGrey = 1.f;
		v_ScaleBlack = 1.f;

		v_MoveBlue = 0;
		v_MoveRed = 0.5f;
		v_MovePurple = 0;
		v_MoveGreen = 0;
		v_MovePink = 0;
		v_MoveYellow = 0;
		v_MoveWhite = 0;
		v_MoveGrey = 0;
		v_MoveBlack = 0;

	}
	else if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 2)
	{
		v_ScaleBlue = 1.f;
		v_ScaleRed = 1.f;
		v_ScalePurple = 1.5f;
		v_ScaleGreen = 1.f;
		v_ScalePink = 1.f;
		v_ScaleYellow = 1.f;
		v_ScaleWhite = 1.f;
		v_ScaleGrey = 1.f;
		v_ScaleBlack = 1.f;

		v_MoveBlue = 0;
		v_MoveRed = 0;
		v_MovePurple = 0.5f;
		v_MoveGreen = 0;
		v_MovePink = 0;
		v_MoveYellow = 0;
		v_MoveWhite = 0;
		v_MoveGrey = 0;
		v_MoveBlack = 0;

	}
	else if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 3)
	{
		v_ScaleBlue = 1.f;
		v_ScaleRed = 1.f;
		v_ScalePurple = 1.f;
		v_ScaleGreen = 1.5f;
		v_ScalePink = 1.f;
		v_ScaleYellow = 1.f;
		v_ScaleWhite = 1.f;
		v_ScaleGrey = 1.f;
		v_ScaleBlack = 1.f;

		v_MoveBlue = 0;
		v_MoveRed = 0;
		v_MovePurple = 0;
		v_MoveGreen = 0.5f;
		v_MovePink = 0;
		v_MoveYellow = 0;
		v_MoveWhite = 0;
		v_MoveGrey = 0;
		v_MoveBlack = 0;

	}
	else if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 4)
	{
		v_ScaleBlue = 1.f;
		v_ScaleRed = 1.f;
		v_ScalePurple = 1.f;
		v_ScaleGreen = 1.f;
		v_ScalePink = 1.5f;
		v_ScaleYellow = 1.f;
		v_ScaleWhite = 1.f;
		v_ScaleGrey = 1.f;
		v_ScaleBlack = 1.f;

		v_MoveBlue = 0;
		v_MoveRed = 0;
		v_MovePurple = 0;
		v_MoveGreen = 0;
		v_MovePink = 0.5f;
		v_MoveYellow = 0;
		v_MoveWhite = 0;
		v_MoveGrey = 0;
		v_MoveBlack = 0;

	}
	else if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 5)
	{
		v_ScaleBlue = 1.f;
		v_ScaleRed = 1.f;
		v_ScalePurple = 1.f;
		v_ScaleGreen = 1.f;
		v_ScalePink = 1.f;
		v_ScaleYellow = 1.5f;
		v_ScaleWhite = 1.f;
		v_ScaleGrey = 1.f;
		v_ScaleBlack = 1.f;

		v_MoveBlue = 0;
		v_MoveRed = 0;
		v_MovePurple = 0;
		v_MoveGreen = 0;
		v_MovePink = 0;
		v_MoveYellow = 0.5f;
		v_MoveWhite = 0;
		v_MoveGrey = 0;
		v_MoveBlack = 0;

	}
	else if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 6)
	{
		v_ScaleBlue = 1.f;
		v_ScaleRed = 1.f;
		v_ScalePurple = 1.f;
		v_ScaleGreen = 1.f;
		v_ScalePink = 1.f;
		v_ScaleYellow = 1.f;
		v_ScaleWhite = 1.5f;
		v_ScaleGrey = 1.f;
		v_ScaleBlack = 1.f;
	
		v_MoveBlue = 0;
		v_MoveRed = 0;
		v_MovePurple = 0;
		v_MoveGreen = 0;
		v_MovePink = 0;
		v_MoveYellow = 0;
		v_MoveWhite = 0.5f;
		v_MoveGrey = 0;
		v_MoveBlack = 0;
	
	}
	else if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 7)
	{
		v_ScaleBlue = 1.f;
		v_ScaleRed = 1.f;
		v_ScalePurple = 1.f;
		v_ScaleGreen = 1.f;
		v_ScalePink = 1.f;
		v_ScaleYellow = 1.f;
		v_ScaleWhite = 1.f;
		v_ScaleGrey = 1.5f;
		v_ScaleBlack = 1.f;
	
		v_MoveBlue = 0;
		v_MoveRed = 0;
		v_MovePurple = 0;
		v_MoveGreen = 0;
		v_MovePink = 0;
		v_MoveYellow = 0;
		v_MoveWhite = 0;
		v_MoveGrey = 0.5f;
		v_MoveBlack = 0;

	}
	else if (v_ColourList.f_GetCurColour()->f_GetColourNum() == 8)
	{
		v_ScaleBlue = 1.f;
		v_ScaleRed = 1.f;
		v_ScalePurple = 1.f;
		v_ScaleGreen = 1.f;
		v_ScalePink = 1.f;
		v_ScaleYellow = 1.f;
		v_ScaleWhite = 1.f;
		v_ScaleGrey = 1.f;
		v_ScaleBlack = 1.5f;
	
		v_MoveBlue = 0;
		v_MoveRed = 0;
		v_MovePurple = 0;
		v_MoveGreen = 0;
		v_MovePink = 0;
		v_MoveYellow = 0;
		v_MoveWhite = 0;
		v_MoveGrey = 0;
		v_MoveBlack = 0.5f;

	}
}


void c_Garage::f_RenderPreviews()
{
	modelStack.PushMatrix();
	modelStack.Translate(-3, 4, 0);
	modelStack.Scale(v_ScalePreview1, v_ScalePreview1, v_ScalePreview1);
	RenderMesh(meshList[CAR1PREVIEW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 4, 0);
	modelStack.Scale(v_ScalePreview2, v_ScalePreview2, v_ScalePreview2);
	RenderMesh(meshList[CAR2PREVIEW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3, 4, 0);
	modelStack.Scale(v_ScalePreview3, v_ScalePreview3, v_ScalePreview3);
	RenderMesh(meshList[CAR3PREVIEW], false);
	modelStack.PopMatrix();
}

void c_Garage::f_UpdateCurCar()
{
	if (v_CarList.f_GetCurCar()->f_GetCarNum() == 0)
	{
		v_ScalePreview1 = 1.5f;
		v_ScalePreview2 = 1.f;
		v_ScalePreview3 = 1.f;
	}
	else if (v_CarList.f_GetCurCar()->f_GetCarNum() == 1)
	{
		v_ScalePreview1 = 1.f;
		v_ScalePreview2 = 1.5f;
		v_ScalePreview3 = 1.f;
	}
	else if (v_CarList.f_GetCurCar()->f_GetCarNum() == 2)
	{
		v_ScalePreview1 = 1.f;
		v_ScalePreview2 = 1.f;
		v_ScalePreview3 = 1.5f;
	}
}

void c_Garage::f_RenderFinal()
{
	modelStack.PushMatrix();
	modelStack.Rotate(45, 1, 0, 0);
	modelStack.Rotate(v_RotateCar, 0, 1, 0);
	switch (v_CarList.f_GetCurCar()->f_GetCarNum())
	{
	case 0:
		switch (v_ColourList.f_GetCurColour()->f_GetColourNum())
		{
		case 0:
			v_TGAName = "Image//testColourBlue.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		case 1:
			v_TGAName = "Image//testColourRed.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		case 2:
			v_TGAName = "Image//testColourPurple.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		case 3:
			v_TGAName = "Image//testColourGreen.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		case 4:
			v_TGAName = "Image//testColourPink.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		case 5:
			v_TGAName = "Image//testColourYellow.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		case 6:
			v_TGAName = "Image//testColourWhite.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		case 7:
			v_TGAName = "Image//testColourGrey.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		case 8:
			v_TGAName = "Image//testColourBlack.tga";
			meshList[car1]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car1], true);
			break;
		default:
			RenderMesh(meshList[car1], true);
			break;
		}
		break;
	case 1:
		switch (v_ColourList.f_GetCurColour()->f_GetColourNum())
		{
		case 0:
			v_TGAName = "Image//testColourWhite.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		case 1:
			v_TGAName = "Image//testColourRed.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		case 2:
			v_TGAName = "Image//testColourPurple.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		case 3:
			v_TGAName = "Image//testColourGreen.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		case 4:
			v_TGAName = "Image//testColourPink.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		case 5:
			v_TGAName = "Image//testColourYellow.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		case 6:
			v_TGAName = "Image//testColourWhite.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		case 7:
			v_TGAName = "Image//testColourGrey.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		case 8:
			v_TGAName = "Image//testColourBlack.tga";
			meshList[car2]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car2], true);
			break;
		default:
			RenderMesh(meshList[car2], true);
			break;
		}
		break;
	case 2:
		switch (v_ColourList.f_GetCurColour()->f_GetColourNum())
		{
		case 0:
			v_TGAName = "Image//testColourBlue.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		case 1:
			v_TGAName = "Image//testColourRed.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		case 2:
			v_TGAName = "Image//testColourPurple.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		case 3:
			v_TGAName = "Image//testColourGreen.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		case 4:
			v_TGAName = "Image//testColourPink.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		case 5:
			v_TGAName = "Image//testColourYellow.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		case 6:
			v_TGAName = "Image//testColourWhite.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		case 7:
			v_TGAName = "Image//testColourGrey.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		case 8:
			v_TGAName = "Image//testColourBlack.tga";
			meshList[car3]->textureID = LoadTGA(v_TGAName);
			RenderMesh(meshList[car3], true);
			break;
		default:
			RenderMesh(meshList[car3], true);
			break;
		}
		break;
	default:
		break;
	}
	modelStack.PopMatrix();
}

void c_Garage::Exit()
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


