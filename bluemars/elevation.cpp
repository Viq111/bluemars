#include "elevation.h"
#include "perlin.h"

std::shared_ptr<ChunkData> elevation::preGenerateChunk(int chunk_x, int chunk_y)
{
	ChunkData* chunk = new ChunkData(chunkSize);
	chunk->chunk_x = chunk_x;
	chunk->chunk_y = chunk_y;
	long x_offset = chunk_x*chunkSize;
	long y_offset = chunk_y*chunkSize;
	long seed = (long) this ;
	Noise noiseInstance = Noise(seed);
	long in_chunk_coordinates;
	// for (int i_x = 0; i_x < chunkSize; ++i_x)
	// {
	// 	for (int i_y = 0; i_y < chunkSize; ++i_y)
	// 	{
	// 		in_chunk_coordinates = i_x * chunkSize + i_y;
	// 		chunk->data[in_chunk_coordinates] =
	// 			(float) noiseInstance.outputValue(i_x+x_offset,i_y+y_offset);
	// 	}
	// }
	chunk->data = elevation::generateData(seed, x_offset, y_offset);
	return std::shared_ptr<ChunkData>(chunk);
}

elevation::elevation(){}

std::vector<float> elevation::generateData(long seed, long x_offset, long y_offset){
// 	int func() { return 1; }
// std::future<int> ret = std::async(&func);
// int i = ret.get();
	int size = chunkSize/NB_THREADS;
	std::vector<std::future<std::vector<float>>> values;
	long x_min;long x_max; long y_min; long y_max;
	std::cout << "x_offset: "<< x_offset<< " y_offset: "<< y_offset << std::endl;
	for(int i=0; i<NB_THREADS; i++){
		x_min = x_offset + i*size; // =x_offset when i=0
		x_max = x_min + size; //=x_offset+chunk_size when i=0
		std::cout << "\tx_min: "<< x_min<< " x_max: "<< x_max << std::endl;
		values.push_back(std::async(std::launch::async, &elevation::generatePartData, this, seed, x_min, x_max, y_offset, chunkSize));
	}
	std::vector<float> result;
	result.reserve(chunkSize*chunkSize);
	//for(auto i = values.size()-1; i < values.size(); i--){
	for(std::vector<int>::size_type i = 0; i != values.size(); i++){
		std::vector<float> pdata = values[i].get();
		result.insert(result.begin(),pdata.begin(),pdata.end());
	}
	return result;

}

std::vector<float> elevation::generatePartData(long seed, long x_min, long x_max,
																							 long y_offset, int size){
	Noise noiseInstance = Noise(seed);
	std::vector<float> result;
	result.reserve((x_max-x_min)*size);
	for (int i_y = y_offset; i_y < size+y_offset; ++i_y)
	{
		for (int i_x = x_min; i_x < x_max; ++i_x)
		{
			result.push_back((float) noiseInstance.outputValue(i_x,i_y));
		}
	}
	return result;
}
