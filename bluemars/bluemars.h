#ifndef INCLUDE_BLUEMARSMAP
#define INCLUDE_BLUEMARSMAP

#include <string>
#include <map>
#include <memory>
#include <iostream>
#include "windowsSpecific.h"
#include "baselayer.h"
#include "simpleAdditionLayer.h"

class BlueMars_EXPORT BlueMarsMap
{
 public:
  void addLayer(std::string, std::shared_ptr<BaseLayer>);
  float get(std::string, long, long);
  void listchunks();
  unsigned long nbChunks();
 private:
  std::map <std::string, std::shared_ptr<BaseLayer> > layers;
};

void test();

#endif
