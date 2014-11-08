#ifndef INCLUDE_elevation
#define INCLUDE_elevation

#include <string>
#include <iostream>
#include "baselayer.h"
#include "chunkdata.h"

class elevation: public BaseLayer
{
public:
	elevation();
	std::shared_ptr<ChunkData> preGenerateChunk(int chunk_x, int chunk_y);

};

#endif
