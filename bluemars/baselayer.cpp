#include "baselayer.h"
#include <iostream>
#include "chunkdata.h"

float BaseLayer::get(long x, long y)
{
  unsigned int chunk_x = x/1024;
  unsigned int chunk_y = y/1024;
  float value;
  std::pair<unsigned int, unsigned int> pair = std::make_pair(chunk_x, chunk_y);
  if (data.find(pair) != data.end())
    {
      value = data[pair]->data[std::make_pair(x%1024,y%1024)];
    }
  else
    {
      preGenerateChunk(chunk_x, chunk_y);
      std::cout << pair.first << "\t" << pair.second << std::endl;
      std::cout << std::make_pair(x%1024,y%1024).first << "\t" << std::make_pair(x%1024,y%1024).second << std::endl;
      //      std::cout << data[pair]->data[std::make_pair(x%1024,y%1024)] << std::endl;
      std::shared_ptr<ChunkData> chunk = data[pair];
      //      std::cout << chunk->chunk_x << std::endl;
    }
  return value;
}

BaseLayer::BaseLayer()
{
}  
