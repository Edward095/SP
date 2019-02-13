#include "c_Collision.h"
#include <fstream>


c_Collision::c_Collision()
{
	highestX = highestY = highestZ = 0;
	lowestX = lowestY = lowestZ = INFINITY;
	pos.Set(0, 0, 0);
	max.Set(0, 0, 0);
	min.Set(0, 0, 0);
}


c_Collision::~c_Collision()
{
}


void c_Collision::setHighLow(const char *file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
	}

	std::vector<unsigned> vertexIndices, uvIndices, normalIndices;
	std::vector<Position> temp_vertices;
	std::vector<TexCoord> temp_uvs;
	std::vector<Vector3> temp_normals;

	while (!fileStream.eof())
	{
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0)
		{
			Position vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);

			//Getting highest and lowest of each XYZ
			if (vertex.x > highestX)
				highestX = vertex.x;
			else if (vertex.x < lowestX)
				lowestX = vertex.x;
			if (vertex.y > highestY)
				highestY = vertex.y;
			else if (vertex.y < lowestY)
				lowestY = vertex.y;
			if (vertex.z > highestZ)
				highestX = vertex.z;
			else if (vertex.z < lowestZ)
				lowestZ = vertex.z;

		}
	}


	Position highest,lowest;
	highest.Set(highestX + pos.x, highestY + pos.y, highestZ + pos.z);
	lowest.Set(lowestX + pos.x, lowestY + pos.y, lowestX + pos.z);
	BB.push_back(highest);
	BB.push_back(lowest);
}

bool c_Collision::AABB(c_Collision other)
{
	//0 = fTopRight; 1 = fTopLeft; 2 = fBottomRight; 3 = fBottomLeft
	std::vector<Position> BB2 = other.getBoxes();

	if (BB[0].x >= BB2[1].x && BB[1].x <= BB2[0].x &&
		BB[0].y >= BB2[1].y && BB[1].y <= BB2[0].y &&
		BB[0].z >= BB2[1].z && BB[1].z <= BB2[0].z)
		return true;
	
	return false;
}

std::vector<Position> c_Collision::getBoxes()
{
	return BB;
}
void c_Collision::updateHighLow()
{
	while(!BB.empty())
	{
		BB.pop_back();
	}
	Position highest, lowest;
	highest.Set(highestX + pos.x, highestY + pos.y, highestZ + pos.z);
	lowest.Set(lowestX + pos.x, lowestY + pos.y, lowestX + pos.z);
	BB.push_back(highest);
	BB.push_back(lowest);
}