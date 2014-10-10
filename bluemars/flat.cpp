#include "flat.h"

std::shared_ptr<ChunkData> Flat::preGenerateChunk(unsigned int chunk_x, unsigned int chunk_y)
{
	ChunkData* chunk = new ChunkData(chunkSize);
	chunk->chunk_x = chunk_x;
	chunk->chunk_y = chunk_y;
	for (unsigned int i_x = 0; i_x < chunkSize; ++i_x)
	{
		for (unsigned int i_y = 0; i_y < chunkSize; ++i_y)
		{
			unsigned long in_chunk_coordinates = i_x * chunkSize + i_y;
			chunk->data[in_chunk_coordinates] = (float) i_x+i_y;
		}
	}
	return std::shared_ptr<ChunkData>(chunk);
}

Flat::Flat(){}

