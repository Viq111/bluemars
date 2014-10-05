#include <vector>
#include <iostream>
#include "bluemars.h"
#include "baselayer.h"

void BlueMarsMap::addLayer(std::string name, BaseLayer layer)
{
  layers[name] = layer;
}

int main(int argc, char** argv)
{
  std::cout << "Hello World" << std::endl;
  return 0;
}
