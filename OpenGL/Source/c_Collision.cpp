#include "c_Collision.h" 

#include <fstream>
#include "Mtx44.h"


c_Collision::c_Collision()
{
	highestX = highestY = highestZ = 0;
	lowestX = lowestY = lowestZ = INFINITY;
	pos.Set(0, 0, 0);
	dimensions = Vector3(0, 0, 0);
	localX.Set(1, 0, 0);
	localY.Set(0, 1, 0);
	localZ.Set(0, 0, 1);
}


c_Collision::~c_Collision()
{
}


void c_Collision::setHighLow(const char *file_path)
{
	if (file_path == "quad")
	{
		dimensions.Set(1, 1, 0);
		type = file_path;
	}
	else
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
					highestZ = vertex.z;
				else if (vertex.z < lowestZ)
					lowestZ = vertex.z;

			}
		}

		type = "OBJ";
		dimensions.Set((highestX - lowestX) / 2, (highestY - lowestY) / 2, (highestZ - lowestZ) / 2);
	}

}

void c_Collision::calcNewAxis(float rotateAmt, float xAxis, float yAxis, float zAxis)
{
	float angle = Math::DegreeToRadian(-rotateAmt);

	if (xAxis == 1)
	{
		//Rotate About x Axis
		Mtx44 matrix(
			1, 0, 0, 0,
			0, cos(angle), -sin(angle), 0,
			0, sin(angle), cos(angle), 0,
			0, 0, 0, 1);



		localY = matrix * localY;
		localZ = matrix * localZ;
	}
	else if (yAxis == 1)
	{
		//Rotate About Y axis
		Mtx44 matrix(
			cos(angle), 0, sin(angle), 0,
			0, 1, 0, 0,
			-sin(angle), 0, cos(angle), 0,
			0, 0, 0, 1);

		localX = matrix * localX;//To Get local X Axis after rotation
		localZ = matrix * localZ;//To Get local Z Axis after rotation
	}
	else if(zAxis == 1)
	{
		//Rotate About Z Axis
		Mtx44 matrix(
			cos(angle), -sin(angle), 0, 0,
			sin(angle), cos(angle), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		localX = matrix * localX;
		localY = matrix * localY;
	}
}
void c_Collision::calcNewDimensions(float xScale, float yScale, float zScale)
{
	this->dimensions.Set(dimensions.x*xScale, dimensions.y*yScale, dimensions.z*zScale);
}
void c_Collision::defaultData()
{
	if (type == "quad")
		dimensions.Set(0.5, 0.5, 0);
	else
		dimensions.Set((highestX - lowestX) / 2, (highestY - lowestY) / 2, (highestZ - lowestZ) / 2);
	pos = (0.f, 0.f, 0.f);
	localX.Set(1, 0, 0);
	localY.Set(0, 1, 0);
	localZ.Set(0, 0, 1);
}

Vector3 c_Collision::getXAxis()
{
	return localX;
}
Vector3 c_Collision::getYAxis()
{
	return localY;
}
Vector3 c_Collision::getZAxis()
{
	return localZ;
}
Vector3 c_Collision::getDimensions()
{
	return dimensions;
}

void c_Collision::setPos(Vector3 pos)
{
	this->pos = pos;
}
bool c_Collision::getSeparatingPlane(const Vector3& RPos, const Vector3& Plane, c_Collision* other)
{
	Vector3 otherX = other->getXAxis();
	Vector3 otherY = other->getYAxis();
	Vector3 otherZ = other->getZAxis();
	Vector3 otherDimension = other->getDimensions();

	return (fabs(RPos.Dot(Plane)) >
		(fabs((localX*dimensions.x).Dot(Plane)) +
			fabs((localY*dimensions.y).Dot(Plane)) +
			fabs((localZ*dimensions.z).Dot(Plane)) +
			fabs((otherX*otherDimension.x).Dot(Plane)) +
			fabs((otherY*otherDimension.y).Dot(Plane)) +
			fabs((otherZ*otherDimension.z).Dot(Plane))));
}
bool c_Collision::OBB(c_Collision* other)
{
	static Vector3 RPos;
	RPos = other->pos - pos;

	Vector3 otherX = other->getXAxis();
	Vector3 otherY = other->getYAxis();
	Vector3 otherZ = other->getZAxis();

	return !(getSeparatingPlane(RPos, localX, other) ||
		getSeparatingPlane(RPos, localY, other) ||
		getSeparatingPlane(RPos, localZ, other) ||
		getSeparatingPlane(RPos, otherX, other) ||
		getSeparatingPlane(RPos, otherY, other) ||
		getSeparatingPlane(RPos, otherZ, other) ||
		getSeparatingPlane(RPos, localX.Cross(otherX), other) ||
		getSeparatingPlane(RPos, localX.Cross(otherY), other) ||
		getSeparatingPlane(RPos, localX.Cross(otherZ), other) ||
		getSeparatingPlane(RPos, localY.Cross(otherX), other) ||
		getSeparatingPlane(RPos, localY.Cross(otherY), other) ||
		getSeparatingPlane(RPos, localY.Cross(otherZ), other) ||
		getSeparatingPlane(RPos, localZ.Cross(otherX), other) ||
		getSeparatingPlane(RPos, localZ.Cross(otherY), other) ||
		getSeparatingPlane(RPos, localZ.Cross(otherZ), other));
}
bool c_Collision::checkSurroundingOBJ(c_Collision* other)
{
	return (other->pos.x >= pos.x - 3 * dimensions.x && other->pos.x <= pos.x + 3 * dimensions.x
		&& other->pos.y >= pos.y - 3 * dimensions.y && other->pos.y <= pos.y + 3 * dimensions.y
		&& other->pos.z >= pos.z - 3 * dimensions.z && other->pos.z <= pos.z + 3 * dimensions.z);
}
