#include "CustomCursor.h"
#include <iostream>

CustomCursor::CustomCursor() {
    sprite.setScale(0.1f, 0.1f); 
}

bool CustomCursor::loadFromFile(const std::string& path) {
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load cursor: " << path << std::endl;
        return false;
    }
    sprite.setTexture(texture);

    sprite.setScale(0.08f, 0.08f); // Giảm kích thước còn 8%
    sprite.setColor(sf::Color(0, 255, 255)); // Xanh cyan
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    return true;
}

void CustomCursor::update(const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sprite.setPosition(mousePos);
}

void CustomCursor::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}