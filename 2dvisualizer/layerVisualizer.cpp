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
	out.push_back((width & 0x00FF));
	out.push_back((width & 0xFF00) / 256);
	out.push_back((height & 0x00FF));
	out.push_back((height & 0xFF00) / 256);
	out.push_back(24);						/* 24 bits bitmap */
	out.push_back(0);

	// Write chunk
	for (long i = 0; i<height*width; i++)
	{
		int data = round(chunk->data.at(i) * 255);
		// B > G > R
		out.push_back(data);
		out.push_back(data);
		out.push_back(data);
	}
	return out;
}
void writeTGA2File(std::string filename, std::vector<unsigned char> tga_data)
{
	std::ofstream file(filename, std::ios::out | std::ios::binary);
	for (long i = 0; i < tga_data.size(); i++)
	{
		file.put(tga_data.at(i));
	}
	file.close();
}

LayerWindow::LayerWindow(std::shared_ptr<BaseLayer>, std::string name, std::shared_ptr<sf::RenderWindow> p)
{
	layerName = name;
	parent = p;
	isScrolling = false;
	startingXY.x = 0; startingXY.y = 0;
	mainWindow = sfg::Window::Create();
	mainWindow->SetTitle(layerName);
	mainCanvas = sfg::Canvas::Create();
	mainCanvas->SetRequisition(sf::Vector2f(200, 200));
	mainWindow->Add(mainCanvas);
	mainCanvas->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(std::bind(&LayerWindow::onStartScroll, this));
	mainCanvas->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect(std::bind(&LayerWindow::onStopScroll, this));
	mainCanvas->GetSignal(sfg::Widget::OnMouseLeave).Connect(std::bind(&LayerWindow::onStopScroll, this));
	mainCanvas->GetSignal(sfg::Widget::OnMouseMove).Connect(std::bind(&LayerWindow::onScroll, this));
}

void LayerWindow::onStartScroll()
{
	isScrolling = true;
	auto mousePos = static_cast<sf::Vector2<float>>(sf::Mouse::getPosition(*parent.get()));
	auto canvasPos = mainCanvas->GetAbsolutePosition();
	startingXY = mousePos - canvasPos;
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
	}
	//std::cout << "Moving" << std::endl;
}

sfg::Window::Ptr LayerWindow::getWindow()
{
	return mainWindow;
}
