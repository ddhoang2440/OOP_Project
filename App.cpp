#include "App.h"
#include <iostream>

// Constants for menu effects

App::App()
    : window(sf::VideoMode::getDesktopMode(), "Space Chicken", sf::Style::Fullscreen),
    menu(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
    pauseMenu(window.getSize().x, window.getSize().y),
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
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50.f);
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

        switch (gameState) {
        case GameState::MENU:
            if (!waitingForEnterEffect) {
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
                    if (clicked == 0 || clicked == 1 || clicked == 2 || clicked == 3 || clicked == 4) {
                        menu.activate();
                        waitingForEnterEffect = true;
                        enterEffectTime = 0.f;
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            break;

        case GameState::PAUSED:
            if (pauseMenu.isVisible()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    pauseMenu.hide();
                    gameState = GameState::PLAYING;
                }

                if (event.type == sf::Event::MouseMoved) {
                    pauseMenu.handleMouseMove(mousePos);
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int clicked = pauseMenu.handleMouseClick(mousePos);
                    if (clicked == 0) {
                        pauseMenu.hide();
                        gameState = GameState::PLAYING;
                    }
                    else if (clicked == 1) {
                        pauseMenu.hide();
                        gameState = GameState::MENU;
                    }
                    else if (clicked == 2) {
                        window.close();
                    }
                }
            }
            break;

        case GameState::PLAYING:
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                gameState = GameState::PAUSED;
                pauseMenu.show();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (ui.getPauseButton().getGlobalBounds().contains(mousePos)) {
                    gameState = GameState::PAUSED;
                    pauseMenu.show();
                }
            }
            // TODO: Add gameplay specific event handling here (e.g., player movement, shooting)
            break;

        case GameState::GAMEOVER:
        case GameState::HIGH_SCORES:
        case GameState::SETTINGS:
        case GameState::HELP:
            // TODO: Add event handling for these states (e.g., return to menu)
            break;
        }
    }
}

void App::update(float dt) {
    cursor.update(window);

    switch (gameState) {
    case GameState::MENU:
        menu.update(dt);
        if (waitingForEnterEffect) {
            enterEffectTime += dt;
            if (enterEffectTime >= MENU_ENTER_EFFECT_DURATION) {
                switch (menu.getSelectedIndex()) {
                case 0:
                    gameState = GameState::PLAYING;
                    score = 0;
                    lives = 3;
                    level.resetLevel();
                    break;
                case 1:
                    window.close();
                    break;
                case 2:
                    gameState = GameState::HIGH_SCORES;
                    break;
                case 3:
                    gameState = GameState::SETTINGS;
                    break;
                case 4:
                    gameState = GameState::HELP;
                    break;
                }
                waitingForEnterEffect = false;
            }
        }
        break;

    case GameState::PLAYING:
        level.update(dt);
        ui.update(score, lives, level.getLevel());
        // TODO: Update score and lives based on gameplay events
        break;

    case GameState::PAUSED:
        // No need to update game logic (level, UI) when paused
        ui.render(window);
        pauseMenu.render(window);
        break;

    case GameState::HIGH_SCORES:
        // TODO: update high scores screen logic
        break;

    case GameState::SETTINGS:
        // TODO: update settings screen logic
        break;

    case GameState::HELP:
        // TODO: update help screen logic
        break;

    case GameState::GAMEOVER:
        // TODO: update game over screen logic
        break;
    }
}

void App::render() {
    window.clear();

    switch (gameState) {
    case GameState::MENU:
        menu.render(window);
        break;

    case GameState::PLAYING:
        // TODO: Render game elements (player, enemies, bullets, etc.)
        // For now, only UI is rendered
        ui.render(window);
        break;

    case GameState::PAUSED:
        // TODO: Render game elements here if you want them visible behind the pause menu
        ui.render(window);
        pauseMenu.render(window);
        break;

    case GameState::HIGH_SCORES:
        // TODO: Render high scores screen
        break;

    case GameState::SETTINGS:
        // TODO: Render settings screen
        break;

    case GameState::HELP:
        // TODO: Render help screen
        break;

    case GameState::GAMEOVER:
        // TODO: Render game over screen
        break;
    }

    cursor.draw(window);
    window.display();
}
