#include "flat.h"
#include <iostream>

void Flat::preGenerateChunk(unsigned int x, unsigned int y)
{
  ChunkData chunk;
  chunk.chunk_x = x;
  chunk.chunk_y = y;
  for(unsigned int i_x = 0; i_x<x+chunkSize; ++i_x )
    {
      for(unsigned int i_y = 0; i_y<y+chunkSize; ++i_y )
	{
	  chunk.data[std::make_pair(i_x, i_y)] = (float)i_x+i_y;
	}
    }
  this->data.insert({std::make_pair(x,y), std::make_shared<ChunkData>(chunk)});
  

}

Flat::Flat()
{
  
}

