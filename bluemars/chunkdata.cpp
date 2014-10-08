#include "chunkdata.h"

ChunkData::ChunkData(int chunk_size)
{
	data.resize(chunk_size*chunk_size);
}