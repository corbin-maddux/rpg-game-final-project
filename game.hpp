#pragma once

#include <string>
#include <vector>

struct Item {
    std::string name;
    std::string type;
    int quantity = 1;
    int damageBonus = 0;
    int speedBonus = 0;
};

struct Player {
    int health = 100;
    int maxHealth = 100;
    int mana = 50;
    int maxMana = 50;
    int damage = 10;
    int magicDamage = 10;
    int speed = 10;
    int defense = 5;
    int coins = 0;

    std::string weaponName = "Fists";
    int weaponDamageBonus = -5;
    int weaponSpeedBonus = 0;
    bool pyroBookEquipped = false;
    bool inCombat = false;
    bool returnToMainMenu = false;
    int selectedItem = 0;
    std::string inventoryMessage;

    std::vector<Item> inventory;
};

struct Enemy {
    std::string name;
    int health;
    int damage;
    int speed;
    int defense;
    int coinDrop;
    double meleeMultiplier = 1.0;
    double magicMultiplier = 1.0;
};