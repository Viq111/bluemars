#ifndef INCLUDE_elevation
#define INCLUDE_elevation

#include <string>
#include <iostream>
#include <future>
#include <thread>
#include "baselayer.h"
#include "chunkdata.h"
#define NB_THREADS 16

class elevation: public BaseLayer
{
public:
	elevation();
	std::shared_ptr<ChunkData> preGenerateChunk(int chunk_x, int chunk_y);
	std::vector<float> generateData(long seed, long x_offset, long y_offset);
	std::vector<float> generatePartData(long seed, long x_min, long x_max, int size);

};

#endif
