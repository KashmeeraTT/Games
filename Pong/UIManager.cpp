#include "UIManager.h"
#include "Constants.h"

UIManager::UIManager(const sf::Font& f, float w, float h) : font(f), screenWidth(w), screenHeight(h) {
    // Menu
    titleText.setFont(font);
    titleText.setString("PONG");
    titleText.setCharacterSize(100);
    centerTextX(titleText, 100);

    btn1Player.setFont(font);
    btn1Player.setString("1 Player (vs AI)");
    btn1Player.setCharacterSize(50);
    btn1Player.setPosition(screenWidth / 2 - 200, 300);

    btn2Player.setFont(font);
    btn2Player.setString("2 Players (Local)");
    btn2Player.setCharacterSize(50);
    btn2Player.setPosition(screenWidth / 2 - 200, 400);

    btnHistory.setFont(font);
    btnHistory.setString("Match History");
    btnHistory.setCharacterSize(50);
    btnHistory.setPosition(screenWidth / 2 - 200, 500);

    btnQuit.setFont(font);
    btnQuit.setString("Quit to Desktop");
    btnQuit.setCharacterSize(50);
    btnQuit.setPosition(screenWidth / 2 - 200, 600);

    // Difficulty Select
    btnEasy.setFont(font);
    btnEasy.setString("Easy");
    btnEasy.setCharacterSize(50);
    centerTextX(btnEasy, 300);

    btnMedium.setFont(font);
    btnMedium.setString("Medium");
    btnMedium.setCharacterSize(50);
    centerTextX(btnMedium, 400);

    btnHard.setFont(font);
    btnHard.setString("Hard");
    btnHard.setCharacterSize(50);
    centerTextX(btnHard, 500);

    // Game Over
    btnRematch.setFont(font);
    btnRematch.setString("Rematch");
    btnRematch.setCharacterSize(50);
    centerTextX(btnRematch, screenHeight / 2 + 50);

    btnMenu.setFont(font);
    btnMenu.setString("Back to Menu");
    btnMenu.setCharacterSize(50);
    centerTextX(btnMenu, screenHeight / 2 + 150);

    // Common
    uiText.setFont(font);
    uiText.setCharacterSize(50);

    scoreText.setFont(font);
    scoreText.setCharacterSize(80);
}

void UIManager::centerTextX(sf::Text& text, float y) {
    text.setPosition(screenWidth / 2 - text.getGlobalBounds().width / 2, y);
}

bool UIManager::isHovering(const sf::Text& text, sf::Vector2i mousePos) const {
    return text.getGlobalBounds().contains(mousePos.x, mousePos.y);
}

void UIManager::updateHoverState(sf::Text& text, sf::Vector2i mousePos) const {
    text.setFillColor(isHovering(text, mousePos) ? sf::Color::Yellow : sf::Color::White);
}

MenuClick UIManager::getMenuClick(sf::Vector2i mousePos) const {
    if (isHovering(btn1Player, mousePos)) return MenuClick::ONE_PLAYER;
    if (isHovering(btn2Player, mousePos)) return MenuClick::TWO_PLAYER;
    if (isHovering(btnHistory, mousePos)) return MenuClick::HISTORY;
    if (isHovering(btnQuit, mousePos)) return MenuClick::QUIT;
    return MenuClick::NONE;
}

DifficultyClick UIManager::getDifficultyClick(sf::Vector2i mousePos) const {
    if (isHovering(btnEasy, mousePos)) return DifficultyClick::EASY;
    if (isHovering(btnMedium, mousePos)) return DifficultyClick::MEDIUM;
    if (isHovering(btnHard, mousePos)) return DifficultyClick::HARD;
    return DifficultyClick::NONE;
}

GameOverClick UIManager::getGameOverClick(sf::Vector2i mousePos) const {
    if (isHovering(btnRematch, mousePos)) return GameOverClick::REMATCH;
    if (isHovering(btnMenu, mousePos)) return GameOverClick::MENU;
    return GameOverClick::NONE;
}

void UIManager::drawMenu(sf::RenderWindow& window, sf::Vector2i mousePos) {
    updateHoverState(btn1Player, mousePos);
    updateHoverState(btn2Player, mousePos);
    updateHoverState(btnHistory, mousePos);
    updateHoverState(btnQuit, mousePos);

    window.draw(titleText);
    window.draw(btn1Player);
    window.draw(btn2Player);
    window.draw(btnHistory);
    window.draw(btnQuit);
}

void UIManager::drawDifficultySelect(sf::RenderWindow& window, sf::Vector2i mousePos) {
    uiText.setString("Select AI Difficulty:");
    centerTextX(uiText, 100);
    window.draw(uiText);

    updateHoverState(btnEasy, mousePos);
    updateHoverState(btnMedium, mousePos);
    updateHoverState(btnHard, mousePos);

    window.draw(btnEasy);
    window.draw(btnMedium);
    window.draw(btnHard);
}

void UIManager::drawNameEntry(sf::RenderWindow& window, bool enteringP1, const std::string& p1Name, const std::string& p2Name) {
    std::string prompt = enteringP1 ? "Player 1 Name: " + p1Name + "_" : "Player 2 Name: " + p2Name + "_";
    uiText.setString(prompt + "\n\n(Press ENTER to continue)");
    uiText.setPosition(screenWidth / 2 - uiText.getGlobalBounds().width / 2, screenHeight / 2 - 50);
    window.draw(uiText);
}

void UIManager::drawHUD(sf::RenderWindow& window, int p1Score, int p2Score, const std::string& p1Name, const std::string& p2Name) {
    scoreText.setString(p1Name + " " + std::to_string(p1Score) + "   -   " + std::to_string(p2Score) + " " + p2Name);
    scoreText.setPosition(screenWidth / 2 - scoreText.getGlobalBounds().width / 2, 20);
    window.draw(scoreText);
}

void UIManager::drawGameOver(sf::RenderWindow& window, int p1Score, int p2Score, const std::string& p1Name, const std::string& p2Name, sf::Vector2i mousePos) {
    std::string winText = (p1Score >= Constants::WINNING_SCORE) ? p1Name + " WINS!" : p2Name + " WINS!";
    uiText.setString(winText);
    uiText.setPosition(screenWidth / 2 - uiText.getGlobalBounds().width / 2, screenHeight / 4);
    window.draw(uiText);

    updateHoverState(btnRematch, mousePos);
    updateHoverState(btnMenu, mousePos);

    window.draw(btnRematch);
    window.draw(btnMenu);
}

void UIManager::drawHistory(sf::RenderWindow& window, const std::vector<std::string>& historyLines, int currentPage, int totalPages) {
    std::string fullText = "MATCH HISTORY (Page " + std::to_string(currentPage + 1) + " / " + std::to_string(totalPages) + ")\n";
    fullText += "Use LEFT/RIGHT arrows to navigate. Click anywhere to exit.\n\n";

    if (historyLines.empty()) {
        fullText += "No matches recorded yet.";
    } else {
        for (const auto& line : historyLines) {
            fullText += line + "\n";
        }
    }

    uiText.setString(fullText);
    uiText.setCharacterSize(30);
    uiText.setPosition(100, 100);
    window.draw(uiText);
    uiText.setCharacterSize(50); // Reset for other states
}

void UIManager::drawError(sf::RenderWindow& window, const std::string& message) {
    sf::RectangleShape bg(sf::Vector2f(screenWidth, screenHeight));
    bg.setFillColor(sf::Color(139, 0, 0)); // Dark red
    window.draw(bg);

    sf::Text errorText;
    errorText.setFont(font);
    errorText.setCharacterSize(40);
    errorText.setString(message + "\n\nPress Escape or close the window to exit.");
    centerTextX(errorText, screenHeight / 2 - 50);
    window.draw(errorText);
}
