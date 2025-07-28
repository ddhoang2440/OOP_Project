#include "App.h"
#include <iostream>

App::App()
    : window(sf::VideoMode::getDesktopMode(), "Space Chicken", sf::Style::Fullscreen),
    menu(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
    gameState(GameState::MENU),
    score(0), lives(3),
    waitingForEnterEffect(false), enterEffectTime(0.f)
{
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    cursor.loadFromFile("assets/cursor/crosshair.png");
}

void App::run() {
    if (!backgroundMusic.openFromFile("assets/sound/background.ogg")) {
        std::cerr << "Failed to load background music!\n";
    }
    else {
        backgroundMusic.setLoop(true);       // Lặp lại liên tục
        backgroundMusic.setVolume(50.f);     // Âm lượng từ 0–100
        backgroundMusic.play();
    }

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void App::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (gameState == GameState::MENU && !waitingForEnterEffect) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) menu.moveUp();
                else if (event.key.code == sf::Keyboard::Down) menu.moveDown();
                else if (event.key.code == sf::Keyboard::Enter) {
                    menu.activate();
                    waitingForEnterEffect = true;
                    enterEffectTime = 0.f;
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                menu.handleMouseMove(mousePos);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                int clicked = menu.handleMouseClick(mousePos);
                if (clicked == 0 || clicked == 1) {
                    menu.activate();
                    waitingForEnterEffect = true;
                    enterEffectTime = 0.f;
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }
    }
}

void App::update(float dt) {
    cursor.update(window);

    if (gameState == GameState::MENU) {
        menu.update(dt);
        if (waitingForEnterEffect) {
            enterEffectTime += dt;
            if (enterEffectTime >= 0.4f) {
                if (menu.getSelectedIndex() == 0) gameState = GameState::PLAYING;
                else if (menu.getSelectedIndex() == 1) window.close();
                waitingForEnterEffect = false;
            }
        }
    }
    else if (gameState == GameState::PLAYING) {
        level.update(dt);
        ui.update(score, lives, level.getLevel());
    }
}

void App::render() {
    window.clear();

    if (gameState == GameState::MENU) {
        menu.render(window);
    }
    else if (gameState == GameState::PLAYING) {
        ui.render(window);
    }

    cursor.draw(window); // vẽ con trỏ sau cùng
    window.display();
}