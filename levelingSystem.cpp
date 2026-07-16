#include "levelingSystem.hpp"

// every time the player gains enough EXP, they'll level up, and their stats will increase by 2
int levelSystem(LevelSystem& player) {
    while (player.playerEXP > player.levelUpRequirement - 1) {
        player.playerLevel += 1;
        if (player.playerLevel > 1) {
            player.levelUpModifier += 2;
        }
        player.levelUpRequirement = player.levelUpRequirement + 15 + player.levelUpModifier;
    }
    return player.playerLevel;
}