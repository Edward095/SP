#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"
/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float length);
	static Mesh* GenerateRect(const std::string &meshName, Color color, float length);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlice, float radius);
	static Mesh* GenerateSemiCircle(const std::string &meshName, Color color, unsigned numSlice, float radius);
	static Mesh* GenerateQuarter(const std::string &meshName, Color color, unsigned numSlice, float radius);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateHemiSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateQuarterSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateCylinder(const std::string &meshName, Color color, unsigned numStack, float height, float radius);
	static Mesh* GenerateCone(const std::string &meshName, Color color, float height, float radius);
	static Mesh* GenerateLine(const std::string &meshName, Color color, float length);
	static Mesh* GenerateTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateHalfTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateQuarterTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateEars(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateNose(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height);
	static Mesh* GenerateTaperedCylinder(const std::string &meshName, Color color, unsigned numStack, float height, float radius);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
};

#endif