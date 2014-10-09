#ifndef INCLUDE_BASELAYER
#define INCLUDE_BASELAYER

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include "chunkdata.h"

class BaseLayer
{
 public:
  BaseLayer();
  float get(long x, long y);
  virtual std::shared_ptr<ChunkData> preGenerateChunk(unsigned int chunk_x, unsigned int chunk_y) = 0;
  void listchunks();
  protected:
  unsigned short chunkSize; // See contructor for default value
  std::vector< std::shared_ptr<BaseLayer> > dependentLayers;
  std::map< std::pair<unsigned int, unsigned int>, std::shared_ptr<ChunkData> > chunk_map ;
};

#endif
