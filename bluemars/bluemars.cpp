#include "bluemars.h"

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

void BlueMarsMap::listchunks()
{
  for(auto it=layers.begin(); it!=layers.end(); ++it)
    {
	  std::cout << std::endl << "*** " << it->first << " ***" << std::endl;;
      std::shared_ptr<BaseLayer> layer=it->second;
      layer -> listchunks();
      std::cout << std::endl << std::endl;
    }
}

int main(int argc, char** argv)
{
	//test();
}

void test()
{
  BlueMarsMap map;
  new Flat();
  map.addLayer("flat", std::make_shared<Flat> (Flat()));
  map.addLayer("flat2", std::make_shared<Flat> (Flat()));
  std::cout << map.get("flat", 10000, 10000) << std::endl;
  map.listchunks();
  std::cout << map.get("flat", 10, 10) << std::endl;
  map.listchunks();
  std::cout << map.get("flat", 10002, 10002) << std::endl;
  std::cout << map.get("flat", 20, 20) << std::endl;
  std::cout << map.get("flat2",10,10) << std::endl;
  map.listchunks();
}
