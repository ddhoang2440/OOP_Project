#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "UIManager.h"
#include "LevelManager.h" 
#include "GameState.h"
#include "Menu.h"
#include "CustomCursor.h"
#include "PauseMenu.h"
#include "SettingMenu.h"
#include "IMenu.h"
// Constants for menu effects
const float MENU_ENTER_EFFECT_DURATION = 0.4f;
const float PRESENT_SPAWN_INTERVAL = 5.0f; // Spawn a present every 5 seconds



class App {
public:
    App();
    void run();

private:
    sf::RenderWindow window;
    sf::Music backgroundMusic;
    UIManager ui;
    LevelManager level;
    std::unique_ptr<IMenu> currentMenu;
    Menu menu;
    PauseMenu pauseMenu;
    SettingsMenu settingsMenu;
    CustomCursor cursor;

    GameState gameState;
    int score, lives;
    bool waitingForEnterEffect;
    float enterEffectTime;
    
    sf::Clock presentSpawnClock;

    void processEvents();
    void update(float dt);
    void render();
};
