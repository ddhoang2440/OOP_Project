#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "UIManager.h"
#include "LevelManager.h"
#include "GameState.h"
#include "Menu.h"
#include "CustomCursor.h"

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
    CustomCursor cursor;

    GameState gameState;
    int score, lives;
    bool waitingForEnterEffect;
    float enterEffectTime;

    void processEvents();
    void update(float dt);
    void render();
};
