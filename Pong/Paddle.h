#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

// Paddle.h - Handles paddle movement, AI logic, and drawing
class Paddle {
public:
    Paddle(float startX, float startY);
    void updatePlayer(bool isUpPressed, bool isDownPressed, float maxHeight);
    void updateAI(float ballY, Constants::AIDifficulty difficulty, float maxHeight);
    void draw(sf::RenderWindow& window) const;
    void reset(float startX, float startY);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

private:
    sf::RectangleShape shape;
    float playerSpeed;
};
