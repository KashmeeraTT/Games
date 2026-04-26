#include "Obstacle.h"

Obstacle::Obstacle(float startX, float startY) : currentSpeed(Constants::OBSTACLE_SPEED) {
    shape.setSize(sf::Vector2f(Constants::OBSTACLE_WIDTH, Constants::OBSTACLE_HEIGHT));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(startX, startY);
}

void Obstacle::update(float maxHeight) {
    shape.move(0, currentSpeed);
    if (shape.getPosition().y <= 0 || shape.getPosition().y >= maxHeight - shape.getSize().y) {
        currentSpeed = -currentSpeed;
    }
}

void Obstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void Obstacle::reset(float startX, float startY) {
    shape.setPosition(startX, startY);
}

sf::FloatRect Obstacle::getBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Obstacle::getPosition() const {
    return shape.getPosition();
}
