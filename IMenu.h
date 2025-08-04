#pragma once
#include <SFML/Graphics.hpp>

class IMenu {
public:
    virtual ~IMenu() = default;

    virtual void handleEvent(sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool isVisible() const = 0;

    virtual int getSelectedIndex() const = 0;
};
