#include "c_Npc.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"



c_Npc::c_Npc()
{


}

c_Npc::~c_Npc()
{

}

void c_Npc::Init()
{
	meshList[TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[TEXT]->textureID = LoadTGA("Image//calibri.tga");
}

void c_Npc::Update(double dt)
{

}

void c_Npc::Render()
{
	modelStack.PushMatrix();
	render.RenderTextOnScreen(meshList[TEXT], "Welcome to the racing game! ", Color(0, 0, 1), 2, 10, 10);
	modelStack.PopMatrix();
}