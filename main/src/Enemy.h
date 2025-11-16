#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
private:
    int health;
    int damage;
    int maxHealth;

public:
    Enemy(int hp = 30, int dmg = 10);
    
    int getHealth() const;
    int getDamage() const;
    int getMaxHealth() const;
        
    void takeDamage(int dmg);
    bool isAlive() const;
};

#endif // ENEMY_H