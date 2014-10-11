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

int BlueMarsMap::nbChunks()
{
	int result=0;
	for(auto it=layers.begin(); it!=layers.end(); ++it)
	{
		std::shared_ptr<BaseLayer> layer=it->second;
		result += layer->nbChunks();
	}
	return result;
}
//int main(int argc, char** argv)
//{
  //test();
//}

void test()
{
  BlueMarsMap map;
  map.addLayer("simpleAdditionLayer", std::make_shared<simpleAdditionLayer> (simpleAdditionLayer()));
  map.addLayer("simpleAdditionLayer2", std::make_shared<simpleAdditionLayer> (simpleAdditionLayer()));
  std::cout << map.get("simpleAdditionLayer", 10000, 10000) << std::endl;
  map.listchunks();
  std::cout << map.get("simpleAdditionLayer", 10, 10) << std::endl;
  map.listchunks();
  std::cout << map.get("simpleAdditionLayer", 10002, 10002) << std::endl;
  std::cout << map.get("simpleAdditionLayer", 20, 20) << std::endl;
  std::cout << map.get("simpleAdditionLayer2",10,10) << std::endl;
  map.listchunks();
}
