#ifndef INCLUDE_BASELAYER
#define INCLUDE_BASELAYER

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include "chunkdata.h"

long mod(long a, long b); // Modulo helper method, ToDo: move this inside a helper file!

class BaseLayer
{
 public:
  BaseLayer();
  float get(long x, long y);
  virtual std::shared_ptr<ChunkData> preGenerateChunk(int chunk_x, int chunk_y) = 0;
  void listchunks();
  unsigned int nbChunks();
  protected:
  unsigned short chunkSize; // See contructor for default value
  std::vector< std::shared_ptr<BaseLayer> > dependentLayers;
  std::map< std::pair<int, int>, std::shared_ptr<ChunkData> > chunk_map ;
};

#endif
