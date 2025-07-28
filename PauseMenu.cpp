#include "PauseMenu.h"
#include <iostream>

PauseMenu::PauseMenu(float width, float height)
    : selectedIndex(0), visible(false)
{
    if (!font.loadFromFile("assets/fonts/Pixel Sans Serif Condensed.ttf")) {
        std::cerr << "PauseMenu: Failed to load font!" << std::endl;
    }

    // Background overlay
    backgroundOverlay.setSize(sf::Vector2f(width, height));
    backgroundOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    std::vector<std::string> labels = { "Resume", "Main Menu", "Quit" };

    const float boxWidth = 300.f;
    const float boxHeight = 60.f;
    const float spacing = 80.f;

    for (size_t i = 0; i < labels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(30);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        text.setPosition(width / 2.f, height / 2.f + i * spacing);

        sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
        box.setOrigin(boxWidth / 2.f, boxHeight / 2.f);
        box.setPosition(text.getPosition());
        box.setFillColor(sf::Color(50, 50, 50, 200));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::White);

        options.push_back(text);
        optionBoxes.push_back(box);
    }
}

void PauseMenu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!visible) return; // Only process events if visible

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseMoved) {
        handleMouseMove(mousePos);
    }

    // Mouse click logic is handled in App::processEvents
    // Key press logic (e.g., Escape) is handled in App::processEvents
}

// Implement IMenu::update
void PauseMenu::update(float dt) {
    // Pause menu typically doesn't have complex animations,
    // but this function is required by the IMenu interface.
    // You could add hover effects or subtle animations here if desired.
}


void PauseMenu::render(sf::RenderWindow& window) {
    if (!visible) return;

    window.draw(backgroundOverlay);
    for (auto& box : optionBoxes)
        window.draw(box);
    for (auto& option : options)
        window.draw(option);
}

void PauseMenu::show() { 
    visible = true; 
}

void PauseMenu::hide() { 
    visible = false; 
}

bool PauseMenu::isVisible() const { 
    return visible; 
}

int PauseMenu::getSelectedIndex() const {
    return selectedIndex;
}

void PauseMenu::handleMouseMove(sf::Vector2f mousePos) {
    if (!visible) return;
    for (size_t i = 0; i < options.size(); ++i) {
        if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
            options[i].setFillColor(sf::Color::Yellow);
            selectedIndex = i;
        }
        else {
            options[i].setFillColor(sf::Color::White);
        }
    }
}

int PauseMenu::handleMouseClick(sf::Vector2f mousePos) {
    if (!visible) return -1;
    for (size_t i = 0; i < optionBoxes.size(); ++i) {
        if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
            return static_cast<int>(i); // 0: Resume, 1: Menu, 2: Quit
        }
    }
    return -1;
}
