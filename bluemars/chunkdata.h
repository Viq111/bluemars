#ifndef INCLUDE_CHUNKDATA
#define INCLUDE_CHUNKDATA

#include <vector>

struct ChunkData
{
	ChunkData(int chunk_size);
	int chunk_x;
	int chunk_y;
	std::vector<float> data;
};

#endif
