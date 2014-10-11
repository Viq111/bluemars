#include "simpleAdditionLayer.h"

std::shared_ptr<ChunkData> simpleAdditionLayer::preGenerateChunk(int chunk_x, int chunk_y)
{
	ChunkData* chunk = new ChunkData(chunkSize);
	chunk->chunk_x = chunk_x;
	chunk->chunk_y = chunk_y;
	for (int i_x = 0; i_x < chunkSize; ++i_x)
	{
		for (int i_y = 0; i_y < chunkSize; ++i_y)
		{
			long in_chunk_coordinates = i_x * chunkSize + i_y;
			chunk->data[in_chunk_coordinates] = (float) i_x+i_y;
		}
	}
	return std::shared_ptr<ChunkData>(chunk);
}

simpleAdditionLayer::simpleAdditionLayer(){}

