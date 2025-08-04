#include "LevelManager.h"

LevelManager::LevelManager()
    : currentLevel(1), spawnTimer(0.f), spawnInterval(5.f) {
}

void LevelManager::update(float deltaTime) {
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.f;
        currentLevel++;

        // TODO: tăng độ khó, spawn enemy/boss
    }
}

int LevelManager::getLevel() const {
    return currentLevel;
}

int LevelManager::resetLevel() {
	this->currentLevel = 1;
	return currentLevel;
}