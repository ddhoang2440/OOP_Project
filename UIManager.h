#pragma once
#include <SFML/Graphics.hpp>

class UIManager {
private:
    sf::Font font;
    sf::Text scoreText, lifeText, levelText;

public:
    UIManager();
    void update(int score, int lives, int level);
    void render(sf::RenderWindow& window);
};