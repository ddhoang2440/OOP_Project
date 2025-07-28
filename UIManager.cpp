#include "UIManager.h"
#include <iostream>
#include <string> 

UIManager::UIManager(){
    // Load font for UI text
    if (!font.loadFromFile("assets/fonts/Pixel Sans Serif Condensed.ttf")) {
        std::cerr << "Error loading font for UIManager\n";
    }

    scoreText.setFont(font);
    livesText.setFont(font);
    levelText.setFont(font);

    scoreText.setCharacterSize(20);
    livesText.setCharacterSize(20);
    levelText.setCharacterSize(20);

    // Đặt màu chữ (ví dụ: màu đen)
    scoreText.setFillColor(sf::Color::White);
    livesText.setFillColor(sf::Color::White);
    levelText.setFillColor(sf::Color::White);

    scoreText.setPosition(10, 10);
    livesText.setPosition(10, 40);
    levelText.setPosition(10, 70);

    // Pause Button Setup
    pauseButton.setSize(sf::Vector2f(100.f, 40.f));
    pauseButton.setFillColor(sf::Color(50, 50, 50, 200)); // Dark grey, semi-transparent
    pauseButton.setOutlineThickness(2.f);
    pauseButton.setOutlineColor(sf::Color::White);

    // Position the pause button on the top-right corner using windowPtr
    // windowWidth - button_width - margin
    pauseButton.setPosition(10, 1000);

    pauseText.setFont(font);
    pauseText.setString("PAUSE");
    pauseText.setCharacterSize(20);
    pauseText.setFillColor(sf::Color::White);
    // Position text in the center of the button
    pauseText.setPosition(pauseButton.getPosition().x + 10, pauseButton.getPosition().y + 8);

}



void UIManager::render(sf::RenderWindow& window) {
    // Draw UI elements to the window
    window.draw(scoreText);
    window.draw(livesText);
    window.draw(levelText);
    window.draw(pauseButton);
    window.draw(pauseText);
}

const sf::RectangleShape& UIManager::getPauseButton() const {
    return pauseButton; // Return reference to pause button for collision detection
}

// Corrected update method for UIManager
void UIManager::update(int score, int lives, int level) {
    // Update the text strings with current game data
    scoreText.setString("Score: " + std::to_string(score));
    livesText.setString("Lives: " + std::to_string(lives));
    levelText.setString("Level: " + std::to_string(level));
}
