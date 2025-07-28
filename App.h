#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "UIManager.h"
#include "LevelManager.h" 
#include "GameState.h"
#include "Menu.h"
#include "CustomCursor.h"
#include "PauseMenu.h"

// Constants for menu effects
const float MENU_ENTER_EFFECT_DURATION = 0.4f;

class App {
public:
    App();
    void run();

private:
    sf::RenderWindow window;
    sf::Music backgroundMusic;
    UIManager ui;
    LevelManager level;
    Menu menu;
    PauseMenu pauseMenu;
    CustomCursor cursor;

    GameState gameState;
    int score, lives;
    bool waitingForEnterEffect;
    float enterEffectTime;

    void processEvents();
    void update(float dt);
    void render();
};
