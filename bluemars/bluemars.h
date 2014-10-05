#ifndef INCLUDE_BLUEMARSMAP
#define INCLUDE_BLUEMARSMAP

#include <map>
#include "baselayer.h"

class BlueMarsMap
{
 public:
  BlueMarsMap();
  ~BlueMarsMap();
  void addLayer(std::string, BaseLayer);
 private:
  std::map <std::string, BaseLayer> layers;
};

#endif
