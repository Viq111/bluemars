#ifndef INCLUDE_ELEVATION_LAYER
#define INCLUDE_ELEVATION_LAYER

#include "baselayer.h"
#include "chunkdata.h"
#include "perlin.h"

class ElevationLayer: public BaseLayer
{
public:
	ElevationLayer(long seed);
	std::shared_ptr<ChunkData> preGenerateChunk(int chunk_x, int chunk_y);
protected:
	long seed;
	Noise noiseInstance;
};

#endif
