#pragma once

// This header defines the player, enemy, weapon, and attack data that must be
// shared between the combat calculations, level files, inventory, and UI.
// #pragma once prevents this file from being included twice in one build unit.
#include <string>
#include "inventory.hpp"
// Core numerical attributes shared by players and enemies.
struct Stats {
    // maxHealth is the healing ceiling; health is the amount currently left.
    int maxHealth = 100;
    int health = 100;

    // damage begins physical calculations, speed decides who opens a fight,
    // and defense is subtracted from incoming physical damage.
    int damage = 10;
    int speed = 10;
    int defense = 5;

    // maxMana limits potion restoration, mana pays spell costs, and
    // magicAttack supplies the starting power for a spell.
    int maxMana = 50;
    int mana = 50;
    int magicAttack = 10;
};

// A weapon adds bonuses to the player's base stats.
struct Weapon {
    // name appears in the inventory. The three bonuses are added to the
    // matching Player stats while this weapon is equipped.
    std::string name = "Fists";
    int damageBonus = 0;
    int speedBonus = 0;
    int magicAttackBonus = 0;
};

// Information that remains with the player throughout the game.
struct Player {
    int coins = 0;
    // These flags remember one-time tutorials and current UI/combat state so
    // screens do not repeat or allow actions at the wrong time.
    bool coinExplanationShown = false;
    bool highlightCoins = false;
    bool merchantIntroduced = false;
    bool returnToMainMenu = false;
    bool inCombat = false;
    bool pyroBookEquipped = false;
    bool statsExplanationShown = false;
    std::string inventoryMessage;

    Stats stats;
    // Base player damage is 10. A -5 bonus makes bare fists deal 5 damage.
    Weapon weapon{"Fists", -5, 0, 0};
    Inventory inventory;
};

// All information needed to run an encounter and award its rewards.
struct Enemy {
    std::string name;
    Stats stats;
    int coinDrop = 0;
    std::string extraDrop;

    //Calculates Damage Resistance/Damage weakness for enemies, set at one but can change

    double physicalDamageMultiplier = 1.0;
    double magicDamageMultiplier = 1.0;
};

// Describes what happened when an attack was attempted.
struct AttackResult {
    bool turnUsed = true;
    int damage = 0;
    int manaSpent = 0;
    std::string message;
};

// Calculates one physical player attack and directly lowers this enemy's HP.
// References(&)are used so changes remain after the function finishes.
AttackResult playerAttack(Player& player, Enemy& enemy);

// Magic attack. A failed attempt caused by insufficient mana should set
// turnUsed to false so the enemy does not receive a free turn.
AttackResult playerMagicAttack(Player& player, Enemy& enemy);

// Physical enemy attack against the player.
AttackResult enemyAttack(Enemy& enemy, Player& player);

// A stronger enemy attack used when the player turns their back to run.
AttackResult enemyBackAttack(Enemy& enemy, Player& player);

// Returns true when a combatant has no health remaining.
bool isDefeated(const Stats& stats);
