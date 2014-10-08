#include "baselayer.h"

float BaseLayer::get(long x, long y)
{
  unsigned int chunk_x = x/1024;
  unsigned int chunk_y = y/1024;
  float value;
  std::pair<unsigned int, unsigned int> pair = std::make_pair(chunk_x, chunk_y);
  if (chunk_map.find(pair) != chunk_map.end())
    {
      value = chunk_map[pair]->data[std::make_pair(x%1024,y%1024)];
    }
  else
    {
      preGenerateChunk(chunk_x, chunk_y);
      value = this->chunk_map[pair]->data[std::make_pair(x%1024,y%1024)];
    }
  return value;
}

void BaseLayer::listchunks()
{
  for(auto it=chunk_map.begin(); it!=chunk_map.end(); ++it)
    {
      std::pair<unsigned int, unsigned int> pair = it->first;
      std::cout << " - ";
      std::cout << "Chunk X : " << pair.first << "\t";
      std::cout << "Chunk Y : " << pair.second << "\t" << std::endl;
    }
}

BaseLayer::BaseLayer()
{
}  
