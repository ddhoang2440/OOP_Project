#pragma once
#include <SFML/Graphics.hpp>

class CustomCursor {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    CustomCursor();
    bool loadFromFile(const std::string& path);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
}; 