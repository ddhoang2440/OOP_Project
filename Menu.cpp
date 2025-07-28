#include "Menu.h"

// Constructor for the Menu class.
// Initializes menu elements, loads assets, and sets up positions/scales.
Menu::Menu(float width, float height) {
    font.loadFromFile("assets/fonts/Pixel Sans Serif Condensed.ttf");

    if (!titleTexture.loadFromFile("assets/images/title.png")) {
        std::cerr << "Failed to load title image!" << std::endl;
    }
    else {
        titleSprite.setTexture(titleTexture);
        sf::FloatRect bounds = titleSprite.getLocalBounds();
        titleSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        titleSprite.setPosition(width / 2.f, 300.f);
        titleSprite.setScale(0.8f, 0.7f);
    }

    if (!backgroundTexture.loadFromFile("assets/images/background.png")) {
        std::cerr << "Failed to load background image!\n";
    }
    background1.setTexture(backgroundTexture);
    background2.setTexture(backgroundTexture);
    background1.setPosition(0, 0);
    background2.setPosition(0.f, -static_cast<float>(backgroundTexture.getSize().y));

    float scaleX = width / backgroundTexture.getSize().x;
    float scaleY = height / backgroundTexture.getSize().y;
    background1.setScale(scaleX, scaleY);
    background2.setScale(scaleX, scaleY);

    if (!spaceshipTexture.loadFromFile("assets/images/spaceship.png")) {
        std::cerr << "Failed to load spaceship image!\n";
    }
    else {
        spaceshipSprite.setTexture(spaceshipTexture);
        spaceshipSprite.setScale(0.4f, 0.4f);
    }

    orbitCenter = sf::Vector2f(width / 2.f, height / 2.f + 100.f);

    std::vector<std::string> labels = { "New Game", "Quit", "High Scores", "Settings", "Help" };
    selectedIndex = 0;
    int centeredCount = 0;

    const float paddingX = 60.f;
    const float paddingY = 30.f;
    const float verticalSpacing = 100.f;

    float maxTextWidth = 0.f;
    float maxTextHeight = 0.f;

    for (const auto& label : labels) {
        if (label == "Quit") continue;
        sf::Text temp;
        temp.setFont(font);
        temp.setString(label);
        temp.setCharacterSize(40);
        sf::FloatRect bounds = temp.getLocalBounds();
        if (bounds.width > maxTextWidth) maxTextWidth = bounds.width;
        if (bounds.height > maxTextHeight) maxTextHeight = bounds.height;
    }

    for (size_t i = 0; i < labels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(40);

        sf::RectangleShape box;

        if (labels[i] == "Quit") {
            text.setOrigin(0.f, 0.f);
            text.setPosition(30.f, height - 70.f - 10.f);

            sf::FloatRect bounds = text.getGlobalBounds();
            box.setSize(sf::Vector2f(bounds.width + 30.f, bounds.height + 20.f));
            box.setOrigin(0.f, 0.f);
            box.setPosition(text.getPosition().x - 10.f, text.getPosition().y - 10.f + 10.f);
        }
        else {
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            text.setPosition(width / 2.f, height / 2.f + centeredCount * verticalSpacing - 10.f);
            centeredCount++;

            float boxW = maxTextWidth + paddingX;
            float boxH = maxTextHeight + paddingY;
            box.setSize(sf::Vector2f(boxW, boxH));
            box.setOrigin(boxW / 2.f, boxH / 2.f);
            box.setPosition(text.getPosition().x, text.getPosition().y + 10.f);
        }

        box.setFillColor(sf::Color(0, 0, 0, 120));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::White);

        text.setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);

        options.push_back(text);
        optionBoxes.push_back(box);
    }
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        options[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}

void Menu::moveDown() {
    if (selectedIndex < options.size() - 1) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        options[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}

void Menu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) moveUp();
        else if (event.key.code == sf::Keyboard::Down) moveDown();
    }
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        handleMouseMove(mousePos);
    }
}

void Menu::update(float dt) {
    background1.move(0, backgroundScrollSpeed * dt);
    background2.move(0, backgroundScrollSpeed * dt);

    float bgHeight = backgroundTexture.getSize().y * background1.getScale().y;
    if (background1.getPosition().y >= bgHeight)
        background1.setPosition(0, background2.getPosition().y - bgHeight);
    if (background2.getPosition().y >= bgHeight)
        background2.setPosition(0, background1.getPosition().y - bgHeight);

    spaceshipTimer -= 0.5f * dt;
    float angle = spaceshipTimer;
    float x = orbitCenter.x + orbitRadius * std::cos(angle);
    float y = orbitCenter.y + orbitRadius * std::sin(angle);
    spaceshipSprite.setPosition(x, y);
    spaceshipSprite.setRotation(angle * 180.f / 3.14159f);

    for (size_t i = 0; i < options.size(); ++i) {
        float currentScale = options[i].getScale().x;
        float targetScale = (i == selectedIndex) ? 1.2f : 1.0f;

        if (isActivating && i == selectedIndex) {
            activateTimer += dt;
            float scale = 1.0f + 0.2f * std::cos(10 * activateTimer);
            options[i].setScale(scale, scale);
            if (activateTimer >= 0.4f) {
                isActivating = false;
                options[i].setScale(1.2f, 1.2f);
            }
        }
        else {
            float smoothScale = currentScale + (targetScale - currentScale) * 0.1f;
            options[i].setScale(smoothScale, smoothScale);
        }

        options[i].setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);

        if (options[i].getOrigin().x == 0.f) {
            optionBoxes[i].setScale(options[i].getScale());
            optionBoxes[i].setPosition(options[i].getPosition().x - 10.f, options[i].getPosition().y - 10.f + 10.f);
        }
        else {
            optionBoxes[i].setPosition(options[i].getPosition());
            optionBoxes[i].setScale(options[i].getScale());
        }
    }
}

void Menu::render(sf::RenderWindow& window) {  
    window.draw(background1);  
    window.draw(background2);  
    window.draw(titleSprite);  
    window.draw(spaceshipSprite);  

    for (const auto& box : optionBoxes)  
        window.draw(box);  
    for (const auto& option : options)  
        window.draw(option);  
}

void Menu::show() {
}

void Menu::hide() {
}

bool Menu::isVisible() const {
    return true;
}

int Menu::getSelectedIndex() const {
    return selectedIndex;
}

void Menu::activate() {
    isActivating = true;
    activateTimer = 0.f;
}

void Menu::handleMouseMove(sf::Vector2f mousePos) {
    for (size_t i = 0; i < options.size(); ++i) {
        if (options[i].getGlobalBounds().contains(mousePos)) {
            selectedIndex = i;
        }
    }
}

int Menu::handleMouseClick(sf::Vector2f mousePos) {
    for (size_t i = 0; i < options.size(); ++i) {
        if (options[i].getGlobalBounds().contains(mousePos)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
