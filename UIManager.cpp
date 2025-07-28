#include "UIManager.h"
#include <iostream>
#include "Menu.h"
UIManager::UIManager() {
    if (!font.loadFromFile("assets/fonts/Pixel Sans Serif Condensed.ttf")) {
        std::cerr << "Font load failed!" << std::endl;
    }
    scoreText.setFont(font);
    lifeText.setFont(font);
    levelText.setFont(font);

    scoreText.setCharacterSize(20);
    lifeText.setCharacterSize(20);
    levelText.setCharacterSize(20);

    // Đặt màu chữ (ví dụ: màu đen)
    scoreText.setFillColor(sf::Color::White);
    lifeText.setFillColor(sf::Color::White);
    levelText.setFillColor(sf::Color::White);

    scoreText.setPosition(10, 10);
    lifeText.setPosition(10, 40);
    levelText.setPosition(10, 70);
}

void UIManager::update(int score, int lives, int level) {
    scoreText.setString("Score: " + std::to_string(score));
    lifeText.setString("Lives: " + std::to_string(lives));
    levelText.setString("Level: " + std::to_string(level));
}

void UIManager::render(sf::RenderWindow& window) {
    window.draw(scoreText);
    window.draw(lifeText);
    window.draw(levelText);
}
