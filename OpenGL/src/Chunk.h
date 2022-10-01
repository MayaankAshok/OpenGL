#pragma once
#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/noise.hpp"

#include "Structures.h"
#include "Block.h"

#include "srcGL/VertexBuffer.h"
#include "srcGL/VertexArray.h"
#include "srcGL/VertexBufferLayout.h"
#include "srcGL/InstancedVB.h"
#include "srcGL/Renderer.h"
#include "srcGL/Shader.h"

class Chunk {
public:


	const static int ChunkXSize = 16, ChunkYSize = 16, ChunkZSize = 16;
	const static int WaterLevel = 8;
	static std::array<Vec2, 6> faceRotations;

	VertexBuffer* vb;
	InstancedVB ivbBlock;
	InstancedVB ivbWater;

	VertexArray vaBlock;
	VertexArray vaWater;

	VertexBufferLayout* layout;

	Vec2 chunkPosition;
	Vec3 playerPosition;
	std::vector<Block> BlockList; // array of all blocks in chunk
	std::vector<Block*> ChunkWaterSorted;
	std::array<std::array<std::array<Block*, ChunkZSize>, ChunkYSize>, ChunkXSize> Mesh; // 3d array containing pointers to blocks in BlockList , nullptr = Air block 
	std::array<std::array<float, ChunkYSize>, ChunkXSize> HeightData;// y values for each block , from perlin data

	//pointers to neighbouring chunks (Xp = X+ = bordering chunk in X+ direction,	Zn = Z- = bordering chunk in Z- direction ) 
	Chunk* chunkXp;
	Chunk* chunkZp;
	Chunk* chunkXn;
	Chunk* chunkZn;

	bool isWaterPresent = true;

	Chunk(Vec2 position, VertexBuffer* _vb, VertexBufferLayout* _layout);
	Chunk();

	void GenNoiseData();
	
	void _Create(Vec2 position, VertexBuffer* _vb, VertexBufferLayout* _layout);
	void _Finalize();
	void _Update(Vec3 playerPosition);
	
	bool operator () (const Block* i1, const Block* i2);
	void const RenderChunk(Renderer& renderer, Shader& shader, glm::vec3 playerPos);
	bool sortBlocks(Block* block1, Block* block2);
	std::vector<VertexInstanced> getWaterData2();
	void updateIVB();
	InstancedVB getIVB();
	
	std::vector<VertexInstanced> getBlockData();
	std::vector<VertexInstanced> getWaterData();
	std::array<bool, 6> shouldDisplayBlock(Block* block);
	std::array<bool, 6> shouldDisplayWater(Block* block);
		
	void const RenderChunk(Renderer& renderer, Shader& shader);

};

class ChunkCluster {
public:
	VertexBuffer vb;
	VertexBufferLayout layout;
	
	const static int ChunksX = 1, ChunksZ = 1;
	
	std::vector<Chunk> ChunkList;
	std::array<std::array<Chunk*, ChunksZ>, ChunksX> ChunkMesh;

	ChunkCluster();


	void updateNeighbourChunks(Chunk* chunk, int i, int k);

	void RenderChunks(Renderer& renderer, Shader& shader,glm::vec3 playerPos);
};


//VertexBuffer Chunk::vb{ &Block::vertices[0].position.x, 6 * sizeof(Vertex), GL_STATIC_DRAW };

