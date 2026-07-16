//file for the core gameplay mechanics
#include <iostream>
/*
//player's base stats when level 1
int playerBaseHealth = 100;
int playerBaseAttack = 15;
int playerBaseSpecialAttack = 13;
int playerBaseDefense = 30;
int playerBaseMana = 15;
int playerLevel = 1;
int playerEXP = 0;

//Variables for the player's stats that'll be affected by modifiers 
int playerHealth;
int playerAttack;
int playerSpecialAttack;
int playerDefense;
int playerMana;*/

//initial level up modifier and level up milestones
// int levelUpModifier = 0;
// int levelUpRequirement = 5;

// // every time the player gains enough EXP, they'll level up, and their stats will increase by 2
// int levelSystem() {
//     while (playerEXP > levelUpRequirement - 1) {
//         playerLevel += 1;
//         if(playerLevel > 1){
//             levelUpModifier += 2; 
//         }
//         levelUpRequirement = levelUpRequirement + 15 + levelUpModifier;
//     }   
//     return playerLevel;
// }
/*
//weapons and how much they add onto ATK stats
int weaponSelected;
//physical ATK
int brokenSword = 3;
int longSword = 7;
int claymore = 15;
//special ATK
int fireSpell = 10;
int electroSpell = 10;*/

//function that declares player's stats with modifiers
// void displayPlayerStats() {
//     playerHealth = playerBaseHealth + levelUpModifier;
//     playerAttack = playerBaseAttack + levelUpModifier;
//     playerSpecialAttack = playerBaseSpecialAttack + levelUpModifier;
//     playerDefense = playerBaseDefense + levelUpModifier;
//     playerMana = playerBaseMana + levelUpModifier;

    //displays player's current stats in terminal to debug
//     std::cout
//         << "\n---------------------------------------" 
//         << "\nPlayer's current stats are: "
//         << "\nLv. " << playerLevel
//         << "\n" << playerEXP << " XP"
//         << "\nEXP nedded to level up: "  << levelUpRequirement << " EXP"
//         << "\n" << playerHealth << " HP"
//         << "\n" << playerAttack << " ATK"
//         << "\n" << playerSpecialAttack << " SP. ATK"
//         << "\n" << playerDefense << " DEF"
//         << "\n" << playerMana << " FP"
//         << "\n---------------------------------------" 
//         << "\nWeapons:" 
//         << "\nBroken Sword: + " << brokenSword << " ATK"
//         << "\nLong Sword: + " << longSword << " ATK"
//         << "\nClaymore: + " << claymore << " ATK"
//         << "\nFire Spells: + " << fireSpell << " SP. ATK"
//         << "\nElectro Spells: + " << electroSpell << " SP. ATK"
//         << "\n---------------------------------------" 
//         << "\nLevel up modifier is: " << levelUpModifier
//         << std::endl;
//}

//main function that will run other functions in this file
// int main() {
//     // levelSystem();
//     displayPlayerStats();
//     return 0;
// }