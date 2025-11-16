#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

class EnemyBuilding {
private:
    int spawnCounter;
    int spawnInterval;

public:
    EnemyBuilding(int interval = 5);
    
    bool shouldSpawnEnemy();
    int getSpawnInterval() const;
    int getSpawnCounter() const;
};

#endif // ENEMY_BUILDING_H