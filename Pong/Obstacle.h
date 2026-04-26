#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

// Obstacle.h - moving object logic
class Obstacle {
public:
    Obstacle(float startX, float startY);
    void update(float maxHeigth);
    void draw(sf::RenderWindow& window) const;
    void reset(float startX, float startY);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

private:
    sf::RectangleShape shape;
    float currentSpeed;
};
 