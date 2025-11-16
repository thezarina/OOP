#include "DirectDamageSpell.h"
#include "Field.h"
#include "Player.h"
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int damage, int range, 
                                   const std::string& name, 
                                   const std::string& description,
                                   int manaCost)
    : Spell(name, description, manaCost), damage(damage), range(range), enhanced(false) {}

bool DirectDamageSpell::cast(GameField& field, Player& player, int targetX, int targetY) {
    // Проверяем, достаточно ли маны у игрока
    if (player.getMana() < manaCost) {
        return false;
    }
    
    // Получаем позицию игрока
    auto playerPos = field.getPlayerPosition();
    int playerX = playerPos.first;
    int playerY = playerPos.second;
    
    // Проверяем, находится ли цель в радиусе действия
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    int actualRange = enhanced ? range + 2 : range;
    
    if (distance > actualRange) {
        return false;
    }
    
    // Применяем улучшение если есть
    int actualDamage = enhanced ? damage + 10 : damage;
    
    // Наносим урон врагу или зданию
    bool success = false;
    
    // Проверяем, есть ли враг на целевой клетке
    if (field.hasEnemy(targetX, targetY)) {
        // Находим врага и наносим урон
        auto& enemies = field.getEnemies();
        auto& enemyPositions = field.getEnemyPositions();
        
        for (size_t i = 0; i < enemyPositions.size(); ++i) {
            if (enemyPositions[i].first == targetX && enemyPositions[i].second == targetY) {
                enemies[i].takeDamage(actualDamage);
                success = true;
                break;
            }
        }
    }
    
    // Тратим ману только если заклинание было успешно применено
    if (success) {
        player.setMana(player.getMana() - manaCost);
    }
    
    return success;
}

void DirectDamageSpell::setDamage(int newDamage) {
    damage = newDamage;
}

void DirectDamageSpell::setRange(int newRange) {
    range = newRange;
}

void DirectDamageSpell::enhance(int damageBonus, int rangeBonus) {
    damage += damageBonus;
    range += rangeBonus;
    enhanced = true;
}