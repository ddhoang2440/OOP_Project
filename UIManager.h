#pragma once
#include <SFML/Graphics.hpp>
#include <string> // Include for std::string if not already included by SFML/Graphics

class UIManager {
private:
    sf::Font font;

    // Pause button
    sf::RectangleShape pauseButton;
    sf::Text pauseText;

    // Game Stats Text
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text levelText;

public:
    UIManager();

    void render(sf::RenderWindow& window);

    // Get the pause button for collision detection
    const sf::RectangleShape& getPauseButton() const;

    // Update method to set score, lives, and level text
    void update(int score, int lives, int level);
};
