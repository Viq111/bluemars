#ifndef INCLUDE_BLUEMARSMAP
#define INCLUDE_BLUEMARSMAP

#include <string>
#include <map>
#include <memory>
#include <iostream>
#include "baselayer.h"
#include "simpleAdditionLayer.h"

class BlueMarsMap
{
 public:
  void addLayer(std::string, std::shared_ptr<BaseLayer>);
  float get(std::string, long, long);
  void listchunks();
  int nbChunks();
 private:
  std::map <std::string, std::shared_ptr<BaseLayer> > layers;
};

void test();

#endif
