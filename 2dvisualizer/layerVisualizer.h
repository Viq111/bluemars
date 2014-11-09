#ifndef INCLUDE_LAYERVISUALIZER
#define INCLUDE_LAYERVISUALIZER

#include <memory>
#include "chunkdata.h"

std::vector<unsigned char> chunk2TGA(std::shared_ptr<ChunkData> chunk, short width, short height); // Convert a chunk to a TGA Image
void writeTGA2File(std::string filename, std::vector<unsigned char> tga_data); // For debug purpose, write a TGA to a file

#endif