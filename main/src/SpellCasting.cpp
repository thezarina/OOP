#include "SpellCasting.h"
#include "Player.h"
#include "Field.h"
#include "Spell.h"
#include <random>
#include <chrono>

// Метод для применения заклинания
bool SpellCasting::castSpell(Player& player, int spellIndex, GameField& field, int targetX, int targetY) {
    if (!player.isAlive()) {
        return false;
    }
    
    Spell* spell = player.getSpellHand().getSpell(spellIndex);
    if (spell == nullptr) {
        return false;
    }
    
    if (!player.canCastSpell(*spell)) {
        return false;
    }
    
    // Применяем заклинание
    bool success = spell->cast(field, player, targetX, targetY);
    
    // Тратим ход на применение заклинания
    if (success) {
        player.setMovement(false);
    }
    
    return success;
}

std::unique_ptr<Spell> SpellCasting::getRandomStarterSpell() {
    // Создаем генератор случайных чисел
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 1);
    
    int spellType = dis(gen);
    
    switch (spellType) {
        case 0:
            return std::make_unique<DirectDamageSpell>();
        case 1:
            return std::make_unique<AreaDamageSpell>();
        default:
            return std::make_unique<DirectDamageSpell>(); 
    }
}

// Метод для проверки, есть ли у игрока заклинания
bool SpellCasting::hasSpells(const Player& player) {
    return player.getSpellHand().getSpellCount() > 0;
}

// Метод для отображения информации о мане и заклинаниях
void SpellCasting::displaySpellInfo(const Player& player) {
    std::cout << "Mana: " << player.getMana() << "/" << player.getMaxMana() << std::endl;
    std::cout << "Spells in hand: " << player.getSpellHand().getSpellCount() 
              << "/" << player.getSpellHand().getMaxSize() << std::endl;
    
    for (size_t i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
        const Spell* spell = player.getSpellHand().getSpell(i);
        if (spell) {
            std::cout << i + 1 << ". " << spell->getName() 
                      << " (" << spell->getManaCost() << " mana) - " 
                      << spell->getDescription();
            
            if (!player.canCastSpell(*spell)) {
                std::cout << " [Not enough mana]";
            }
            std::cout << std::endl;
        }
    }
}

// Метод для покупки заклинания
bool SpellCasting::buySpell(Player& player, std::unique_ptr<Spell> spell, int cost) {
    if (spell == nullptr) {
        return false;
    }
    
    // Проверяем, хватает ли очков
    if (player.getScore() < cost) {
        std::cout << "Not enough score points! Need " << cost << ", but have " << player.getScore() << std::endl;
        return false;
    }
    
    // Пытаемся добавить заклинание в руку
    if (player.getSpellHand().addSpell(std::move(spell))) {
        // Вычитаем стоимость из очков
        player.addScore(-cost);
        std::cout << "Spell purchased successfully!" << std::endl;
        return true;
    } else {
        std::cout << "Spell hand is full! Cannot purchase new spell." << std::endl;
        return false;
    }
}