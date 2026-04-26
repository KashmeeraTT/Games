#include "Ball.h"

Ball::Ball(float startX, float startY) : velX(0.0f), velY(0.0f), currentSpeed(Constants::DEFAULT_BALL_SPEED) {
    shape.setSize(sf::Vector2f(Constants::BALL_SIZE, Constants::BALL_SIZE));
    shape.setPosition(startX, startY);
}

void Ball::update() {
    shape.move(velX, velY);
}

void Ball::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void Ball::reset(float startX, float startY, float direction) {
    shape.setPosition(startX, startY);
    velX = direction * Constants::DEFAULT_BALL_SPEED;
    velY = 0.0f;
    currentSpeed = Constants::DEFAULT_BALL_SPEED;
}

sf::FloatRect Ball::getBounds() const {
    return shape.getGlobalBounds();
}

void Ball::reverseY() {
    velY = -velY;
}

void Ball::reverseX() {
    velX = -velX;
}

void Ball::setVelocity(float vx, float vy) {
    velX = vx;
    velY = vy;
}

void Ball::changeSpeed(float amount) {
    currentSpeed += amount;
}

void Ball::move(float dx, float dy) {
    shape.move(dx, dy);
}

void Ball::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

sf::Vector2f Ball::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Ball::getVelocity() const {
    return sf::Vector2f(velX, velY);
}

float Ball::getSpeed() const {
    return currentSpeed;
}
