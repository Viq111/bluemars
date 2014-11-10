#include "layerVisualizer.h"

#include <iostream>
#include <fstream>
#include <math.h>

std::vector<unsigned char> chunk2TGA(std::shared_ptr<ChunkData> chunk, short width, short height)
{
	std::vector<unsigned char> out;
	// Write header
	out.push_back(0);
	out.push_back(0);
	out.push_back(2);						/* uncompressed RGB */
	out.push_back(0);	out.push_back(0);
	out.push_back(0);	out.push_back(0);
	out.push_back(0);
	out.push_back(0);	out.push_back(0);	/* X origin */
	out.push_back(0);	out.push_back(0);	/* y origin */
	// Write sizes - See http://paulbourke.net/dataformats/tga/
	out.push_back((unsigned char)(width & 0x00FF));
	out.push_back((unsigned char)((width & 0xFF00) / 256));
	out.push_back((unsigned char)(height & 0x00FF));
	out.push_back((unsigned char)((height & 0xFF00) / 256));
	out.push_back(24);						/* 24 bits bitmap */
	out.push_back(0);

	// Write chunk
	for (long y = 0; y < height; y++)
	{
		for (long x = 0; x < width; x++)
		{
			// In TGA, origin is lower left and we need upper left, so we search at j and set at i
			//long i = y * width + x;
			long j = (height - y - 1) * width + x;
			int data = static_cast<int>(round(chunk->data.at(j) * 255));
			// B > G > R
			out.push_back((unsigned char)data);
			out.push_back((unsigned char)data);
			out.push_back((unsigned char)data);
		}
	}
	return out;
}
void writeTGA2File(std::string filename, std::vector<unsigned char> tga_data)
{
	std::ofstream file(filename, std::ios::out | std::ios::binary);
	for (unsigned long i = 0; i < tga_data.size(); i++)
	{
		file.put(tga_data.at(i));
	}
	file.close();
}

LayerWindow::LayerWindow(std::shared_ptr<BaseLayer> l, std::string name, std::shared_ptr<sf::RenderWindow> p)
{
	layerName = name;
	layer = l;
	parent = p;
	isScrolling = false;
	startingXY = { 0, 0 };
	currentUpperLeft = {0, 0}; 
	startingUpperLeft = { 0, 0 };
	mainWindow = sfg::Window::Create();
	mainWindow->SetTitle(layerName);
	mainCanvas = sfg::Canvas::Create();
	mainCanvas->SetRequisition(sf::Vector2f(200, 200));
	mainWindow->Add(mainCanvas);
	mainCanvas->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(std::bind(&LayerWindow::onStartScroll, this));
	mainCanvas->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(std::bind(&LayerWindow::onStopScroll, this));
	mainCanvas->GetSignal(sfg::Widget::OnMouseLeave).Connect(std::bind(&LayerWindow::onStopScroll, this));
	mainCanvas->GetSignal(sfg::Widget::OnMouseMove).Connect(std::bind(&LayerWindow::onScroll, this));
	mainWindow->GetSignal(sfg::Widget::OnSizeAllocate).Connect(std::bind(&LayerWindow::onResize, this));
	update();
}

void LayerWindow::onStartScroll()
{
	isScrolling = true;
	auto mousePos = static_cast<sf::Vector2<float>>(sf::Mouse::getPosition(*parent.get()));
	auto canvasPos = mainCanvas->GetAbsolutePosition();
	startingXY = mousePos - canvasPos;
	startingUpperLeft = currentUpperLeft;
}
void LayerWindow::onStopScroll()
{
	isScrolling = false;
}
void LayerWindow::onScroll()
{
	if (isScrolling)
	{
		// Do the scrolling
		auto mousePos = static_cast<sf::Vector2<float>>(sf::Mouse::getPosition(*parent.get()));
		auto canvasPos = mainCanvas->GetAbsolutePosition();
		auto movedPos = (mousePos - canvasPos) - startingXY;
		currentUpperLeft = startingUpperLeft - static_cast<sf::Vector2<long>>(movedPos);
		update();
	}
	//std::cout << "Moving" << std::endl;
}
void LayerWindow::onResize()
{
	update();
}
void LayerWindow::update()
{
	auto alloc = mainCanvas->GetAllocation();
	currentLowerRight.x = currentUpperLeft.x + (long)round(alloc.width);
	currentLowerRight.y = currentUpperLeft.y + (long)round(alloc.height);
	std::pair<int, int> upperLeftChunk = std::make_pair<int, int>(static_cast<int>(floor((double)currentUpperLeft.x / layer->chunkSize)), static_cast<int>(floor((double)currentUpperLeft.y / layer->chunkSize)));
	std::pair<int, int> lowerRightChunk = std::make_pair<int, int>(static_cast<int>(floor((double)currentLowerRight.x / layer->chunkSize)), static_cast<int>(floor((double)currentLowerRight.y / layer->chunkSize)));
	
	// 1 - Unload unused chunk (Too far)
	{
		auto it = chunk_sprites.begin();
		while (it != chunk_sprites.end())
		{
			if (((*it)->chunk_x < upperLeftChunk.first - 2) || ((*it)->chunk_x > lowerRightChunk.first + 2) || ((*it)->chunk_y < upperLeftChunk.second - 2) || ((*it)->chunk_y > lowerRightChunk.first + 2))
			{
				// Chunk is outside of visible box + 2, remove it
				std::cout << "Unloading chunk " << (*it)->chunk_x << ";" << (*it)->chunk_y << std::endl;
				it = chunk_sprites.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	
	// For each chunk in the visible box
	for (int i = upperLeftChunk.first; i <= lowerRightChunk.first; i++)
	{
		for (int j = upperLeftChunk.second; j <= lowerRightChunk.second; j++)
		{
			layer->get(i * layer->chunkSize, j * layer->chunkSize); // For each chunk, be sure they are generated
			
			// 2 - Load needed chunk
			bool toLoad = true;
			for (auto it = chunk_sprites.begin(); it != chunk_sprites.end(); it++) // Check if the sprite is already loaded
			{
				if ((*it)->chunk_x == i && (*it)->chunk_y == j)
				{
					toLoad = false;
					break;
				}
			}
			if (toLoad) // Load Chunk
			{
				auto ptr = std::make_shared<ChunkSprite>(layer, i, j);
				chunk_sprites.push_back(ptr);
				std::cout << "Loading chunk " << i << ";" << j << std::endl;
			}
			
		}
	}
	
	mainCanvas->Bind();
	mainCanvas->Clear(sf::Color(255, 0, 0, 0), true);

	// 3 - Update sprite position & Draw
	for (auto it = chunk_sprites.begin(); it != chunk_sprites.end(); it++)
	{
		long posX = (*it)->chunk_x * layer->chunkSize - currentUpperLeft.x;
		long posY = (*it)->chunk_y * layer->chunkSize - currentUpperLeft.y;
		(*it)->sprite.setPosition(sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY)));
		mainCanvas->Draw((*it)->sprite);
	}
	mainCanvas->Display();
	mainCanvas->Unbind();
	
}

sfg::Window::Ptr LayerWindow::getWindow()
{
	return mainWindow;
}


// ChunkSprite
ChunkSprite::ChunkSprite(std::shared_ptr<BaseLayer> layer, int cx, int cy)
{
	chunk_x = cx;
	chunk_y = cy;
	auto chunk = layer->getChunk(chunk_x, chunk_y);
	auto tga = chunk2TGA(chunk, layer->chunkSize, layer->chunkSize);
	texture.loadFromMemory(tga.data(), tga.size());
	sprite.setTexture(texture);
}