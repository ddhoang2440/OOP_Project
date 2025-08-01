#include "App.h"

App::App()
    : window(sf::VideoMode::getDesktopMode(), "Space Chicken", sf::Style::Fullscreen),
    ui(), // Changed to call parameterless UIManager constructor
    menu(static_cast<float>(sf::VideoMode::getDesktopMode().width), static_cast<float>(sf::VideoMode::getDesktopMode().height)),
    pauseMenu(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)),
    settingsMenu(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)),
    gameState(GameState::MENU),
    score(0), lives(3),
    waitingForEnterEffect(false), enterEffectTime(0.f)
{
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    cursor.loadFromFile("assets/cursor/crosshair.png");

    // Removed setActiveMenu(&mainMenu); as we are not using currentMenu pointer anymore
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
            // Let mainMenu handle its own events
             menu.handleEvent(event, window);

            if (!waitingForEnterEffect) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    menu.activate();
                    waitingForEnterEffect = true;
                    enterEffectTime = 0.f;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    int clicked = menu.handleMouseClick(mousePos);
                    if (clicked != -1) {
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
            // Let pauseMenu handle its own events
            pauseMenu.handleEvent(event, window);

            if (pauseMenu.isVisible()) {
                // No need for mousePos here, handled by pauseMenu.handleEvent
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    pauseMenu.hide();
                    gameState = GameState::PLAYING;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    int clicked = pauseMenu.handleMouseClick(mousePos);
                    if (clicked == 0) { // Resume
                        pauseMenu.hide();
                        gameState = GameState::PLAYING;
                    }
                    else if (clicked == 1) { // Main Menu
                        pauseMenu.hide();
                        gameState = GameState::MENU;
                    }
                    else if (clicked == 2) { // Quit
                        window.close();
                    }
                }
            }
            break;

        case GameState::SETTINGS:
            // Let settingsMenu handle its own events
            settingsMenu.handleEvent(event, window);

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                settingsMenu.hide();
                gameState = GameState::MENU; // Go back to main menu
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                int clicked = settingsMenu.handleMouseClick(mousePos);
                if (clicked == static_cast<int>(settingsMenu.getOptionCount() - 1)) { // "Back to Main Menu" is the last option
                    settingsMenu.hide();
                    gameState = GameState::MENU;
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
        menu.update(dt); // Directly call mainMenu's update
        if (waitingForEnterEffect) {
            enterEffectTime += dt;
            if (enterEffectTime >= MENU_ENTER_EFFECT_DURATION) {
                switch (menu.getSelectedIndex()) {
                case 0: // New Game
                    gameState = GameState::PLAYING;
                    score = 0;
                    lives = 3;
                    level.resetLevel();
                    break;
                case 1: // Quit
                    window.close();
                    break;
                case 2: // High Scores
                    gameState = GameState::HIGH_SCORES;
                    // TODO: Show HighScoresMenu
                    break;
                case 3: // Settings
                    gameState = GameState::SETTINGS;
                    settingsMenu.show(); // Show settings menu
                    break;
                case 4: // Help
                    gameState = GameState::HELP;
                    // TODO: Show HelpMenu
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
            // Present spawning logic
            //if (presentSpawnClock.getElapsedTime().asSeconds() >= PRESENT_SPAWN_INTERVAL) {
            //    Present newPresent;
            //    // Set initial position (random X at top of screen)
            //    newPresent.anim_.sprite_.setPosition(static_cast<float>(rand() % static_cast<int>(window.getSize().x)), 0.f);
            //    presents.push_back(newPresent);
            //    presentSpawnClock.restart();
            //}

            //// Update and clean up presents
            //for (auto it = presents.begin(); it != presents.end(); ) {
            //    it->update(dt); // Pass window to present's update
            //    if (!it->get_is_on_screen()) {
            //        it = presents.erase(it); // Remove if off-screen
            //    }
            //    else {
            //        ++it;
            //    }
            //}
            break;

    case GameState::PAUSED:
        // No need to update game logic (level, UI) when paused
        // The pause menu itself might have minimal updates, but not game logic
        pauseMenu.update(dt); // Call pauseMenu's update
        break;

    case GameState::HIGH_SCORES:
        // TODO: update high scores screen logic
        // highScoresMenu.update(dt); // Assuming a highScoresMenu object
        break;

    case GameState::SETTINGS:
        settingsMenu.update(dt); // Call settingsMenu's update
        // Apply settings changes to background music
        backgroundMusic.setVolume(settingsMenu.getMasterVolume());
        if (settingsMenu.getSoundEffectsEnabled()) {
            if (backgroundMusic.getStatus() != sf::Music::Playing) {
                backgroundMusic.play();
            }
        }
        else {
            backgroundMusic.stop();
        }
        break;
    case GameState::HELP:
        // TODO: update help screen logic
        // helpMenu.update(dt); // Assuming a helpMenu object
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
        menu.render(window); // Directly call mainMenu's render
        break;

    case GameState::PLAYING:
        // TODO: Render game elements (player, enemies, bullets, etc.)
        ui.render(window);
        break;

    case GameState::PAUSED:
        // Render game elements as they were before pausing (optional, but good for context)
        // TODO: Render game elements here if you want them visible behind the pause menu
        ui.render(window);
        pauseMenu.render(window); // Directly call pauseMenu's render
        break;

    case GameState::HIGH_SCORES:
        // TODO: Render high scores screen
        // highScoresMenu.render(window); // Assuming a highScoresMenu object
        break;

    case GameState::SETTINGS:
        settingsMenu.render(window); // Directly call settingsMenu's render
        break;

    case GameState::HELP:
        // TODO: Render help screen
        // helpMenu.render(window); // Assuming a helpMenu object
        break;

    case GameState::GAMEOVER:
        // TODO: Render game over screen
        break;
    }

    cursor.draw(window);
    window.display();
}

// Removed setActiveMenu function
// void App::setActiveMenu(IMenu* menu) {
//     if (currentMenu) {
//         currentMenu->hide();
//     }
//     currentMenu.reset(menu);
//     if (currentMenu) {
//         currentMenu->show();
//     }
// }
