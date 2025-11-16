#pragma once
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "CombatManager.h"
#include <vector>

class GameField {
private:
    int width, height;
    int playerX, playerY;
    std::vector<std::vector<Cell>> grid;
    std::vector<std::pair<int, int>> enemyPositions;
    std::vector<Enemy> enemies;
    std::vector<std::pair<int, int>> buildingPositions;
    std::vector<EnemyBuilding> buildings;
    CombatManager combatManager;  

public:
    //Конструкторы
    GameField(int w, int h);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    // Методы для работы с игроком
    bool setPlayerPosition(int x, int y, Player& player);
    std::pair<int, int> getPlayerPosition() const;

    // Методы для проверки клеток
    bool isCellPassable(int x, int y) const;
    bool hasPlayer(int x, int y) const;
    bool hasEnemy(int x, int y) const;
    bool hasBuilding(int x, int y) const;
    CellType getCellType(int x, int y) const;

    // Методы для работы с врагами (делегируются CombatManager)
    void moveEnemies(Player& player) { combatManager.moveEnemies(player); }
    void removeDeadEnemies() { combatManager.removeDeadEnemies(); }
    
    // Метод для спавна врагов из зданий
    void spawnEnemiesFromBuildings();

    // Геттеры
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    size_t getEnemyCount() const { return enemies.size(); }
    std::vector<Enemy>& getEnemies() { return enemies; }
    const std::vector<std::pair<int, int>>& getEnemyPositions() const { return enemyPositions; }

private:
    void initializeGrid();
    void addRandomEnemies(int count);
    void addRandomBuildings(int count);
    
    // Вспомогательный метод для атаки
    bool attackEnemy(int x, int y, Player& player) { 
        return combatManager.attackEnemy(x, y, player); 
    }
};