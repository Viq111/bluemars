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
      value = this->data[pair]->data[std::make_pair(x%1024,y%1024)];
    }
  return value;
}

BaseLayer::BaseLayer()
{
}  
