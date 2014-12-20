#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include "2dvisualizer.h"

void MainWindow::main() {
	// Create SFML's window.
	auto render_window = std::make_shared<sf::RenderWindow>(sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "BlueMars 2D Visualizer" );
	render_window->setVerticalSyncEnabled(true); // Enable vsync to stop hogging CPU

	// Create a window for ElevationLayer
	std::shared_ptr<ElevationLayer> sa_layer = std::make_shared<ElevationLayer>(ElevationLayer(5));
	//LayerWindow sa_window(sa_layer, "ElevationLayer", render_window);
	auto sa_window = LayerWindow::Create(sa_layer, "ElevationLayer", render_window);

	// Create a desktop and add the window to it.
	sfg::Desktop desktop;
	desktop.Add(sa_window);

	// We're not using SFML to render anything in this program, so reset OpenGL
	// states. Otherwise we wouldn't see anything.
	render_window->resetGLStates();

	// Main loop!
	sf::Event event;
	sf::Clock clock;

	while (render_window->isOpen())
	{
		// Event processing.
		while (render_window->pollEvent(event))
		{
			desktop.HandleEvent(event);
			// If window is about to be closed, leave program.
			if (event.type == sf::Event::Closed)
			{
				render_window->close();
			}
		}
		// Update SFGUI with elapsed seconds since last call.
		desktop.Update(clock.restart().asSeconds());
		// Rendering.
		render_window->clear();
		root.Display(*render_window);
		render_window->display();
	}
}

int main() {
	MainWindow window;
	window.main();
	return 0;
}
