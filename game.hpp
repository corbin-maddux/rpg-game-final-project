#pragma once

#include <string>
#include <vector>

// structure for items
struct Item {
    std::string name;
    std::string type;
    int quantity = 1;
    int damageBonus = 0;
    int speedBonus = 0;
};

struct Player {
    //player's stats
    int health = 100;
    int maxHealth = 100;
    int mana = 50;
    int maxMana = 50;
    int damage = 10;
    int magicDamage = 10;
    int speed = 10;
    int defense = 5;
    
    // player's coins
    int coins = 0;

    // variables for the leveling up system
    int level = 1;
    int experience = 0;
    int levelUpRequirement = 100;
    int levelUpModifier = 0;
    bool levelTutorialShown = false;

    //variables for player's weapons
    std::string weaponName = "Fists";
    int weaponDamageBonus = -5;
    int weaponSpeedBonus = 0;
    bool pyroBookEquipped = false;

    //variables to check if the player's in combat or returning to the main menu
    bool inCombat = false;
    bool returnToMainMenu = false;

    // variables for inventory
    int selectedItem = 0;
    std::string inventoryMessage;
    std::vector<Item> inventory;
};

// function for enemy and enemy stats
struct Enemy {
    std::string name;
    int health;
    int damage;
    int speed;
    int defense;
    int coinDrop;
    double meleeMultiplier = 1.0;
    double magicMultiplier = 1.0;
    int experienceDrop = 0;
};