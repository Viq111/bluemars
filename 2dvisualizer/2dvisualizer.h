#ifndef INCLUDE_2DVISUALIZER
#define INCLUDE_2DVISUALIZER
#include "layerVisualizer.h"
#include "elevationLayer.h"

// Default window
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class MainWindow {
public:
	void main();
private:
	sfg::SFGUI root; // Create an SFGUI. This is required before doing anything with SFGUI.
};


#endif