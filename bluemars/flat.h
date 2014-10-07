#include "baselayer.h"
#include "chunkdata.h"

class Flat: public BaseLayer
{
 public:
  void preGenerateChunk(unsigned int, unsigned int);
  Flat();
 private:
  unsigned short chunkSize = 1024;
  std::vector< std::shared_ptr<BaseLayer> > dependentLayers;
  std::map< std::pair<unsigned int, unsigned int>, std::shared_ptr<ChunkData> > data ;

};

