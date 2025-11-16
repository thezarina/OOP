#include "AreaDamageSpell.h"
#include "Field.h"
#include "Player.h"
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int areaSize,
                               const std::string& name, 
                               const std::string& description,
                               int manaCost)
    : Spell(name, description, manaCost), damage(damage), range(range), 
      areaSize(areaSize), enhanced(false) {}

bool AreaDamageSpell::cast(GameField& field, Player& player, int targetX, int targetY) {
    if (player.getMana() < manaCost) {
        return false;
    }
    
    auto playerPos = field.getPlayerPosition();
    int playerX = playerPos.first;
    int playerY = playerPos.second;
    
    // Проверяем радиус
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    if (distance > range) {
        return false;
    }
    
    int actualAreaSize = enhanced ? areaSize + 1 : areaSize;
    int actualDamage = enhanced ? damage + 5 : damage;
    
    bool hitSomething = false;
    
    // Наносим урон по области
    for (int dx = 0; dx < actualAreaSize; ++dx) {
        for (int dy = 0; dy < actualAreaSize; ++dy) {
            int currentX = targetX + dx;
            int currentY = targetY + dy;
            
            // Проверяем границы поля
            if (currentX >= 0 && currentX < field.getWidth() && 
                currentY >= 0 && currentY < field.getHeight()) {
                
                // Наносим урон врагам в этой клетке
                if (field.hasEnemy(currentX, currentY)) {
                    auto& enemies = field.getEnemies();
                    auto& enemyPositions = field.getEnemyPositions();
                    
                    for (size_t i = 0; i < enemyPositions.size(); ++i) {
                        if (enemyPositions[i].first == currentX && enemyPositions[i].second == currentY) {
                            enemies[i].takeDamage(actualDamage);
                            hitSomething = true;
                        }
                    }
                }
            }
        }
    }
    
    // Заклинание считается успешным, даже если никого не задело
    if (hitSomething) {
        player.setMana(player.getMana() - manaCost);
    }
    
    return true;
}

void AreaDamageSpell::enhance(int damageBonus, int areaBonus) {
    damage += damageBonus;
    areaSize += areaBonus;
    enhanced = true;
}