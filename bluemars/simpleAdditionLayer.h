#ifndef INCLUDE_simpleAdditionLayer
#define INCLUDE_simpleAdditionLayer

#include <string>
#include <iostream>
#include "windowsSpecific.h"
#include "baselayer.h"
#include "chunkdata.h"

class BlueMars_EXPORT simpleAdditionLayer : public BaseLayer
{
public:
	simpleAdditionLayer();
	std::shared_ptr<ChunkData> preGenerateChunk(int chunk_x, int chunk_y);
  
};

#endif
