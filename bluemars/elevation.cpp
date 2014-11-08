#include "elevation.h"
#include "perlin.h"

std::shared_ptr<ChunkData> elevation::preGenerateChunk(int chunk_x, int chunk_y)
{
	ChunkData* chunk = new ChunkData(chunkSize);
	chunk->chunk_x = chunk_x;
	chunk->chunk_y = chunk_y;
	long x_offset = chunk_x*chunkSize;
	long y_offset = chunk_y*chunkSize;
	long seed = (long) this ;
	Noise noiseInstance = Noise(seed);
	for (int i_x = 0; i_x < chunkSize; ++i_x)
	{
		for (int i_y = 0; i_y < chunkSize; ++i_y)
		{
			long in_chunk_coordinates = i_x * chunkSize + i_y;
			chunk->data[in_chunk_coordinates] =
				(float) noiseInstance.outputValue(i_x+x_offset,i_y+y_offset);
		}
	}
	return std::shared_ptr<ChunkData>(chunk);
}

elevation::elevation(){}
