#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Menu {
private:
    float backgroundScrollSpeed = 20.f;

    // Background và tiêu đề
    sf::Texture titleTexture;
    sf::Sprite titleSprite;
    sf::Texture backgroundTexture;
    sf::Sprite background1;
    sf::Sprite background2;

    // Tàu vũ trụ
    sf::Texture spaceshipTexture;
    sf::Sprite spaceshipSprite;
    float spaceshipTimer = 0.f;
    float orbitRadius = 580.f;
    sf::Vector2f orbitCenter;

    // Menu
    sf::Font font;
    std::vector<sf::Text> options;
    std::vector<sf::RectangleShape> optionBoxes;
    int selectedIndex = 0;
    bool isActivating = false;
    float activateTimer = 0.f;

public:
    Menu(float width, float height);
    void moveUp();
    void moveDown();
    void update(float dt);
    void render(sf::RenderWindow& window);
    int getSelectedIndex() const;
    void activate();
    void handleMouseMove(sf::Vector2f mousePos);
    int handleMouseClick(sf::Vector2f mousePos);
};