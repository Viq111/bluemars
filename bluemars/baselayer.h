#ifndef INCLUDE_BASELAYER
#define INCLUDE_BASELAYER

#include <vector>
#include "chunkdata.h"

class BaseLayer
{
 public:
  void get(long x, long y);
 private:
  void preGenerateChunk(unsigned int x, unsigned int y);
  std::vector <ChunkData> data ;
};

#endif
