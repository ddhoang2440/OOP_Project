#pragma once

class LevelManager {
private:
    int currentLevel;
    float spawnTimer;
    float spawnInterval;

public:
    LevelManager();

    void update(float deltaTime);
    int getLevel() const;

    // TODO: thêm hàm để spawn enemy
};