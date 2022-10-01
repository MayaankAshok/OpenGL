#pragma once
#include <vector>
#include "Structures.h"

struct MatProp {
	std::vector<Vec2> texAtlID;
	bool isTransparent;
};

struct BlockType {
	enum Types {
		AIR,
		WATER,
		GRASS,
		DIRT,
		STONE,
	};

	static std::vector<MatProp> blockProp;
	MatProp getMatProp(Types blockType) {
		return  blockProp[blockType];
	}
};


class Block
{
public:
	static std::vector<Vertex> vertices;
	//static std::vector<int> indices;
	Vec3 position;
	BlockType::Types blockMat;
	bool isTransparent;

	


	Block(float x, float y, float z, BlockType::Types blockMaterial);

	Block();

	void setMaterial(BlockType::Types matType);

};



