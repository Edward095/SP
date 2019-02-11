#ifndef C_MAINMENU_H
#define C_MAINMENU_H

#include "Variables.h"
#include "c_Render.h"

class c_MainMenu
{
public:
	c_MainMenu();
	~c_MainMenu();
	void Init();
	void Update(double dt);
	void Render();

private:
	Mesh* meshList[NUM_GEOMETRY];
	MS modelStack, viewStack, projectionStack;

	float APosX, APosY, APosZ;
	c_Render render;
};

#endif