#include "baselayer.h"

BaseLayer::BaseLayer()
{
}

float BaseLayer::get(long x, long y)
{
	unsigned int chunk_x = x / chunkSize;
	unsigned int chunk_y = y / chunkSize;
	float value;
	std::pair<unsigned int, unsigned int> chunk_coordinates = std::make_pair(chunk_x, chunk_y);
	if (chunk_map.find(chunk_coordinates) != chunk_map.end()) // If chunk already exists
	{
		value = chunk_map[chunk_coordinates]->data[std::make_pair(x % chunkSize, y % chunkSize)]; // Get value
	}
	else
	{
		preGenerateChunk(chunk_x, chunk_y); // Else generate it and save it in chunk_map
		// WARNING: Save is done in preGenerateChunk, is this normal ?
		value = chunk_map[chunk_coordinates]->data[std::make_pair(x % chunkSize, y % chunkSize)];
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


