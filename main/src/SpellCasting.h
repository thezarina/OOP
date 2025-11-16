#ifndef SPELL_CASTING_H
#define SPELL_CASTING_H

#include "Player.h"
#include "Field.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <memory>
#include <iostream>

class SpellCasting {
public:
    // Метод для применения заклинания
    static bool castSpell(Player& player, int spellIndex, GameField& field, int targetX, int targetY);
    
    // Метод для получения случайного заклинания (начальное заклинание)
    static std::unique_ptr<Spell> getRandomStarterSpell();
    
    // Метод для проверки, есть ли у игрока заклинания
    static bool hasSpells(const Player& player);
    
    // Метод для отображения информации о мане и заклинаниях
    static void displaySpellInfo(const Player& player);
    
    // Метод для покупки заклинания
    static bool buySpell(Player& player, std::unique_ptr<Spell> spell, int cost);
};

#endif // SPELL_CASTING_H