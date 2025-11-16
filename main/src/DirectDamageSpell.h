#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;
    bool enhanced;

public:
    DirectDamageSpell(int damage = 30, int range = 3, 
                     const std::string& name = "Fireball", 
                     const std::string& description = "Deals direct damage to a single target",
                     int manaCost = 20);
    
    bool cast(GameField& field, Player& player, int targetX, int targetY) override;
    bool requiresTarget() const override { return true; }
    
    void setDamage(int newDamage);
    void setRange(int newRange);
    void enhance(int damageBonus = 10, int rangeBonus = 2);
};

#endif