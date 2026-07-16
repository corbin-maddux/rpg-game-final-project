#include "leveling.hpp"

#include <string>

#include "ui.hpp"

void grantExperience(Player& player, int amount) {
    player.experience += amount;
}

int processLevelUps(Player& player) {
    int levelsGained = 0;

    while (player.experience >= player.levelUpRequirement) {
        player.experience -= player.levelUpRequirement;
        player.level++;
        levelsGained++;
        player.levelUpModifier += 2;

        player.damage += 2;
        player.speed += 2;
        player.defense += 2;
        player.magicDamage += 2;

        player.maxHealth += 10;
        player.health = player.maxHealth;

        player.levelUpRequirement += 15 + player.levelUpModifier;
    }

    return levelsGained;
}

void showFirstLevelUpTutorial(Player& player) {
    if (player.levelTutorialShown) {
        return;
    }

    showMessage(
        "LEVEL UP GUIDE 1/2: YOUR STATS",
        {
            "You reached Level " + std::to_string(player.level) + "!",
            "Each level adds 2 Damage, 2 Speed, 2 Defense, and 2 Magic Damage.",
            "Maximum Health increases by 10.",
            "Leveling also fully restores your Health."
        },
        "Next Page"
    );

    if (player.returnToMainMenu) {
        return;
    }

    showMessage(
        "LEVEL UP GUIDE 2/2: EARNING EXP",
        {
            "You earn EXP by defeating enemies.",
            "Stronger enemies normally award more EXP.",
            "Your Level and EXP progress are always shown in the right panel.",
            "Reach the displayed EXP requirement to gain another level."
        },
        "Continue"
    );

    player.levelTutorialShown = true;
}