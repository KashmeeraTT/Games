#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// UIManager.h - Rendering text and buttons
class UIManager {
public:
    UIManager(const sf::Font& font, float screenWidth, float screenHeight);

    void drawMenu(sf::RenderWindow& window, sf::Vector2i mousePos);
    void drawDifficultySelect(sf::RenderWindow& window, sf::Vector2i mousePos);
    void drawNameEntry(sf::RenderWindow& window, bool enteringP1, const std::string& p1Name, const std::string& p2Name);
    void drawHUD(sf::RenderWindow& window, int p1Score, int p2Score, const std::string& p1Name, const std::string& p2Name);
    void drawGameOver(sf::RenderWindow& window, int p1Score, int p2Score, const std::string& p1Name, const std::string& p2Name, sf::Vector2i mousePos);
    void drawHistory(sf::RenderWindow& window, const std::vector<std::string>& historyLines, int currentPage, int maxPages);
    void drawError(sf::RenderWindow& window, const std::string& message);

    bool isHovering(const sf::Text& text, sf::Vector2i mousePos) const;
    void updateHoverState(sf::Text& text, sf::Vector2i mousePos) const;

    sf::Text btn1Player;
    sf::Text btn2Player;
    sf::Text btnHistory;
    sf::Text btnQuit;

    sf::Text btnEasy;
    sf::Text btnMedium;
    sf::Text btnHard;

    sf::Text btnRematch;
    sf::Text btnMenu;

private:
    const sf::Font& font;
    float screenWidth;
    float screenHeight;

    sf::Text titleText;
    sf::Text uiText;
    sf::Text scoreText;

    void centerTextX(sf::Text& text, float y);
};
