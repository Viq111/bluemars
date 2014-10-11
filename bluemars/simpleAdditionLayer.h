#ifndef INCLUDE_simpleAdditionLayer
#define INCLUDE_simpleAdditionLayer

#include <string>
#include <iostream>
#include "baselayer.h"
#include "chunkdata.h"

class simpleAdditionLayer: public BaseLayer
{
public:
	simpleAdditionLayer();
	std::shared_ptr<ChunkData> preGenerateChunk(unsigned int, unsigned int);
  
};

#endif
