#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Game::Game(float w, float h, sf::RenderWindow& win) 
    : screenWidth(w), screenHeight(h), window(win),
      state(Constants::GameState::MENU),
      currentMode(Constants::GameMode::SINGLE_PLAYER),
      aiDifficulty(Constants::AIDifficulty::MEDIUM),
      p1Score(0), p2Score(0),
      enteringP1Name(true),
      shakeTime(0.0f),
      currentHistoryPage(0), totalHistoryPages(1),
      uiManager(nullptr),
      ball(screenWidth / 2, screenHeight / 2),
      leftPaddle(50, screenHeight / 2 - 60),
      rightPaddle(screenWidth - 70, screenHeight / 2 - 60),
      obstacle(screenWidth / 2 - 15, screenHeight / 2 - 75),
      scoreManager("scores.txt")
{
    // Initialize Random
    std::random_device rd;
    rng = std::mt19937(rd());

    defaultView = window.getDefaultView();

    // Verify Font loading
    if (!font.loadFromFile("arial.ttf")) {
        state = Constants::GameState::ERROR_STATE;
        std::cerr << "Failed to load font 'arial.ttf'" << std::endl;
        // The game will just show a black screen in the error state. Ideal would be using a default SFML font, but SFML doesn't have one builtin that easily.
    } else {
        uiManager = std::make_unique<UIManager>(font, screenWidth, screenHeight);
    }

    // Audio
    if (hitBuffer.loadFromFile("hit.wav")) hitSound.setBuffer(hitBuffer);
    if (scoreBuffer.loadFromFile("score.wav")) scoreSound.setBuffer(scoreBuffer);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // --- MOUSE CLICK LOGIC ---
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (state == Constants::GameState::MENU && uiManager) {
                switch (uiManager->getMenuClick(mousePos)) {
                    case MenuClick::ONE_PLAYER:
                        currentMode = Constants::GameMode::SINGLE_PLAYER;
                        state = Constants::GameState::DIFFICULTY_SELECT;
                        break;
                    case MenuClick::TWO_PLAYER:
                        currentMode = Constants::GameMode::MULTIPLAYER;
                        state = Constants::GameState::NAME_ENTRY;
                        enteringP1Name = true;
                        p1Name = ""; p2Name = "";
                        break;
                    case MenuClick::HISTORY:
                        state = Constants::GameState::HISTORY;
                        currentHistoryPage = 0;
                        loadHistoryPage();
                        break;
                    case MenuClick::QUIT:
                        window.close();
                        break;
                    default: break;
                }
            }
            else if (state == Constants::GameState::DIFFICULTY_SELECT && uiManager) {
                switch (uiManager->getDifficultyClick(mousePos)) {
                    case DifficultyClick::EASY:
                        aiDifficulty = Constants::AIDifficulty::EASY;
                        state = Constants::GameState::NAME_ENTRY;
                        enteringP1Name = true;
                        p1Name = ""; p2Name = "Computer";
                        break;
                    case DifficultyClick::MEDIUM:
                        aiDifficulty = Constants::AIDifficulty::MEDIUM;
                        state = Constants::GameState::NAME_ENTRY;
                        enteringP1Name = true;
                        p1Name = ""; p2Name = "Computer";
                        break;
                    case DifficultyClick::HARD:
                        aiDifficulty = Constants::AIDifficulty::HARD;
                        state = Constants::GameState::NAME_ENTRY;
                        enteringP1Name = true;
                        p1Name = ""; p2Name = "Computer";
                        break;
                    default: break;
                }
            }
            else if (state == Constants::GameState::GAME_OVER && uiManager) {
                switch (uiManager->getGameOverClick(mousePos)) {
                    case GameOverClick::REMATCH:
                        startMatch();
                        break;
                    case GameOverClick::MENU:
                        state = Constants::GameState::MENU;
                        break;
                    default: break;
                }
            }
            else if (state == Constants::GameState::HISTORY) {
                // Return to menu on click
                state = Constants::GameState::MENU;
            }
        }

        // --- KEYBOARD / TYPING LOGIC ---
        if (state == Constants::GameState::NAME_ENTRY && event.type == sf::Event::TextEntered) {
            char typed = static_cast<char>(event.text.unicode);
            std::string* activeName = enteringP1Name ? &p1Name : &p2Name;

            if (typed == '\b' && !activeName->empty()) {
                activeName->pop_back(); // Backspace
            }
            else if (typed == '\r' && !activeName->empty()) { // Enter key
                if (enteringP1Name && currentMode == Constants::GameMode::MULTIPLAYER) {
                    enteringP1Name = false; // Move to P2
                }
                else {
                    startMatch();
                }
            }
            else if (typed >= 32 && typed <= 126 && activeName->length() < 10) {
                *activeName += typed; // Add character
            }
        }
        else if (state == Constants::GameState::HISTORY && event.type == sf::Event::KeyPressed) {
             if (event.key.code == sf::Keyboard::Left) {
                if (currentHistoryPage > 0) {
                    currentHistoryPage--;
                    loadHistoryPage();
                }
            }
            else if (event.key.code == sf::Keyboard::Right) {
                if (currentHistoryPage < totalHistoryPages - 1) {
                    currentHistoryPage++;
                    loadHistoryPage();
                }
            }
        }

        // Global Escape
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (state == Constants::GameState::PLAYING || state == Constants::GameState::DIFFICULTY_SELECT || state == Constants::GameState::NAME_ENTRY || state == Constants::GameState::HISTORY || state == Constants::GameState::GAME_OVER) {
                state = Constants::GameState::MENU;
                window.setView(defaultView); // reset view just in case
            }
        }
    }
}

void Game::update() {
    if (state == Constants::GameState::PLAYING) {
        // Screen Shake Timer Decay
        if (shakeTime > 0) {
            shakeTime -= Constants::SHAKE_DECAY;
        } else {
            window.setView(defaultView);
        }

        // Update Entities
        obstacle.update(screenHeight);

        // P1 Controls
        leftPaddle.updatePlayer(sf::Keyboard::isKeyPressed(sf::Keyboard::W), sf::Keyboard::isKeyPressed(sf::Keyboard::S), screenHeight);

        // P2 Controls
        if (currentMode == Constants::GameMode::SINGLE_PLAYER) {
            rightPaddle.updateAI(ball.getPosition().y, aiDifficulty, screenHeight);
        } else {
            rightPaddle.updatePlayer(sf::Keyboard::isKeyPressed(sf::Keyboard::Up), sf::Keyboard::isKeyPressed(sf::Keyboard::Down), screenHeight);
        }

        int substeps = static_cast<int>(std::ceil(ball.getSpeed() / 10.0f));
        substeps = std::max(1, substeps);
        float fraction = 1.0f / static_cast<float>(substeps);

        for (int i = 0; i < substeps; i++) {
            ball.update(fraction);
            checkCollisions();
        }

        // Scoring
        if (ball.getPosition().x < 0) {
            scorePoint(Constants::PaddleSide::RIGHT); // Right scores
        } else if (ball.getPosition().x > screenWidth) {
            scorePoint(Constants::PaddleSide::LEFT); // Left scores
        }
    }
}

void Game::checkCollisions() {
    // Top / Bottom Wall Collision
    if ((ball.getPosition().y <= 0 && ball.getVelocity().y < 0) || (ball.getPosition().y >= screenHeight - Constants::BALL_SIZE && ball.getVelocity().y > 0)) {
        ball.reverseY();
        if (hitSound.getBuffer()) hitSound.play();
    }

    // Paddle Collisions
    handleBallPaddleCollision(leftPaddle, Constants::PaddleSide::LEFT);
    handleBallPaddleCollision(rightPaddle, Constants::PaddleSide::RIGHT);

    // Obstacle Collision
    sf::FloatRect ballBounds = ball.getBounds();
    sf::FloatRect obsBounds = obstacle.getBounds();
    sf::FloatRect obsOverlap;

    if (ballBounds.intersects(obsBounds, obsOverlap)) {
        float bCenterX = ballBounds.left + ballBounds.width / 2.0f;
        float bCenterY = ballBounds.top + ballBounds.height / 2.0f;
        float oCenterX = obsBounds.left + obsBounds.width / 2.0f;
        float oCenterY = obsBounds.top + obsBounds.height / 2.0f;

        if (obsOverlap.width < obsOverlap.height) {
            // Hit the side of the obstacle
            if ((bCenterX < oCenterX && ball.getVelocity().x > 0) || (bCenterX > oCenterX && ball.getVelocity().x < 0)) {
                ball.reverseX();
            }
            ball.move(bCenterX < oCenterX ? -obsOverlap.width : obsOverlap.width, 0);
        } else {
            // Hit the top/bottom of the obstacle
            if ((bCenterY < oCenterY && ball.getVelocity().y > 0) || (bCenterY > oCenterY && ball.getVelocity().y < 0)) {
                ball.reverseY();
            }
            ball.move(0, bCenterY < oCenterY ? -obsOverlap.height : obsOverlap.height);
        }
        if (hitSound.getBuffer()) hitSound.play();
    }
}

void Game::handleBallPaddleCollision(Paddle& paddle, Constants::PaddleSide side) {
    sf::FloatRect bBounds = ball.getBounds();
    sf::FloatRect pBounds = paddle.getBounds();
    sf::FloatRect overlap;

    if (bBounds.intersects(pBounds, overlap)) {
        float bCenterX = bBounds.left + bBounds.width / 2.0f;
        float bCenterY = bBounds.top + bBounds.height / 2.0f;
        float pCenterX = pBounds.left + pBounds.width / 2.0f;
        float pCenterY = pBounds.top + pBounds.height / 2.0f;

        float directionX = (side == Constants::PaddleSide::LEFT) ? 1.0f : -1.0f;

        if (overlap.width < overlap.height) {
            bool hitFront = (directionX > 0 && bCenterX >= pCenterX) || (directionX < 0 && bCenterX <= pCenterX);
            if (hitFront) {
                // Hit the front face (normal hit)
                ball.changeSpeed(0.5f);
                ball.setPosition(directionX > 0 ? pBounds.left + pBounds.width : pBounds.left - bBounds.width, bBounds.top);
                float hitPoint = bCenterY - pCenterY;
                ball.setVelocity(directionX * ball.getSpeed(), (hitPoint / 60.0f) * ball.getSpeed());
                shakeTime = Constants::SHAKE_DURATION_HIT;
            } else {
                // Hit the back face (ball passed paddle)
                if ((bCenterX < pCenterX && ball.getVelocity().x > 0) || (bCenterX > pCenterX && ball.getVelocity().x < 0)) {
                    ball.reverseX();
                }
                ball.move(bCenterX < pCenterX ? -overlap.width : overlap.width, 0);
            }
        } else {
            // Hit the top/bottom edge! (Bounce vertically safely)
            if ((bCenterY < pCenterY && ball.getVelocity().y > 0) || (bCenterY > pCenterY && ball.getVelocity().y < 0)) {
                ball.reverseY();
            }
            ball.move(0, bCenterY < pCenterY ? -overlap.height : overlap.height);
        }
        if (hitSound.getBuffer()) hitSound.play();
    }
}

void Game::scorePoint(const Constants::PaddleSide scoringSide) {
    if (scoreSound.getBuffer()) scoreSound.play();

    float spawnX = 0;
    float spawnY = screenHeight / 2.0f - (Constants::BALL_SIZE / 2.0f);
    float serveDirection = 1.0f;

    // Spawn dynamically based on obstacle bounds so it doesn't clip
    float oLeft = obstacle.getPosition().x;
    float oRight = obstacle.getPosition().x + obstacle.getBounds().width;

    if (scoringSide == Constants::PaddleSide::RIGHT) {
        p2Score++;
        serveDirection = -1.0f; // Serve Left
        spawnX = oLeft - Constants::BALL_SIZE - 20.0f;
    } else {
        p1Score++;
        serveDirection = 1.0f;  // Serve Right
        spawnX = oRight + 20.0f;
    }

    ball.reset(spawnX, spawnY, serveDirection);
    shakeTime = Constants::SHAKE_DURATION_SCORE;

    if (p1Score >= Constants::WINNING_SCORE || p2Score >= Constants::WINNING_SCORE) {
        state = Constants::GameState::GAME_OVER;
        window.setView(defaultView); // Reset view immediately
        saveScore();
    }
}

void Game::startMatch() {
    p1Score = 0; p2Score = 0;
    leftPaddle.reset(50, screenHeight / 2 - (Constants::PADDLE_HEIGHT / 2));
    rightPaddle.reset(screenWidth - 70, screenHeight / 2 - (Constants::PADDLE_HEIGHT / 2));
    obstacle.reset(screenWidth / 2 - (Constants::OBSTACLE_WIDTH / 2), screenHeight / 2 - (Constants::OBSTACLE_HEIGHT / 2));

    // Initial serve Right
    float oRight = obstacle.getPosition().x + obstacle.getBounds().width;
    ball.reset(oRight + 20.0f, screenHeight / 2 - (Constants::BALL_SIZE / 2), 1.0f);

    shakeTime = 0.0f;
    state = Constants::GameState::PLAYING;
}

void Game::saveScore() {
    std::string winner = (p1Score >= Constants::WINNING_SCORE) ? p1Name : p2Name;
    std::string loser = (p1Score >= Constants::WINNING_SCORE) ? p2Name : p1Name;
    int winScore = (p1Score >= Constants::WINNING_SCORE) ? p1Score : p2Score;
    int loseScore = (p1Score >= Constants::WINNING_SCORE) ? p2Score : p1Score;

    scoreManager.addMatch(winner, winScore, loser, loseScore);
}

void Game::loadHistoryPage() {
    int totalMatches = scoreManager.getHistoryPage(currentHistoryPage * HISTORY_LIMIT, HISTORY_LIMIT, currentHistoryLines);
    totalHistoryPages = std::max(1, (int)std::ceil((float)totalMatches / HISTORY_LIMIT));

    if (currentHistoryPage >= totalHistoryPages) {
         currentHistoryPage = totalHistoryPages - 1;
         scoreManager.getHistoryPage(currentHistoryPage * HISTORY_LIMIT, HISTORY_LIMIT, currentHistoryLines);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    if (state == Constants::GameState::ERROR_STATE) {
        if (uiManager) {
            uiManager->drawError(window, "Failed to load 'arial.ttf'. Please ensure the font file is in the same folder as the executable.");
        } else {
            sf::RectangleShape bg(sf::Vector2f(screenWidth, screenHeight));
            bg.setFillColor(sf::Color::Red);
            window.draw(bg);
        }
        window.display();
        return;
    }

    // Apply Screen Shake
    if (state == Constants::GameState::PLAYING && shakeTime > 0) {
        sf::View shakeView = defaultView;
        shakeView.move(Utils::randomFloat(rng, -5, 5), Utils::randomFloat(rng, -5, 5));
        window.setView(shakeView);
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (state == Constants::GameState::MENU) {
        uiManager->drawMenu(window, mousePos);
    } 
    else if (state == Constants::GameState::DIFFICULTY_SELECT) {
        uiManager->drawDifficultySelect(window, mousePos);
    }
    else if (state == Constants::GameState::NAME_ENTRY) {
        uiManager->drawNameEntry(window, enteringP1Name, p1Name, p2Name);
    }
    else if (state == Constants::GameState::PLAYING) {
        uiManager->drawHUD(window, p1Score, p2Score, p1Name, p2Name);
        leftPaddle.draw(window);
        rightPaddle.draw(window);
        obstacle.draw(window);
        ball.draw(window);
    }
    else if (state == Constants::GameState::GAME_OVER) {
        uiManager->drawGameOver(window, p1Score, p2Score, p1Name, p2Name, mousePos);
    }
    else if (state == Constants::GameState::HISTORY) {
        uiManager->drawHistory(window, currentHistoryLines, currentHistoryPage, totalHistoryPages);
    }

    window.display();
}