#include "baselayer.h"

BaseLayer::BaseLayer()
{
	chunkSize = 1024; // Default value for chunkSize
}

float BaseLayer::get(long x, long y)
{
	unsigned int chunk_x = x / chunkSize;
	unsigned int chunk_y = y / chunkSize;
	float value;
	std::pair<unsigned int, unsigned int> chunk_coordinates = std::make_pair(chunk_x, chunk_y);
	unsigned long in_chunk_coordinates = (x % chunkSize) * chunkSize + y % chunkSize;
	if (chunk_map.find(chunk_coordinates) != chunk_map.end()) // If chunk already exists
	{
		value = chunk_map[chunk_coordinates]->data[in_chunk_coordinates]; // Get value
	}
	else
	{
	  std::shared_ptr<ChunkData> chunk_ptr = preGenerateChunk(chunk_x, chunk_y);
	  auto chunk_coord = std::make_pair(chunk_x, chunk_y);
	  this->chunk_map.insert({chunk_coord, chunk_ptr});
	  value = chunk_map[chunk_coordinates]->data[in_chunk_coordinates];
	}
	return value;
}

void BaseLayer::listchunks()
{
	std::cout << "Currently loaded chunks:" << std::endl;
	for(auto it=chunk_map.begin(); it!=chunk_map.end(); ++it)
	{
		std::pair<unsigned int, unsigned int> coordinates = it->first;
		std::cout << " - " << coordinates.first << ";" << coordinates.second << std::endl;
	}
}

