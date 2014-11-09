#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include "2dvisualizer.h"

void MainWindow::main() {
	// Create SFML's window.
	sf::RenderWindow render_window( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "BlueMars 2D Visualizer" );

	// Create a simpleAdditionLayer
	simpleAdditionLayer sa_layer;
	sa_layer.get(0, 0); // Populate chunk 0,0
	// Create the image
	sf::Image layerImage;
	auto tga = chunk2TGA(sa_layer.getChunk(0, 0),sa_layer.chunkSize, sa_layer.chunkSize);
	layerImage.loadFromMemory(tga.data(), tga.size());
	// Create a simpleAdditionLayer window
	auto sfgImage = sfg::Image::Create();
	sfgImage->SetImage(layerImage);

	// Create a vertical box layouter with 5 pixels spacing and add the label
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	box->Pack(sfgImage);

	// Create a window and add the box layouter to it. Also set the window's title.
	auto window = sfg::Window::Create();
	window->SetTitle("simpleAdditionLayer");
	window->Add(box);

	// Create a desktop and add the window to it.
	sfg::Desktop desktop;
	desktop.Add(window);

	// We're not using SFML to render anything in this program, so reset OpenGL
	// states. Otherwise we wouldn't see anything.
	render_window.resetGLStates();

	// Main loop!
	sf::Event event;
	sf::Clock clock;

	while( render_window.isOpen() ) {
		// Event processing.
		while( render_window.pollEvent( event ) ) {
			desktop.HandleEvent( event );

			// If window is about to be closed, leave program.
			if( event.type == sf::Event::Closed ) {
				render_window.close();
			}
		}

		// Update SFGUI with elapsed seconds since last call.
		desktop.Update( clock.restart().asSeconds() );
		// Rendering.
		render_window.clear();
		root.Display(render_window);
		render_window.display();
	}
}

int main() {
	MainWindow window;
	window.main();
	return 0;
}
