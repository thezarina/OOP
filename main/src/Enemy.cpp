#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(int hp, int dmg) : health(hp), maxHealth(hp), damage(dmg) {}

int Enemy::getHealth() const { return health; }
int Enemy::getDamage() const { return damage; }
int Enemy::getMaxHealth() const { return maxHealth; }

void Enemy::takeDamage(int dmg) {
    health = std::max(0, health - dmg);
}

bool Enemy::isAlive() const {
    return health > 0;
}