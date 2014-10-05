#ifndef INCLUDE_CHUNKDATA
#define INCLUDE_CHUNKDATA

#include <map>

struct ChunkData {
  int chunk_x;
  int chunk_y;
  std::map < std::pair<unsigned int, unsigned int>, float> data;
};

#endif
