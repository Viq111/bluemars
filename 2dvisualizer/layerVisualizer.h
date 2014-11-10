#ifndef INCLUDE_LAYERVISUALIZER
#define INCLUDE_LAYERVISUALIZER

#include <memory>
#include <string>
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include "baselayer.h"

std::vector<unsigned char> chunk2TGA(std::shared_ptr<ChunkData> chunk, short width, short height); // Convert a chunk to a TGA Image
void writeTGA2File(std::string filename, std::vector<unsigned char> tga_data); // For debug purpose, write a TGA to a file

class ChunkSprite
{
public:
	ChunkSprite(std::shared_ptr<BaseLayer> layer, int chunk_x, int chunk_y);
	int chunk_x;
	int chunk_y;
	sf::Texture texture;
	sf::Sprite sprite;
};

class LayerWindow : public sfg::Window // A scrollable widget linked to a layer
{
public:
	typedef std::shared_ptr<LayerWindow> Ptr;
	static Ptr Create(std::shared_ptr<BaseLayer> layer, std::string layerName, std::shared_ptr<sf::RenderWindow> parent);
	void onStartScroll();
	void onStopScroll();
	void onScroll();
	void onResize();
	LayerWindow();
protected:
	void update(); // Called every frame, check if dirty and redraw if that's the case
	std::string layerName;
	std::shared_ptr<BaseLayer> layer;
	sfg::Canvas::Ptr mainCanvas;
	std::shared_ptr<sf::RenderWindow> parent;
	bool isScrolling; // Whether or not the user is scrolling the image
	sf::Vector2<float> startingXY; // Where the user started scrolling
	sf::Vector2<long> startingUpperLeft;
	sf::Vector2<long> currentUpperLeft; // The current inWorld coordinate of the upper left corner
	sf::Vector2<long> currentLowerRight;
	std::vector<std::shared_ptr<ChunkSprite>> chunk_sprites;
};

#endif