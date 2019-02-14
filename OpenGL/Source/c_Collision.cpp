#include "c_Collision.h" 

#include <fstream>
#include "Mtx44.h"


c_Collision::c_Collision()
{
	highestX = highestY = highestZ = 0;
	lowestX = lowestY = lowestZ = INFINITY;
	pos.Set(0, 0, 0);
	max.Set(0, 0, 0);
	min.Set(0, 0, 0);
	dimensions.Set(0, 0, 0);
	localX.Set(1, 0, 0);
	localY.Set(0, 1, 0);
	localZ.Set(0, 0, 1);
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
				highestZ = vertex.z;
			else if (vertex.z < lowestZ)
				lowestZ = vertex.z;

		}
	}


	calcData();

}

bool c_Collision::AABB(c_Collision other)
{
	//0 = highest; 1 = lowest
	std::vector<Vector3> BB2 = other.getBoxes();

	if (BB[0].x >= BB2[1].x && BB[1].x <= BB2[0].x &&
		BB[0].y >= BB2[1].y && BB[1].y <= BB2[0].y &&
		BB[0].z >= BB2[1].z && BB[1].z <= BB2[0].z)
		return true;
	
	return false;
}
void c_Collision::calcNewAxis(float rotateAmt,Vector3 Axis)
{
	float angle = Math::DegreeToRadian(rotateAmt);

	if (Axis.x == 1)
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
	else if (Axis.y == 1)
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
	else if(Axis.z == 1)
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

std::vector<Vector3> c_Collision::getBoxes()
{
	return BB;
}
std::vector<Vector3> c_Collision::getCornervec()
{
	return cornerVec;
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
void c_Collision::updateHighLow()
{
	while(!BB.empty())
		BB.pop_back();
	while (!cornerVec.empty())
		cornerVec.pop_back();

	calcData();
}
void c_Collision::calcData()
{
	Vector3 highest, lowest;
	highest.Set(highestX + pos.x, highestY + pos.y, highestZ + pos.z);
	lowest.Set(lowestX + pos.x, lowestY + pos.y, lowestX + pos.z);
	BB.push_back(highest);
	BB.push_back(lowest);

	frontTopLeft.Set(lowestX + pos.x, highestY + pos.y, highestZ + pos.z);
	frontTopRight.Set(highestX + pos.x, highestY + pos.y, highestZ + pos.z);
	frontBottomLeft.Set(lowestX + pos.x, lowestY + pos.y, highestZ + pos.z);
	frontBottomRight.Set(highestX + pos.x, lowestY + pos.y, highestZ + pos.z);

	backTopLeft.Set(lowestX + pos.x, highestY + pos.y, lowestZ + pos.z);
	backTopRight.Set(highestX + pos.x, highestY + pos.y, lowestZ + pos.z);
	backBottomLeft.Set(lowestX + pos.x, lowestY + pos.y, lowestZ + pos.z);
	backBottomRight.Set(highestX + pos.x, lowestY + pos.y, lowestZ + pos.z);

	cornerVec.push_back(frontTopLeft);
	cornerVec.push_back(frontTopRight);
	cornerVec.push_back(frontBottomLeft);
	cornerVec.push_back(frontBottomRight);
	cornerVec.push_back(backTopLeft);
	cornerVec.push_back(backTopRight);
	cornerVec.push_back(backBottomLeft);
	cornerVec.push_back(backBottomRight);

	//x = HalfWidth; y = HalfHeight; z = HalfDepth
	dimensions.Set((highestX - lowestX) / 2, (highestY - lowestY) / 2, (highestZ - lowestZ) / 2);
}

bool c_Collision::getSeparatingPlane(const Vector3& RPos, const Vector3& Plane, c_Collision& other)
{
	Vector3 otherX = other.getXAxis();
	Vector3 otherY = other.getYAxis();
	Vector3 otherZ = other.getZAxis();
	Vector3 otherDimension = other.getDimensions();

	return (fabs(RPos.Dot(Plane)) >
		(fabs((localX*dimensions.x).Dot(Plane)) +
			fabs((localY*dimensions.y).Dot(Plane)) +
			fabs((localZ*dimensions.z).Dot(Plane)) +
			fabs((otherX*otherDimension.x).Dot(Plane)) +
			fabs((otherY*otherDimension.y).Dot(Plane)) +
			fabs((otherZ*otherDimension.z).Dot(Plane))));
}
bool c_Collision::OBB(c_Collision& other)
{
	static Vector3 RPos;
	//RPos.Set(other.pos.x - pos.x, other.pos.y - pos.y, other.pos.z - pos.z);
	RPos = other.pos - pos;

	Vector3 otherX = other.getXAxis();
	Vector3 otherY = other.getYAxis();
	Vector3 otherZ = other.getZAxis();

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
void c_Collision::setPos(Vector3 pos)
{
	this->pos = pos;
}