#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "IMenu.h"

class SettingsMenu : public IMenu {
public:
    SettingsMenu(float width, float height);

    sf::Texture backgroundTexture;
    sf::Sprite background1;
    // Implement IMenu pure virtual functions
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    void show() override;
    void hide() override;
    bool isVisible() const override;
    int getSelectedIndex() const override; // For navigation/selection within settings
    // Helper methods for mouse interaction
    void handleMouseMove(sf::Vector2f mousePos);
    int handleMouseClick(sf::Vector2f mousePos);
    size_t getOptionCount() const;
    bool getSoundEffectsEnabled() const;
    float getMasterVolume() const;
private:
    sf::Font font;
    std::vector<sf::Text> options;
    std::vector<sf::RectangleShape> optionBoxes;
    int selectedIndex;
    bool visible;

    // Example settings variables (you will expand these)
    float masterVolume;
    bool soundEffectsEnabled;
};
