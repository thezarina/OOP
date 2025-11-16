#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int range;
    int areaSize;
    bool enhanced;

public:
    AreaDamageSpell(int damage = 25, int range = 2, int areaSize = 2,
                   const std::string& name = "Firestorm", 
                   const std::string& description = "Deals damage in an area",
                   int manaCost = 30);
    
    bool cast(GameField& field, Player& player, int targetX, int targetY) override;
    bool requiresTarget() const override { return true; }
    
    void enhance(int damageBonus = 5, int areaBonus = 1);
};

#endif