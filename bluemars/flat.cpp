#include "flat.h"

void Flat::preGenerateChunk(unsigned int chunk_x, unsigned int chunk_y)
{
	ChunkData* chunk = new ChunkData();
	chunk->chunk_x = chunk_x;
	chunk->chunk_y = chunk_y;
	for (unsigned int i_x = 0; i_x < chunkSize; ++i_x)
	{
		for (unsigned int i_y = 0; i_y < chunkSize; ++i_y)
		{
			chunk->data[std::make_pair(i_x, i_y)] = (float) i_x+i_y;
		}
	}
	this->chunk_map.insert({ std::make_pair(chunk_x, chunk_y), std::unique_ptr<ChunkData>(chunk) });
}



