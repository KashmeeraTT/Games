#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <random>

#include "Constants.h"
#include "Ball.h"
#include "Paddle.h"
#include "Obstacle.h"
#include "ScoreManager.h"
#include "UIManager.h"

// Game.h - Main loop and logic coordinator
class Game {
public:
    Game(float width, float height, sf::RenderWindow& window);
    ~Game();
    void run();

private:
    sf::RenderWindow& window;
    float screenWidth;
    float screenHeight;
    sf::View defaultView;

    sf::Font font;
    sf::SoundBuffer hitBuffer, scoreBuffer;
    sf::Sound hitSound, scoreSound;

    // Managers
    UIManager* uiManager;
    ScoreManager scoreManager;

    // Entities
    Ball ball;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Obstacle obstacle;

    // State
    Constants::GameState state;
    Constants::GameMode currentMode;
    Constants::AIDifficulty aiDifficulty;

    int p1Score;
    int p2Score;
    std::string p1Name;
    std::string p2Name;
    bool enteringP1Name;

    float shakeTime;
    std::mt19937 rng; // Random number generator

    // History Pagination
    int currentHistoryPage;
    int totalHistoryPages;
    const int HISTORY_LIMIT = 10;
    std::vector<std::string> currentHistoryLines;

    void handleEvents();
    void update();
    void render();

    // Core Logic
    void checkCollisions();
    void handleBallPaddleCollision(Paddle& paddle, Constants::PaddleSide side);
    void scorePoint(const Constants::PaddleSide scoringSide);
    void startMatch();
    void saveScore();
    void loadHistoryPage();
    float getRandomFloat(float min, float max);
};
