#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

// Ball.h - Handles ball physics, velocity, resetting and drawing
class Ball {
public:
    Ball(float startX, float startY);
    void update(float fraction = 1.0f);
    void draw(sf::RenderWindow& window) const;
    void reset(float startX, float startY, float dX);
    sf::FloatRect getBounds() const;

    // Physics API
    void reverseY();
    void reverseX();
    void setVelocity(float vx, float vy);
    void changeSpeed(float amount);
    void syncSpeed();
    void move(float dx, float dy);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    float getSpeed() const;

private:
    sf::RectangleShape shape;
    float velX;
    float velY;
    float currentSpeed;
};
