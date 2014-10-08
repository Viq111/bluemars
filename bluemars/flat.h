#ifndef INCLUDE_FLAT
#define INCLUDE_FLAT

#include <string>
#include <iostream>
#include "baselayer.h"
#include "chunkdata.h"

class Flat: public BaseLayer
{
public:
	Flat();
	void preGenerateChunk(unsigned int, unsigned int);
  
};

#endif
