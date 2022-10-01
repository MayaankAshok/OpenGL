#pragma once
#include <cmath>

struct Vec3i
{
	int x, y, z;
};

struct Vec3
{
	float x, y, z;
	operator Vec3i() {
		return { (int)x, (int)y, (int)z };
	};
	Vec3 operator + (Vec3 const& obj) {
		return { x + obj.x, y + obj.y, z + obj.z };
	}

};

struct Vec2i
{
	int x, y;

};

struct Vec2
{
	float x, y;

};

struct Vec4
{
	float x, y, z, w;
	Vec4()
		: x(0), y(0), z(0), w(0)
	{	}
	Vec4(Vec3 vec3, float num)
		: x(vec3.x), y(vec3.y), z(vec3.z), w(num)
	{	}
	/*Vec4(Vec3 vec3, int num)
		: x(vec3.x), y(vec3.y), z(vec3.z), w(num)
	{	}*/
};

struct Vertex
{
	Vec3 position;
	Vec2 TexCoords;

};

struct VertexInstanced
{
	Vec3 translation;
	Vec2 rotation;
	Vec2 materialCoords;

};


