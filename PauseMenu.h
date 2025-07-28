#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "IMenu.h" // Include the IMenu interface

class PauseMenu : public IMenu { // Inherit from IMenu
public:
    PauseMenu(float width, float height);

    // Implement IMenu pure virtual functions
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override; // Pause menu typically doesn't update much, but required by interface
    void render(sf::RenderWindow& window) override;

    void show() override;
    void hide() override;
    bool isVisible() const override;

    int getSelectedIndex() const override; // Not typically used for PauseMenu, but required by interface

    // Public methods for App to interact with specific PauseMenu logic
    void handleMouseMove(sf::Vector2f mousePos);
    int handleMouseClick(sf::Vector2f mousePos);

private:
    sf::Font font;
    std::vector<sf::Text> options;
    std::vector<sf::RectangleShape> optionBoxes;

    sf::RectangleShape backgroundOverlay;

    int selectedIndex;
    bool visible;
};
