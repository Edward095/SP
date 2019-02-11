#include "c_MainMenu.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"


c_MainMenu::c_MainMenu()
{
}


c_MainMenu::~c_MainMenu()
{
}

void c_MainMenu::Init()
{
	APosX = 0;
	APosY = 0;
	APosZ = 0;

	meshList[NEWGAME] = MeshBuilder::GenerateQuad("NewGame", RED, 5);
	meshList[NEWGAME]->textureID = LoadTGA("Image//NewGame.tga");
}

void c_MainMenu::Update(double dt)
{
	if (Application::IsKeyPressed(VK_UP))
	{

	}
}

void c_MainMenu::Render()
{
	modelStack.PushMatrix();
	render.RenderMesh(meshList[NEWGAME], false);
	modelStack.PopMatrix();

}
