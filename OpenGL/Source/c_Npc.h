#ifndef C_NPC_H
#define C_NPC_H

#include "Scene.h"
#include "Variables.h"
#include "c_Render.h"

class c_Npc 
{
public:
	c_Npc();
	~c_Npc();
	void Init();
	void Update(double dt);
	void Render();

private:
	Mesh * meshList[NUM_GEOMETRY];
	MS modelStack, viewStack, projectionStack;

	
	c_Render render;
};



#endif

