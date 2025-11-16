#ifndef SPELL_H
#define SPELL_H


class GameField;  // Вместо #include "Field.h"
class Player;     // Вместо #include "Player.h"
#include <string>

class Spell{

protected:
    std::string name;
    std::string description;
    int manaCost;

public:
    Spell(const std::string& name, const std::string description, int manaCost);
    virtual ~Spell() = default;

    virtual bool cast(GameField& field, Player& player, int TargetX, int TargetY) = 0;

    const std::string& getName() const;
    const std::string& getDescription() const;
    int getManaCost() const;

    virtual bool requiresTarget() const = 0;

};

#endif