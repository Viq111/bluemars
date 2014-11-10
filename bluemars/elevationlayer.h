#ifndef INCLUDE_elevation
#define INCLUDE_elevation

#include <string>
#include <iostream>
#include "baselayer.h"
#include "chunkdata.h"

class ElevationLayer: public BaseLayer
{
public:
	ElevationLayer();
	std::shared_ptr<ChunkData> preGenerateChunk(int chunk_x, int chunk_y);

};

#endif
