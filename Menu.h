#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "IMenu.h" // Include the IMenu interface

class Menu : public IMenu { // Inherit from IMenu
private:
    float backgroundScrollSpeed = 20.f;

    // Background and title
    sf::Texture titleTexture;
    sf::Sprite titleSprite;
    sf::Texture backgroundTexture;
    sf::Sprite background1;
    sf::Sprite background2;

    // Spaceship
    sf::Texture spaceshipTexture;
    sf::Sprite spaceshipSprite;
    float spaceshipTimer = 0.f;
    float orbitRadius = 580.f;
    sf::Vector2f orbitCenter;

    // Menu options
    sf::Font font;
    std::vector<sf::Text> options;
    std::vector<sf::RectangleShape> optionBoxes;
    int selectedIndex = 0;
    bool isActivating = false;
    float activateTimer = 0.f;

  

public:  
    void handleMouseMove(sf::Vector2f mousePos);
    int handleMouseClick(sf::Vector2f mousePos);
    Menu(float width, float height);
    void moveUp();
    void moveDown();

    // Implement IMenu pure virtual functions
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    void show() override; // Main menu is always "shown" implicitly
    void hide() override; // Main menu is "hidden" when not active
    bool isVisible() const override; // Check if main menu is the active state

    int getSelectedIndex() const override;
    void activate();
};
