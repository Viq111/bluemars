#ifndef INCLUDE_FLAT
#define INCLUDE_FLAT

#include <string>
#include <iostream>
#include "baselayer.h"
#include "chunkdata.h"

class Flat: public BaseLayer
{
public:
	Flat();
	std::shared_ptr<ChunkData> preGenerateChunk(unsigned int, unsigned int);
  
};

#endif
