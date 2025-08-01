#include "SettingMenu.h"
#include <iostream>

SettingsMenu::SettingsMenu(float width, float height)
    : selectedIndex(0), visible(false), masterVolume(50.f), soundEffectsEnabled(true)
{
    if (!font.loadFromFile("assets/fonts/Pixel Sans Serif Condensed.ttf")) {
        std::cerr << "SettingsMenu: Failed to load font!" << std::endl;
    }

    if (!backgroundTexture.loadFromFile("assets/images/backgroundsetting.png")) {
        std::cerr << "Failed to load background image!\n";
    }
    background1.setTexture(backgroundTexture);
    background1.setPosition(0, 0);

    float scaleX = width / backgroundTexture.getSize().x;
    float scaleY = height / backgroundTexture.getSize().y;
    background1.setScale(scaleX, scaleY);


    sf::RectangleShape backgroundOverlay;
    backgroundOverlay.setSize(sf::Vector2f(width, height));
    backgroundOverlay.setFillColor(sf::Color(0, 0, 0, 180));

    std::vector<std::string> labels = {
        "Settings",
        "Volume: " + std::to_string(static_cast<int>(masterVolume)),
        "Sound Effects: " + std::string(soundEffectsEnabled ? "On" : "Off"),
        "Back to Main Menu"
    };

    const float boxWidth = 400.f;
    const float boxHeight = 60.f;
    const float spacing = 80.f;
    float startY = height / 2.f - (labels.size() / 2.0f) * spacing;

    for (size_t i = 0; i < labels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(i == 0 ? 40 : 30);
        text.setFillColor(sf::Color::White);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        text.setPosition(width / 2.f, startY + i * spacing);

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

void SettingsMenu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (!visible) return;

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseMoved) {
        handleMouseMove(mousePos);
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int clicked = handleMouseClick(mousePos);
        if (clicked == 1) {
            masterVolume += 10.f;
            if (masterVolume > 100.f) masterVolume = 0.f;
            options[1].setString("Volume: " + std::to_string(static_cast<int>(masterVolume)));
        }
        else if (clicked == 2) {
            soundEffectsEnabled = !soundEffectsEnabled;
            options[2].setString("Sound Effects: " + std::string(soundEffectsEnabled ? "On" : "Off"));
        }
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedIndex = (selectedIndex + 1) % options.size();
        }
        if (event.key.code == sf::Keyboard::Enter) {
            int selectedOption = selectedIndex;
            if (selectedOption == 1) {
                masterVolume += 10.f;
                if (masterVolume > 100.f) masterVolume = 0.f;
                options[1].setString("Volume: " + std::to_string(static_cast<int>(masterVolume)));
            }
            else if (selectedOption == 2) {
                soundEffectsEnabled = !soundEffectsEnabled;
                options[2].setString("Sound Effects: " + std::string(soundEffectsEnabled ? "On" : "Off"));
            }
        }
    }
}

void SettingsMenu::update(float dt) {
    for (size_t i = 0; i < options.size(); ++i) {
        options[i].setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
    }
}

void SettingsMenu::render(sf::RenderWindow& window) {
    if (!visible) return;    
    window.draw(background1);

    sf::RectangleShape currentBackgroundOverlay;
    currentBackgroundOverlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    currentBackgroundOverlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(currentBackgroundOverlay);

    for (auto& box : optionBoxes)
        window.draw(box);
    for (auto& option : options)
        window.draw(option);
}

void SettingsMenu::show() { visible = true; selectedIndex = 0; }
void SettingsMenu::hide() { visible = false; }
bool SettingsMenu::isVisible() const { return visible; }

int SettingsMenu::getSelectedIndex() const {
    return selectedIndex;
}

void SettingsMenu::handleMouseMove(sf::Vector2f mousePos) {
    for (size_t i = 0; i < options.size(); ++i) {
        if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
            selectedIndex = i;
        }
    }
}

int SettingsMenu::handleMouseClick(sf::Vector2f mousePos) {
    for (size_t i = 0; i < optionBoxes.size(); ++i) {
        if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

size_t SettingsMenu::getOptionCount() const {
	return options.size();
}

bool SettingsMenu::getSoundEffectsEnabled() const {
    return soundEffectsEnabled;
}

float SettingsMenu::getMasterVolume() const {
    return masterVolume;
}
