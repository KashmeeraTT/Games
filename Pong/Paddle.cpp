#include "Paddle.h"

Paddle::Paddle(float startX, float startY) : playerSpeed(Constants::PLAYER_SPEED) {
    shape.setSize(sf::Vector2f(Constants::PADDLE_WIDTH, Constants::PADDLE_HEIGHT));
    shape.setPosition(startX, startY);
}

void Paddle::updatePlayer(bool isUpPressed, bool isDownPressed, float maxHeight) {
    if (isUpPressed && shape.getPosition().y > 0) {
        shape.move(0, -playerSpeed);
    }
    if (isDownPressed && shape.getPosition().y < maxHeight - shape.getSize().y) {
        shape.move(0, playerSpeed);
    }
}

void Paddle::updateAI(float ballY, Constants::AIDifficulty difficulty, float maxHeight) {
    float aiSpeed = 0.0f;
    float offset = 0.0f;
    switch (difficulty) {
        case Constants::AIDifficulty::EASY:   aiSpeed = 6.0f;  offset = 40.0f; break;
        case Constants::AIDifficulty::MEDIUM: aiSpeed = 8.5f;  offset = 20.0f; break;
        case Constants::AIDifficulty::HARD:   aiSpeed = 9.5f;  offset = 8.0f;  break;
    }

    float aiCenter = shape.getPosition().y + (Constants::PADDLE_HEIGHT / 2);
    float ballCenter = ballY + (Constants::BALL_SIZE / 2);

    if (aiCenter < ballCenter - offset && shape.getPosition().y < maxHeight - shape.getSize().y) {
        shape.move(0, aiSpeed);
    }
    else if (aiCenter > ballCenter + offset && shape.getPosition().y > 0) {
        shape.move(0, -aiSpeed);
    }
}

void Paddle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void Paddle::reset(float startX, float startY) {
    shape.setPosition(startX, startY);
}

sf::FloatRect Paddle::getBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const {
    return shape.getPosition();
}
