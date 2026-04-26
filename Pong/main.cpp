#include <SFML/Graphics.hpp>
#include "Game.h"

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Ultimate Pong", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    Game game(desktop.width, desktop.height, window);
    game.run();

    return 0;
}