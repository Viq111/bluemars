#ifndef INCLUDE_FLAT
#define INCLUDE_FLAT

#include <string>
#include <iostream>
#include "baselayer.h"
#include "chunkdata.h"

class Flat: public BaseLayer
{
 public:
  void preGenerateChunk(unsigned int, unsigned int);
  Flat();
};

#endif
