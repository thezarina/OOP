#include "Spell.h"
#include "Field.h"    
#include "Player.h"  
#include <string>

Spell::Spell(const std::string &name, const std::string description, int manaCost)
    : name(name), description(description), manaCost(manaCost) {}

const std::string& Spell::getName() const{
    return name;
}
const std::string& Spell::getDescription() const{
    return description;
}
int Spell::getManaCost() const{
    return manaCost;
}
