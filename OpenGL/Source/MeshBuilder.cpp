#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "Vertex.h"
#include "MyMath.h"
#include <math.h>

#include <Windows.h>
#include <time.h>

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	// An array of 3 vectors which represents 3 vertices
	Vertex v;
	v.normal.Set(0, 0, 1);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	
	for (unsigned j = 0; j < numRow; ++j)
	{
		for (unsigned i = 0; i < numCol; ++i)
		{
			float u1 = i * width;
			float v1 = 1.f - height - j * height;

			v.pos.Set(-0.5f, -0.5f, 0); v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, -0.5f, 0); v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f , 0.5f, 0); v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, 0.5f, 0); v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}
	
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = 6;
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	// An array of 3 vectors which represents 3 vertices
	Vertex v;
	v.normal.Set(0, 0, 1);
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-0.5f * length, -0.5f * length, 0); v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,0);v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,0.5f * length,0);v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length,0.5f * length,0);v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}
Mesh* MeshBuilder::GenerateRect(const std::string &meshName, Color color, float length)
{
	// An array of 3 vectors which represents 3 vertices
	Vertex v;
	v.normal.Set(0, 0, 1);
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-0.5f * length, -0.1667f * length, 0); v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.1667f * length, 0); v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0); v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0); v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	// An array of 3 vectors which represents 3 vertices
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;

	//Front Face
	v.normal.Set(0, 0, 1);
	v.pos.Set(-0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);

	//Right Face
	v.normal.Set(1, 0, 0);
	v.pos.Set(0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);

	//Top Face
	v.normal.Set(0, 1, 0);
	v.pos.Set(0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);


	//Left Face
	v.normal.Set(-1, 0, 0);
	v.pos.Set(-0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);

	//Bottom Face
	v.normal.Set(0, -1, 0);
	v.pos.Set(-0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f);
	vertex_buffer_data.push_back(v);

	//Back Face
	v.normal.Set(0, 0, -1);
	v.pos.Set(-0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, -0.5f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, -0.5f);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	for (int i = 0; i < vertex_buffer_data.size(); i++)
	{
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = 36;
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

/******************************************************************************/
/*!
 \brief
 Generate the vertices of a circle; Use random color for each vertex
 Then generate the VBO/IBO and store them in Mesh object
 
 \param meshName - name of mesh
 \param numSlice - number of triangle used to draw
 \param radius - radius of the circle
 
 \return Pointer to mesh storing VBO/IBO of cube
 */
/******************************************************************************/
Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, Color color, unsigned numSlice, float radius)
{
	Vertex v;
	v.normal.Set(0, 1, 0);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerSlice = 360.f / numSlice;
	for(unsigned slice = 0; slice < numSlice; ++slice) {

		float theta = slice * degreePerSlice;
/*v0*/	v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

/*v1*/	v.pos.Set(0, 0, 0);
		v.color = color;
		vertex_buffer_data.push_back(v);

		float theta2 = (slice + 1) * degreePerSlice;
/*v2*/	v.pos.Set(radius * cos(Math::DegreeToRadian(theta2)), 0, radius * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice; ++slice) {
		index_buffer_data.push_back(slice * 3 + 0);
		index_buffer_data.push_back(slice * 3 + 1);
		index_buffer_data.push_back(slice * 3 + 2);
	}
	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateSemiCircle(const std::string &meshName, Color color, unsigned numSlice, float radius)
{
	Vertex v;
	v.normal.Set(0, 1, 0);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerSlice = 180.f / numSlice;
	for (unsigned slice = 0; slice < numSlice; ++slice) {

		float theta = slice * degreePerSlice;
		/*v0*/	v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		/*v1*/	v.pos.Set(0, 0, 0);
		v.color = color;
		vertex_buffer_data.push_back(v);

		float theta2 = (slice + 1) * degreePerSlice;
		/*v2*/	v.pos.Set(radius * cos(Math::DegreeToRadian(theta2)), 0, radius * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < numSlice; ++slice) {
		index_buffer_data.push_back(slice * 3 + 0);
		index_buffer_data.push_back(slice * 3 + 1);
		index_buffer_data.push_back(slice * 3 + 2);
	}
	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateQuarter(const std::string &meshName, Color color, unsigned numSlice, float radius)
{
	Vertex v;
	v.normal.Set(0, 1, 0);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerSlice = 90.f / numSlice;
	for (unsigned slice = 0; slice < numSlice; ++slice) {

		float theta = slice * degreePerSlice;
		/*v0*/	v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		/*v1*/	v.pos.Set(0, 0, 0);
		v.color = color;
		vertex_buffer_data.push_back(v);

		float theta2 = (slice + 1) * degreePerSlice;
		/*v2*/	v.pos.Set(radius * cos(Math::DegreeToRadian(theta2)), 0, radius * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < numSlice; ++slice) {
		index_buffer_data.push_back(slice * 3 + 0);
		index_buffer_data.push_back(slice * 3 + 1);
		index_buffer_data.push_back(slice * 3 + 2);
	}
	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}

/******************************************************************************/
/*!
 \brief
 Generate the vertices of a Ring; Use random color for each vertex
 Then generate the VBO/IBO and store them in Mesh object
 
 \param meshName - name of mesh
 \param numSlice - number of triangle used to draw
 \param outerR - outer radius of the circle
 \param innerR - inner radius of the circle
 
 \return Pointer to mesh storing VBO/IBO of cube
 */
/******************************************************************************/
Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR, float innerR)
{
	Vertex v;
	v.normal.Set(0, 1, 0);
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerSlice = 360.f / numSlice;
	for(unsigned slice = 0; slice < numSlice; ++slice) {

		float theta = slice * degreePerSlice;
/*v0*/	v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

/*v1*/	v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		vertex_buffer_data.push_back(v);

		float theta2 = (slice + 1) * degreePerSlice;
/*v2*/	v.pos.Set(outerR * cos(Math::DegreeToRadian(theta2)), 0, outerR * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		vertex_buffer_data.push_back(v);

/*v3*/	v.pos.Set(innerR * cos(Math::DegreeToRadian(theta2)), 0, innerR * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice; ++slice) {
		index_buffer_data.push_back(slice * 4 + 0);
		index_buffer_data.push_back(slice * 4 + 1);
		index_buffer_data.push_back(slice * 4 + 2);
		
		index_buffer_data.push_back(slice * 4 + 3);
		index_buffer_data.push_back(slice * 4 + 2);
		index_buffer_data.push_back(slice * 4 + 1);
	}
	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}

/******************************************************************************/
/*!
 \brief
 Generate the vertices of a Sphere or Hemisphere; Use random color for each vertex
 Then generate the VBO/IBO and store them in Mesh object
 
 \param meshName - name of mesh
 \param numSlice - number of triangle used to draw
 \param numStack - number of layers used to draw
 \param radius -  radius of the circle
 
 \return Pointer to mesh storing VBO/IBO of cube
 */
/******************************************************************************/

float sphereX(float phi, float theta) {
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta) {
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta) {
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

/******************************************************************************/
Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	for(unsigned stack = 0; stack < numStack + 1; ++stack) {
		float phi = -90 + stack * degreePerStack;
		for(unsigned slice = 0; slice < numSlice + 1; ++slice) {
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for(unsigned stack = 0; stack < numStack; ++stack) {
		for(unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}
	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateHemiSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerStack = 90.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		float phi = -90 + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}

	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateQuarterSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerStack = 90.f / numStack;
	float degreePerSlice = 180.f / numSlice;
	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		float phi = -90 + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 2; ++slice) {
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}
	
	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, unsigned numStack, float height, float radius)
{
	int i = 0;
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float stackHeight = height / numStack;

	for (int stack = 0; stack < numStack; ++stack)
	{
		for (float theta = 0; theta <= 360; theta += 10)
		{
			v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2 + stack * stackHeight, radius*sin(Math::DegreeToRadian(theta)));
			v.normal.Set(cos(Math::DegreeToRadian(theta)), 0, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);	

			v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2 + (stack + 1)*stackHeight, radius*sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(i++);
			index_buffer_data.push_back(i++);
		}
	}
	for (float theta = 0; theta <= 360; theta += 10) //Top
	{
		v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), height / 2, radius*sin(Math::DegreeToRadian(theta)));
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height / 2, 0);
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(i++);
		index_buffer_data.push_back(i++);
	}
	for (float theta = 0; theta <= 360; theta += 10) //bottom
	{
		v.pos.Set(0, -height / 2, 0);
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2, radius*sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(i++);
		index_buffer_data.push_back(i++);
	}

	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, float height, float radius)
{
	int i = 0;
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	//Curved
	for (float theta = 0; theta <= 360; theta += 10)
	{
		v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2, radius*sin(Math::DegreeToRadian(theta)));
		v.normal.Set(cos(Math::DegreeToRadian(theta)), 0, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height / 2, 0);
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(i++);
		index_buffer_data.push_back(i++);
	}
	//Bottom Circle
	for (float theta = 0; theta <= 360; theta += 10)
	{
		v.pos.Set(0, -height / 2, 0);
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2, radius*sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(i++);
		index_buffer_data.push_back(i++);
	}

	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateLine(const std::string &meshName, Color color, float length)
{
	Vertex v;
	int i = 0;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(0, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(length, 0, 0);
	vertex_buffer_data.push_back(v);
	index_buffer_data.push_back(i++);
	index_buffer_data.push_back(i++);

	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 360.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float x1, z1;
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			z1 = outerR * cos(Math::DegreeToRadian(stack * degreePerStack));
			x1 = outerR * sin(Math::DegreeToRadian(stack * degreePerStack));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * cos(Math::DegreeToRadian(stack * degreePerStack));
			y2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * sin(Math::DegreeToRadian(stack * degreePerStack));
			z2 = innerR * sin(Math::DegreeToRadian(slice * degreePerSlice));

			v.pos.Set(x2, y2, z2);
			v.normal.Set(cos(Math::DegreeToRadian(slice * degreePerSlice)) * cos(Math::DegreeToRadian(stack * degreePerStack)),
				cos(Math::DegreeToRadian(slice * degreePerSlice)) * sin(Math::DegreeToRadian(stack * degreePerStack)),
				sin(Math::DegreeToRadian(slice * degreePerSlice)));
			v.color = color;

			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}
	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer); // bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}
Mesh* MeshBuilder::GenerateHalfTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack =180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float x1, z1;
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			z1 = outerR * cos(Math::DegreeToRadian(stack * degreePerStack));
			x1 = outerR * sin(Math::DegreeToRadian(stack * degreePerStack));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * cos(Math::DegreeToRadian(stack * degreePerStack));
			y2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * sin(Math::DegreeToRadian(stack * degreePerStack));
			z2 = innerR * sin(Math::DegreeToRadian(slice * degreePerSlice));

			v.pos.Set(x2, y2, z2);
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(slice * degreePerSlice)) * cos(Math::DegreeToRadian(stack * degreePerStack)),
				cos(Math::DegreeToRadian(slice * degreePerSlice)) * sin(Math::DegreeToRadian(stack * degreePerStack)),
				sin(Math::DegreeToRadian(slice * degreePerSlice)));

			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}
	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer); // bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}
Mesh* MeshBuilder::GenerateQuarterTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 90.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float x1, z1;
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			z1 = outerR * cos(Math::DegreeToRadian(stack * degreePerStack));
			x1 = outerR * sin(Math::DegreeToRadian(stack * degreePerStack));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * cos(Math::DegreeToRadian(stack * degreePerStack));
			y2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * sin(Math::DegreeToRadian(stack * degreePerStack));
			z2 = innerR * sin(Math::DegreeToRadian(slice * degreePerSlice));

			v.pos.Set(x2, y2, z2);
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(slice * degreePerSlice)) * cos(Math::DegreeToRadian(stack * degreePerStack)),
				cos(Math::DegreeToRadian(slice * degreePerSlice)) * sin(Math::DegreeToRadian(stack * degreePerStack)),
				sin(Math::DegreeToRadian(slice * degreePerSlice)));

			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}
	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer); // bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}
Mesh* MeshBuilder::GenerateEars(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR)
{
	/*int i = 0;
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float stackHeight = height / numStack;

	for (int stack = 0; stack < numStack; ++stack)
	{
		for (float theta = 0; theta <= 180; theta += 10)
		{
			v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2 + stack * stackHeight, radius  *sin(Math::DegreeToRadian(theta)));
			v.normal.Set(cos(Math::DegreeToRadian(theta)), 0, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);

			v.pos.Set(radius  *cos(Math::DegreeToRadian(theta)), -height / 2 + (stack + 1)*stackHeight, radius  *sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(i++);
			index_buffer_data.push_back(i++);
		}
	}*/
	//for (float theta = 0; theta <= 180; theta += 10) //Top
	//{
	//	v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), height / 2, radius*sin(Math::DegreeToRadian(theta)));
	//	v.normal.Set(0, 1, 0);
	//	vertex_buffer_data.push_back(v);

	//	v.pos.Set(0, height / 2, 0);
	//	vertex_buffer_data.push_back(v);

	//	index_buffer_data.push_back(i++);
	//	index_buffer_data.push_back(i++);
	//}
	//for (float theta = 0; theta <= 180; theta += 10) //bottom
	//{
	//	v.pos.Set(0, -height / 2, 0);
	//	v.normal.Set(0, -1, 0);
	//	vertex_buffer_data.push_back(v);

	//	v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2, radius*sin(Math::DegreeToRadian(theta)));
	//	vertex_buffer_data.push_back(v);

	//	index_buffer_data.push_back(i++);
	//	index_buffer_data.push_back(i++);
	//}

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 45.f / numStack;
	float degreePerSlice = 180.f / numSlice;
	float x1, z1;
	float x2, y2, z2;

	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			//z1 = outerR * cos(Math::DegreeToRadian(stack * degreePerStack));
			//x1 = outerR * sin(Math::DegreeToRadian(stack * degreePerStack));
			x2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * cos(Math::DegreeToRadian(stack * degreePerStack));
			y2 = (outerR + innerR * cos(Math::DegreeToRadian(slice * degreePerSlice))) * sin(Math::DegreeToRadian(stack * degreePerStack));
			z2 = innerR * sin(Math::DegreeToRadian(slice * degreePerSlice));

			v.pos.Set(x2, y2, z2);
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(slice * degreePerSlice)) * cos(Math::DegreeToRadian(stack * degreePerStack)),
				cos(Math::DegreeToRadian(slice * degreePerSlice)) * sin(Math::DegreeToRadian(stack * degreePerStack)),
				sin(Math::DegreeToRadian(slice * degreePerSlice)));

			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}
	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer); // bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}
Mesh* MeshBuilder::GenerateNose(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius, float height)
{
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float oneThird = (float)numStack / 3.f;
	float twoThird = oneThird * 2;
	float taperedRadius = radius / twoThird;
	float increment = radius / twoThird;
	bool temp = false;

	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		float phi = -90 + stack * degreePerStack;
		if (stack > twoThird)
			temp = true;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			float theta = slice * degreePerSlice;

				v.pos.Set(taperedRadius * sphereX(phi, theta), height * sphereY(phi, theta), taperedRadius * sphereZ(phi, theta));
				v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
				vertex_buffer_data.push_back(v);
		}
		if(temp)
			taperedRadius -= increment;//Tapered Up
		else
			taperedRadius += increment;//Tapereed Down
	}
	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}

	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}


Mesh* MeshBuilder::GenerateCandy(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	srand(time(NULL));

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float ranGreen = rand() % 192 + 182;
	float ranBlue = rand() % 203 + 193;
	Color white(1, 1, 1);
	Color pink(1.00000, 0.75294, 0.79608);

	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		float phi = -90 + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			if (rand() % 2)
				v.color = white;
			else
				v.color = pink;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}
	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateTaperedCylinder(const std::string &meshName, Color color, unsigned numStack, float height, float radius)
{
	int i = 0;
	Vertex v;
	v.color = color;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float stackHeight = height / numStack;
	bool overHalf = false;
	float taperedRadius = radius;

	for (int stack = 0; stack < numStack; ++stack)
	{
		if (stack > numStack / 2)
			overHalf = true;

		for (float theta = 0; theta <= 360; theta += 10)
		{
			v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2 + stack * stackHeight, radius*sin(Math::DegreeToRadian(theta)));
			v.normal.Set(cos(Math::DegreeToRadian(theta)), 0, sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);

			v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2 + (stack + 1)*stackHeight, radius*sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(i++);
			index_buffer_data.push_back(i++);

			if (overHalf)
			{
				taperedRadius += 0.005f;
				v.pos.Set(taperedRadius *cos(Math::DegreeToRadian(theta)), -height / 2 + stack * stackHeight, taperedRadius *sin(Math::DegreeToRadian(theta)));
				v.normal.Set(cos(Math::DegreeToRadian(theta)), 0, sin(Math::DegreeToRadian(theta)));
				vertex_buffer_data.push_back(v);

				v.pos.Set(taperedRadius *cos(Math::DegreeToRadian(theta)), -height / 2 + (stack + 1)*stackHeight, taperedRadius *sin(Math::DegreeToRadian(theta)));
				vertex_buffer_data.push_back(v);

				index_buffer_data.push_back(i++);
				index_buffer_data.push_back(i++);
			}
		}
	}
	for (float theta = 0; theta <= 360; theta += 10) //Top
	{
		v.pos.Set(taperedRadius*cos(Math::DegreeToRadian(theta)), height / 2, taperedRadius*sin(Math::DegreeToRadian(theta)));
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height / 2, 0);
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(i++);
		index_buffer_data.push_back(i++);
	}
	for (float theta = 0; theta <= 360; theta += 10) //bottom
	{
		v.pos.Set(0, -height / 2, 0);
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(radius*cos(Math::DegreeToRadian(theta)), -height / 2, radius*sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(i++);
		index_buffer_data.push_back(i++);
	}

	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}
Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path) 
{
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	//Create the mesh and call glBindBuffer, glBufferData
	Mesh *mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	//Use triangle list and remember to set index size

	return mesh;
}