#include "Game.h"
#include <iostream>

int main() {
    int height;
    int width;
    std::cout << "Input size of field" << std::endl;
    std::cin >> width;
    std::cin >> height;

    Game game(width, height);
    game.play();
    return 0;
}