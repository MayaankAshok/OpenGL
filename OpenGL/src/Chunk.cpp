#include "Chunk.h"
#include <iostream>
#include <algorithm>

#define print(x) std::cout<<x<<std::endl

std::array<Vec2, 6> Chunk::faceRotations{
	0,0, 1,0, 2,0, 3,0, 0,1, 0,3
};

Chunk::Chunk(Vec2 position,VertexBuffer* _vb,VertexBufferLayout* _layout) {
	chunkPosition = position;
	vb = _vb;
	layout = _layout;
	//vb->CreateData(&Block::vertices[0].position.x, 6 * sizeof(Vertex), GL_STATIC_DRAW);
	//Create Height Data
	{
		double frequency = .8;
		int octaves = 8;
		int seed = 111;
		int height = 16;
		int width = 16;

		double fx = width / frequency;
		double fy = height / frequency;

		int ih = 0, kh = 0;
		for (auto& HeightRow : HeightData) {
			kh = 0;
			for (auto& HeightVal : HeightRow) {
				HeightVal = (1 + glm::perlin(glm::vec2((chunkPosition.x + ih / (16.0f)) * frequency, (chunkPosition.y + kh / (16.0f)) * frequency))) / 2;
				kh += 1;
			}
			ih += 1;
		}
		float val = glm::perlin(glm::vec2(1 / 16, 3 / 16));
	}

	// Populate Chunk
	{
		BlockList.resize(ChunkXSize * ChunkYSize * ChunkZSize);
		int i = 0, j = 0, k = 0, n = 0;
		for (auto& BlockLayer : Mesh) {
			j = 0;
			for (auto& BlockRow : BlockLayer) {
				k = 0;
				for (auto& block : BlockRow) {
					float heightLevel = floor(16 * HeightData[i][k]);
					if (j == heightLevel) {
						BlockList[n] = { (float)i,(float)j, (float)k,BlockType::GRASS };
						block = &BlockList[n];
						n += 1;
						//print(i  << " " << j << " " << k << " " << block.position.x << " " << block.position.y << " " << block.position.z);

						//print(i <<" "<< j<<" "<<k);

					}
					else if (j < heightLevel) {
						BlockList[n] = { (float)i,(float)j, (float)k,BlockType::DIRT };
						block = &BlockList[n];
						n += 1;
						//print(i  << " " << j << " " << k << " " << block.position.x << " " << block.position.y << " " << block.position.z);

						//print(i <<" "<< j<<" "<<k);

					}
					else {
						block = nullptr;
					}
					k += 1;
				}
				j += 1;
			}
			i += 1;
		}
		BlockList.resize(n);
	}

	//updateIVB();

	//layout.Push<float>(3, false);
	//layout.Push<float>(2, false);
	//layout.Push<float>(3, true);
	//layout.Push<float>(2, true);
	//layout.Push<float>(2, true);

	vaBlock.AddBuffer(*vb, ivbBlock, *layout);

}

Chunk::Chunk() {

}

void Chunk::GenNoiseData() {
	double frequency = .8;
	int octaves = 8;
	int seed = 1;
	int height = 16;
	int width = 16;

	double fx = width / frequency;
	double fy = height / frequency;

	int ih = 0, kh = 0;
	for (auto& HeightRow : HeightData) {
		kh = 0;
		for (auto& HeightVal : HeightRow) {
			HeightVal = (1 + glm::perlin(glm::vec3((chunkPosition.x + ih / (16.0f)) * frequency, (chunkPosition.y + kh / (16.0f)) * frequency, seed))) / 2;
			kh += 1;
		}
		ih += 1;
	}
	float val = glm::perlin(glm::vec2(1 / 16, 3 / 16));
}

bool doCompare(const Block* block1, const Block* block2,Vec2 chunkPosition, Vec3 playerPosition)
{
	float dist1 = pow(playerPosition.x - (chunkPosition.x * 16 + block1->position.x),2) +
		pow(playerPosition.y - block1->position.y,2) +
		pow(playerPosition.z - (chunkPosition.y * 16 + block1->position.z),2);

	float dist2 = pow(playerPosition.x - (chunkPosition.x * 16 + block2->position.x),2) +
		pow(playerPosition.y - block2->position.y,2) +
		pow(playerPosition.z - (chunkPosition.y * 16 + block2->position.z),2);

	return dist1 > dist2;

}

void Chunk::_Create(Vec2 position,VertexBuffer* _vb, VertexBufferLayout* _layout) {
	vb = _vb;
	layout = _layout;
	chunkPosition = position;

	//Create Height Data
	GenNoiseData();

	// Populate Chunk
	{
		BlockList.resize(ChunkXSize * ChunkYSize * ChunkZSize);
		int i = 0, j = 0, k = 0, n = 0;
		for (auto& BlockLayer : Mesh) {
			j = 0;
			for (auto& BlockRow : BlockLayer) {
				k = 0;
				for (auto& block : BlockRow) {
					float heightLevel = floor(16 * HeightData[i][k]);
					if (j == heightLevel) {
						BlockList[n] = { (float)i,(float)j, (float)k,BlockType::GRASS };
						block = &BlockList[n];
						n += 1;


					}
					else if (j < heightLevel && j>= heightLevel-3) {
						BlockList[n] = { (float)i,(float)j, (float)k,BlockType::DIRT };
						block = &BlockList[n];
						n += 1;


					}
					else if (j < heightLevel - 3) {
						BlockList[n] = { (float)i,(float)j, (float)k,BlockType::STONE };
						block = &BlockList[n];
						n += 1;


					}
					else if (j < WaterLevel) {
						BlockList[n] = { (float)i,(float)j, (float)k,BlockType::WATER };
						block = &BlockList[n];
						ChunkWaterSorted.push_back(&BlockList[n]);
						n += 1;


					}
					else {
						block = nullptr;
					}
					k += 1;
				}
				j += 1;
			}
			i += 1;
		}
		BlockList.resize(n);
	}

	//updateIVB();

	//layout.Push<float>(3, false);
	//layout.Push<float>(2, false);
	//layout.Push<float>(3, true);
	//layout.Push<float>(2, true);
	//layout.Push<float>(2, true);

	//vaBlock.AddBuffer(*vb, ivbBlock, *layout);


}


void Chunk::_Finalize() {
	updateIVB();
	vaBlock.AddBuffer(*vb, ivbBlock, *layout);
	vaWater .AddBuffer(*vb, ivbWater, *layout);

}
void Chunk::_Update(Vec3 _playerPosition) {
	std::sort(ChunkWaterSorted.begin(), ChunkWaterSorted.end(), [this,_playerPosition](Block* block1, Block* block2) {return doCompare(block1, block2, chunkPosition, _playerPosition); });
	updateIVB();

}

std::vector<VertexInstanced> Chunk::getBlockData() {
	int n = 0;
	Vec3 chunkUpdatedPosition = { 16 * chunkPosition.x,0,16 * chunkPosition.y };
	std::vector<VertexInstanced> data(6 * 16 * 16 * 16);
	for (auto BlockLayer : Mesh) {
		for (auto BlockRow : BlockLayer) {
			for (auto block : BlockRow) {
				if (block != nullptr) {
					if (block->isTransparent == false) {
						auto faceVisiblity = shouldDisplayBlock(block);
						for (int i = 0; i < 6; i++) {
							if (faceVisiblity[i] == true) {
								data[n] = { block->position + chunkUpdatedPosition,faceRotations[i],BlockType::blockProp[block->blockMat].texAtlID[i] };
								n += 1;
							}

						}
					}
				}
			}
		}
	}
	data.resize(n);
	return data;
}

std::vector<VertexInstanced> Chunk::getWaterData() {
	int n = 0;
	Vec3 chunkUpdatedPosition = { 16 * chunkPosition.x,0,16 * chunkPosition.y };
	std::vector<VertexInstanced> data(6 * 16 * 16 * 16);
	for (auto BlockLayer : Mesh) {
		for (auto BlockRow : BlockLayer) {
			for (auto block : BlockRow) {
				if (block != nullptr) {
					if (block->blockMat == BlockType::WATER) {
						auto faceVisiblity = shouldDisplayWater(block);
						for (int i = 0; i < 6; i++) {
							if (faceVisiblity[i] == true) {
								data[n] = { block->position + chunkUpdatedPosition,faceRotations[i],BlockType::blockProp[block->blockMat].texAtlID[i] };
								n += 1;
							}

						}
					}
				}
			}
		}
	}
	data.resize(n);
	if (data.size() == 0) { isWaterPresent = false; }
	else { 
		isWaterPresent = true; }
	//print(n);
	return data;
}

std::vector<VertexInstanced> Chunk::getWaterData2() {
	int n = 0;
	Vec3 chunkUpdatedPosition = { 16 * chunkPosition.x,0,16 * chunkPosition.y };
	std::vector<VertexInstanced> data(6 * 16 * 16 * 16);
	for (auto block : ChunkWaterSorted) {
		assert(block != nullptr);
		if (block->blockMat == BlockType::WATER) {
			auto faceVisiblity = shouldDisplayWater(block);
			for (int i = 0; i < 6; i++) {
				if (faceVisiblity[i] == true) {
					data[n] = { block->position + chunkUpdatedPosition,faceRotations[i],BlockType::blockProp[block->blockMat].texAtlID[i] };
					n += 1;
				}

			}
		}

	}
	data.resize(n);
	if (data.size() == 0) { isWaterPresent = false; }
	else {
		isWaterPresent = true;
	}
	//print(n);
	return data;
}
void Chunk::updateIVB() {
	auto blockData = getBlockData();
	ivbBlock.CreateData(&blockData[0].translation.x, blockData.size() * sizeof(VertexInstanced), blockData.size(), GL_STATIC_DRAW);
	auto waterData = getWaterData2();
	if (isWaterPresent) ivbWater.CreateData(&waterData[0].translation.x, waterData.size() * sizeof(VertexInstanced), waterData.size(), GL_STATIC_DRAW);

}

std::array<bool, 6> Chunk::shouldDisplayBlock(Block* block) {
	Vec3 position1 = block->position;
	Vec3i position = (Vec3i)position1;
	std::array<bool, 6>result{ false,false, false, false, false, false };


	// face z+
	if (position.z == ChunkZSize - 1) {
		if (chunkZp == nullptr) {
			result[0] = true;
		}
		else {
			if (chunkZp->Mesh[position.x][position.y][0] == nullptr) {
				result[0] = true;
			}
			else if (chunkZp->Mesh[position.x][position.y][0]->isTransparent) {
				result[0] = true;
			}
		}
	}
	else if (Mesh[position.x][position.y][position.z + 1] == nullptr) {
		result[0] = true;
	}
	else if (Mesh[position.x][position.y][position.z + 1]->isTransparent) {
		result[0] = true;
	}

	// face x+
	if (position.x == ChunkXSize - 1) {
		if (chunkXp == nullptr) {
			result[1] = true;
		}
		else {
			if (chunkXp->Mesh[0][position.y][position.z] == nullptr) {
				result[1] = true;
			}
			else if (chunkXp->Mesh[0][position.y][position.z]->isTransparent) {
				result[1] = true;
			}
		}
	}
	else if (Mesh[position.x + 1][position.y][position.z] == nullptr) {
		result[1] = true;
	}
	else if (Mesh[position.x + 1][position.y][position.z]->isTransparent) {
		result[1] = true;
	}

	// face z-
	if (position.z == 0) {
		if (chunkZn == nullptr) {
			result[2] = true;
		}
		else {
			if (chunkZn->Mesh[position.x][position.y][ChunkZSize - 1] == nullptr) {
				result[2] = true;
			}
			else if (chunkZn->Mesh[position.x][position.y][ChunkZSize - 1]->isTransparent) {
				result[2] = true;
			}
		}
	}
	else if (Mesh[position.x][position.y][position.z - 1] == nullptr) {
		result[2] = true;
	}
	else if (Mesh[position.x][position.y][position.z - 1]->isTransparent) {
		result[2] = true;
	}

	// face x-
	if (position.x == 0) {
		if (chunkXn == nullptr) {
			result[3] = true;
		}
		else {
			if (chunkXn->Mesh[ChunkXSize - 1][position.y][position.z] == nullptr) {
				result[3] = true;
			}
			else if (chunkXn->Mesh[ChunkXSize - 1][position.y][position.z]->isTransparent) {
				result[3] = true;
			}
		}
	}
	else if (Mesh[position.x - 1][position.y][position.z] == nullptr) {
		result[3] = true;
	}
	else if (Mesh[position.x - 1][position.y][position.z]->isTransparent) {
		result[3] = true;
	}

	// face y+
	if (position.y == ChunkYSize - 1) {
		result[4] = true;
	}
	else if (Mesh[position.x][position.y + 1][position.z] == nullptr) {
		result[4] = true;
	}
	else if (Mesh[position.x][position.y + 1][position.z]->isTransparent) {
		result[4] = true;
	}

	// face y-
	if (position.y == 0) {
		result[5] = true;
	}
	else if (Mesh[position.x][position.y - 1][position.z] == nullptr) {
		result[5] = true;
	}
	else if (Mesh[position.x][position.y - 1][position.z]->isTransparent) {
		result[5] = true;
	}

	return result;
}

std::array<bool, 6> Chunk::shouldDisplayWater(Block* block) {
	Vec3 position1 = block->position;
	Vec3i position = (Vec3i)position1;
	std::array<bool, 6>result{ false,false, false, false, false, false };


	// face z+
	if (position.z == ChunkZSize - 1) {
		if (chunkZp == nullptr) {
			result[0] = true;
		}
		else {
			if (chunkZp->Mesh[position.x][position.y][0] == nullptr) {
				result[0] = true;
			}
			//else if (chunkZp->Mesh[position.x][position.y][0]->blockMat != BlockType::WATER) {
			//	result[0] = true;
			//}
		}
	}
	else if (Mesh[position.x][position.y][position.z + 1] == nullptr) {
		result[0] = true;
	}
	//else if (Mesh[position.x][position.y][position.z + 1]->blockMat != BlockType::WATER) {
	//	result[0] = true;
	//}

	// face x+
	if (position.x == ChunkXSize - 1) {
		if (chunkXp == nullptr) {
			result[1] = true;
		}
		else {
			if (chunkXp->Mesh[0][position.y][position.z] == nullptr) {
				result[1] = true;
			}
			//else if (chunkXp->Mesh[0][position.y][position.z]->blockMat != BlockType::WATER) {
			//	result[1] = true;
			//}
		}
	}
	else if (Mesh[position.x + 1][position.y][position.z] == nullptr) {
		result[1] = true;
	}
	//else if (Mesh[position.x + 1][position.y][position.z]->blockMat != BlockType::WATER) {
	//	result[1] = true;
	//}

	// face z-
	if (position.z == 0) {
		if (chunkZn == nullptr) {
			result[2] = true;
		}
		else {
			if (chunkZn->Mesh[position.x][position.y][ChunkZSize - 1] == nullptr) {
				result[2] = true;
			}
			//else if (chunkZn->Mesh[position.x][position.y][ChunkZSize - 1]->blockMat != BlockType::WATER) {
			//	result[2] = true;
			//}
		}
	}
	else if (Mesh[position.x][position.y][position.z - 1] == nullptr) {
		result[2] = true;
	}
	//else if (Mesh[position.x][position.y][position.z - 1]->blockMat != BlockType::WATER) {
	//	result[2] = true;
	//}

	// face x-
	if (position.x == 0) {
		if (chunkXn == nullptr) {
			result[3] = true;
		}
		else {
			if (chunkXn->Mesh[ChunkXSize - 1][position.y][position.z] == nullptr) {
				result[3] = true;
			}
			//else if (chunkXn->Mesh[ChunkXSize - 1][position.y][position.z]->blockMat != BlockType::WATER) {
			//	result[3] = true;
			//}
		}
	}
	else if (Mesh[position.x - 1][position.y][position.z] == nullptr) {
		result[3] = true;
	}
	//else if (Mesh[position.x - 1][position.y][position.z]->blockMat != BlockType::WATER) {
	//	result[3] = true;
	//}

	// face y+
	if (position.y == ChunkYSize - 1) {
		result[4] = true;
	}
	else if (Mesh[position.x][position.y + 1][position.z] == nullptr) {
		result[4] = true;
	}
	//else if (Mesh[position.x][position.y + 1][position.z]->blockMat != BlockType::WATER) {
	//	result[4] = true;
	//}

	// face y-
	if (position.y == 0) {
		result[5] = true;
	}
	else if (Mesh[position.x][position.y - 1][position.z] == nullptr) {
		result[5] = true;
	}
	//else if (Mesh[position.x][position.y - 1][position.z]->blockMat != BlockType::WATER) {
	//	result[5] = true;
	//}

	return result;
}

InstancedVB Chunk::getIVB() {
	auto blockData = getBlockData();
	return  InstancedVB(&blockData[0].translation.x, blockData.size() * sizeof(VertexInstanced), blockData.size(), GL_STATIC_DRAW);

}

bool Chunk::operator()(const Block* block1, const Block* block2)
{
	float dist1 = abs(playerPosition.x - (chunkPosition.x * 16 + block1->position.x)) +
		abs(playerPosition.y - block1->position.y) +
		abs(playerPosition.z - (chunkPosition.y * 16 + block1->position.z));

	float dist2 = abs(playerPosition.x - (chunkPosition.x * 16 + block2->position.x)) +
		abs(playerPosition.y - block2->position.y) +
		abs(playerPosition.z - (chunkPosition.y * 16 + block2->position.z));

	return dist1 > dist2;

}


void const Chunk::RenderChunk(Renderer& renderer, Shader& shader, glm::vec3 playerPos) {
	_Update({ playerPos.x ,playerPos.y,playerPos.z});
	glEnable(GL_DEPTH_TEST);
	renderer.DrawInstanced(vaBlock, shader, 6, ivbBlock.m_InstanceCount);
	//glDisable(GL_DEPTH_TEST);
	renderer.DrawInstanced(vaWater, shader, 6, ivbWater.m_InstanceCount);
	//renderer.DrawInstancedWireFrame(vaBlock, shader, 6, ivbBlock.m_InstanceCount);
	//renderer.DrawInstancedWireFrame(vaWater, shader, 6, ivbWater.m_InstanceCount);
}


ChunkCluster::ChunkCluster() {
	vb.CreateData(&Block::vertices[0].position.x, 6 * sizeof(Vertex), GL_STATIC_DRAW);
	layout.Push<float>(3, false);
	layout.Push<float>(2, false);
	layout.Push<float>(3, true);
	layout.Push<float>(2, true);
	layout.Push<float>(2, true);



	ChunkList.resize(ChunksX * ChunksZ);
	//chunk._Create({ 0,0 });
	float i = 0, k = 0, n = 0;

	for (auto& chunkRow : ChunkMesh) {
		k = 0;
		for (auto& chunk : chunkRow) {
			ChunkList[n]._Create({ i - floor(ChunksX / 2.0f),k - floor(ChunksZ / 2.0f) }, &vb,&layout);
			ChunkMesh[i][k] = &ChunkList[n];
			k += 1;
			n += 1;
			//prXint(i<<" "<<k);
		}
		i += 1;
	}

	i = 0, k = 0, n = 0;
	for (auto& chunkRow : ChunkMesh) {
		k = 0;
		for (auto& chunk : chunkRow) {
			updateNeighbourChunks(chunk, i, k);
			chunk->_Finalize();
			k += 1;
			n += 1;
			//prXint(i<<" "<<k);
		}
		i += 1;
	}

	
}

void ChunkCluster::updateNeighbourChunks(Chunk* chunk, int i, int k) {
	if (i == 0) {
		chunk->chunkXn = nullptr;
	}
	else {
		chunk->chunkXn = ChunkMesh[i - 1][k];
	}

	if (i == ChunksX - 1) {
		chunk->chunkXp = nullptr;
	}
	else {
		chunk->chunkXp = ChunkMesh[i + 1][k];
	}

	if (k == 0) {
		chunk->chunkZn = nullptr;
	}
	else {
		chunk->chunkZn = ChunkMesh[i][k - 1];
	}

	if (k == ChunksZ - 1) {
		chunk->chunkZp = nullptr;
	}
	else {
		chunk->chunkZp = ChunkMesh[i][k + 1];
	}
}

void ChunkCluster::RenderChunks(Renderer& renderer, Shader& shader,glm::vec3 playerPos) {
	//chunk.RenderChunk(renderer,shader);
	for (auto& chunk : ChunkList) {
		chunk.RenderChunk(renderer, shader,playerPos);
	}

}