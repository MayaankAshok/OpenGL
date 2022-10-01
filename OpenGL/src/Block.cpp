#include "Block.h"

std::vector<MatProp> BlockType::blockProp{
	{{{7,0},{7,0},{7,0},{7,0},{7,0},{7,0}},true},//AIR
	{{{8,0},{8,0},{8,0},{8,0},{8,0},{8,0}},true},//WATER
	{{{1,0},{1,0},{1,0},{1,0},{0,0},{2,0}},false},//GRASS
	{{{2,0},{2,0},{2,0},{2,0},{2,0},{2,0}},false},//DIRT
	{{{3,0},{3,0},{3,0},{3,0},{3,0},{3,0}},false},//STONE
};


std::vector<Vertex> Block::vertices = {
	//position				// texcoord
Vertex{-1.0f, -1.0f,  1.0f,		0.0f, 0.0f	},//0 
Vertex{ 1.0f, -1.0f,  1.0f,		1.0f, 0.0f	},//1,
Vertex{ 1.0f,  1.0f,  1.0f,		1.0f, 1.0f	},//2,
Vertex{-1.0f, -1.0f,  1.0f,		0.0f, 0.0f	},//0 
Vertex{ 1.0f,  1.0f,  1.0f,		1.0f, 1.0f	},//2,
Vertex{-1.0f,  1.0f,  1.0f,		0.0f, 1.0f	},//3,

};

Block::Block(float x, float y, float z, BlockType::Types blockMaterial) {
	position = { x,y,z };
	blockMat = blockMaterial;
	isTransparent = BlockType::blockProp[blockMaterial].isTransparent;

}
Block::Block() {
	position = { 0,0,0 };
	blockMat = BlockType::AIR;
	isTransparent = true;
}
void Block::setMaterial(BlockType::Types matType) {
	blockMat = matType;
	isTransparent = BlockType::blockProp[matType].isTransparent;

}