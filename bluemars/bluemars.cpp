#include <vector>
#include <iostream>
#include <memory>
#include "bluemars.h"
#include "baselayer.h"
#include "flat.h"

void BlueMarsMap::addLayer(std::string name, std::shared_ptr<BaseLayer> layer_ptr)
{
  layers[name] = layer_ptr;
}

float BlueMarsMap::get(std::string name, long x, long y)
{
  if (layers.find(name) == layers.end())
    {
      std::cout << "Layer not found" << std::endl;
      return (float)0;
    }
  else {
    std::shared_ptr<BaseLayer> layer = layers[name];
    return layer->get(x, y);
  }
}

int main(int argc, char** argv)
{
  BlueMarsMap map;
  new Flat();
  map.addLayer("flat", std::make_shared<Flat> (Flat()));
  std::cout << map.get("flat", 10000, 10000) << std::endl;
}
