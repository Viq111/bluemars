#ifndef INCLUDE_BLUEMARSMAP
#define INCLUDE_BLUEMARSMAP

#include <map>
#include <memory>
#include "baselayer.h"

class BlueMarsMap
{
 public:
  void addLayer(std::string, std::shared_ptr<BaseLayer>);
  float get(std::string, long, long);
 private:
  std::map <std::string, std::shared_ptr<BaseLayer> > layers;
};

#endif
