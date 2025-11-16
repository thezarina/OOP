#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "Player.h"
#include <iostream>

class Game {
private:
    GameField field;
    Player player;
    int turnCount;

public:
    Game(int width = 15, int height = 15);
    
    void display() const;
    bool processPlayerMove(char direction);
    void processEnemyTurns();
    void play();
    void buySpell();
};

#endif // GAME_H