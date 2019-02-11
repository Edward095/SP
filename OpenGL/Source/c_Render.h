#ifndef C_RENDER_H
#define C_RENDER_H

#include "Variables.h"


class c_Render
{
public:
	c_Render();
	~c_Render();
	void RenderMesh(Mesh *mesh, bool enableLight);

private:
	 unsigned m_vertexArrayID;
	 Mesh* meshList[NUM_GEOMETRY];

	 unsigned m_programID;
	 unsigned m_parameters[U_TOTAL];
	 FirstPersonCamera camera;

	 MS modelStack, viewStack, projectionStack;

	 Light light[3];
};

#endif