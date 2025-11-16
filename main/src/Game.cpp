#include "Game.h"
#include "SpellCasting.h"
#include <iostream>
#include <cctype>


Game::Game(int width, int height) : field(width, height), player(), turnCount(0) {
    auto starterSpell = SpellCasting::getRandomStarterSpell();
    player.getSpellHand().addSpell(std::move(starterSpell));
}

void Game::display() const {
    std::cout << "Turn: " << turnCount << " | Health: " << player.getHealth() 
              << " | Score: " << player.getScore() 
              << " | Combat: " << (player.getCombatType() == CombatType::MELEE ? "Melee" : "Ranged")
              << " | Can move: " << (player.canMoveNextTurn() ? "Yes" : "No") << "\n\n";

    SpellCasting::displaySpellInfo(player);
    std::cout << "\n";
    
    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            if (field.hasPlayer(x, y)) {
                std::cout << "P ";
            } else if (field.hasEnemy(x, y)) {
                std::cout << "E ";
            } else if (field.hasBuilding(x, y)) {
                std::cout << "B ";
            } else {
                switch (field.getCellType(x, y)) {
                    case CellType::BLOCKED: std::cout << "# "; break;
                    case CellType::SLOW: std::cout << "~ "; break;
                    case CellType::EMPTY: std::cout << ". "; break;
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool Game::processPlayerMove(char direction) {
    if (!player.canMoveNextTurn()) {
        std::cout << "Player cannot move this turn!\n";

        // только смена урона
        if (direction == 'f'|| direction == 'F') {
            player.switchCombatType();
            std::cout << "Switched to " 
                    << (player.getCombatType() == CombatType::MELEE ? "melee" : "ranged") 
                    << " combat\n";
            player.resetMovement();
            return true;
        } else if (direction == 'c' || direction == 'C') {
            // Применение заклинания в режиме без движения
            if (!SpellCasting::hasSpells(player)) {
                std::cout << "No spells available!\n";
                return true;
            }
            
            std::cout << "Available spells:\n";
            SpellCasting::displaySpellInfo(player);
            
            int spellIndex;
            std::cout << "Select spell by number: ";
            std::cin >> spellIndex;
            
            if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
                std::cout << "Invalid spell selection!\n";
                return true;
            }
            
            int targetX, targetY;
            std::cout << "Enter target coordinates (x y): ";
            std::cin >> targetX >> targetY;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid coordinates!\n";
                return true;
            }
            
            bool success = SpellCasting::castSpell(player, spellIndex, field, targetX, targetY);
            if (success) {
                std::cout << "Spell cast successfully!\n";
                player.resetMovement(); // Восстанавливаем возможность движения после применения заклинания
            } else {
                std::cout << "Spell failed!\n";
            }
            return true;

        }else if (direction == 'w' || direction == 'W' || direction == 's' || direction == 'S' || 
                direction == 'a' || direction == 'A' || direction == 'd' || direction == 'D') {
            std::cout << "Movement blocked by slow effect!\n";
            player.resetMovement();
            return true;
        } else {
            std::cout << "Invalid command!\n";
            return true;
        }
    }

    
    std::pair<int, int> playerPos = field.getPlayerPosition();
    int x = playerPos.first;
    int y = playerPos.second;
    int newX = x, newY = y;
    
    switch (direction) {
        case 'w': case 'W': newY--; break;
        case 's': case 'S': newY++; break;
        case 'a': case 'A': newX--; break;
        case 'd': case 'D': newX++; break;
        case 'f': case 'F': 
            player.switchCombatType();
            std::cout << "Switched to " 
                      << (player.getCombatType() == CombatType::MELEE ? "melee" : "ranged") 
                      << " combat\n";
            return true;

        case 'c': case 'C': {
            // Применение заклинания
            if (!SpellCasting::hasSpells(player)) {
                std::cout << "No spells available!\n";
                return true;
            }
            
            std::cout << "Available spells:\n";
            SpellCasting::displaySpellInfo(player);
            
            int spellIndex;
            std::cout << "Select spell by number: ";
            std::cin >> spellIndex;
            
            if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
                std::cout << "Invalid spell selection!\n";
                return true;
            }
            
            int targetX, targetY;
            std::cout << "Enter target coordinates (x y): ";
            std::cin >> targetX >> targetY;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid coordinates!\n";
                return true;
            }
            
            bool success = SpellCasting::castSpell(player, spellIndex, field, targetX, targetY);
            if (success) {
                std::cout << "Spell cast successfully!\n";
                player.setMovement(false); // Тратим ход на применение заклинания
            } else {
                std::cout << "Spell failed!\n";
            }
            return true;
        }

        default:
            std::cout << "Invalid move!\n";
            return true;
    }
    

    
    if (field.setPlayerPosition(newX, newY, player)) {
        // Проверяем тип клетки
        if (field.getCellType(newX, newY) == CellType::SLOW) {
            player.setMovement(false);
            std::cout << "Player stepped on slow cell and cannot move next turn!\n";
        }
        return true;
    } else {
        std::cout << "Cannot move there!\n";
        return false;
    }
}

void Game::processEnemyTurns() {
    int enemiesBefore = field.getEnemyCount();
    
    field.moveEnemies(player);
    field.spawnEnemiesFromBuildings();
    field.removeDeadEnemies();
    
    int enemiesAfter = field.getEnemyCount();
    
    // Если врагов стало меньше - кого-то убили
    if (enemiesAfter < enemiesBefore) {
        int enemiesKilled = enemiesBefore - enemiesAfter;
        player.restoreMana(enemiesKilled * 10); // +10 маны за каждого убитого
        std::cout << "Gained " << (enemiesKilled * 10) << " mana for killing enemies!\n";
    }
}

void Game::play() {
    std::cout << "Game Started! Controls: WASD to move, F to switch combat type, Q to quit\n";
    
    while (player.isAlive()) {
        display();
        
        char input;
        std::cout << "Enter move: ";
        std::cin >> input;
        
        if (input == 'q' || input == 'Q') {
            std::cout << "Game ended by player.\n";
            break;
        }else if (input == 'b' || input == 'B') {
            buySpell();
            continue; // Не засчитываем ход за покупку
        }
        
        if (processPlayerMove(input)) {
            turnCount++;
            processEnemyTurns();
            field.removeDeadEnemies();
        }
        
        if (!player.isAlive()) {
            std::cout << "Game Over! Player died.\n";
            break;
        }
        
        if (field.getEnemyCount() == 0) {
            std::cout << "Congratulations! All enemies defeated!\n";
            break;
        }
    }
    
    std::cout << "Final Score: " << player.getScore() << "\n";

}

void Game::buySpell() {
    std::cout << "=== SPELL SHOP ===" << std::endl;
    std::cout << "Your score: " << player.getScore() << std::endl;
    std::cout << "Spells in hand: " << player.getSpellHand().getSpellCount() << "/" << player.getSpellHand().getMaxSize() << std::endl;
    std::cout << std::endl;
    
    std::cout << "Available spells (cost: 50 points each):" << std::endl;
    std::cout << "1. Fireball - Direct damage spell" << std::endl;
    std::cout << "   Deals 30 damage to single target, range: 3" << std::endl;
    std::cout << "2. Firestorm - Area damage spell" << std::endl;
    std::cout << "   Deals 25 damage in 2x2 area, range: 2" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Choose spell to buy (1-2) or 0 to cancel: ";
    int choice;
    std::cin >> choice;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input!" << std::endl;
        return;
    }
    
    std::unique_ptr<Spell> newSpell;
    
    switch (choice) {
        case 1:
            newSpell = std::make_unique<DirectDamageSpell>();
            break;
        case 2:
            newSpell = std::make_unique<AreaDamageSpell>();
            break;
        case 0:
            std::cout << "Purchase cancelled." << std::endl;
            return;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
    }
    
    // Используем метод из SpellCasting
    bool success = SpellCasting::buySpell(player, std::move(newSpell), 50);
    
    if (success) {
        std::cout << "Spell added to your hand!" << std::endl;
    }
}