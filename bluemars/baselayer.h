#ifndef INCLUDE_BASELAYER
#define INCLUDE_BASELAYER

#include <memory>
#include <vector>
#include "chunkdata.h"

class BaseLayer
{
 public:
  BaseLayer();
  float get(long x, long y);
  virtual void preGenerateChunk(unsigned int x, unsigned int y) = 0;
  void listchunks();
 protected:
  unsigned short chunkSize = 1024;
  std::vector< std::shared_ptr<BaseLayer> > dependentLayers;
  std::map< std::pair<unsigned int, unsigned int>, std::shared_ptr<ChunkData> > chunk_map ;
};

#endif
