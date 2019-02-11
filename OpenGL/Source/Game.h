#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Variables.h"
#include "c_MainMenu.h"

class Game : public Scene
{
public:
	Game();
	~Game();
	void Init();
	void Update(double dt);
	void Render();
	void Exit();
private:
	 unsigned m_vertexArrayID;
	 Mesh* meshList[NUM_GEOMETRY];

	 unsigned m_programID;
	 unsigned m_parameters[U_TOTAL];
	 FirstPersonCamera camera;

	 MS modelStack, viewStack, projectionStack;

	 Light light[3];
	 c_Render render;
	 c_MainMenu MMScene;
};
#endif
