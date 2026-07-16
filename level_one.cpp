#include "levels.hpp"

#include "combat.hpp"
#include "merchant.hpp"
#include "leveling.hpp"
#include "ui.hpp"

bool levelOne(Player& player) {
    showMessage(
        "LEVEL 1: THE HOLLOW HALLS",
        {
            "You enter a hallway carved from black stone.",
            "Something moves along the ceiling above you."
        },
        "Continue"
    );

    if (player.returnToMainMenu) return false;

    Enemy bat = {
        "Hollow Bat", 65, 14, 7, 4, 45, 1.0, 1.0, 75
    };

    int result = fightEnemy(player, bat);

    if (result != 1) {
        return false;
    }

    showMessage(
        "COMBAT TIP: STONE GUARD",
        {
            "Guards are weak against melee attacks.",
            "They take double physical damage.",
            "Their armor resists magic, so spells deal half damage.",
            "This guard is faster than you and attacks first."
        },
        "Learn About Stats"
    );

    if (player.returnToMainMenu) return false;

    showMessage(
        "STATS GUIDE 1/3: SPEED",
        {
            "Speed decides who receives the opening attack.",
            "Your weapon can increase or decrease your effective Speed.",
            "The Iron Sword lowers your Speed by 2."
        },
        "Next Page"
    );

    if (player.returnToMainMenu) return false;

    showMessage(
        "STATS GUIDE 2/3: ATTACKS",
        {
            "Melee starts with Damage plus the equipped weapon bonus.",
            "Enemy weakness is applied, then enemy Defense is subtracted.",
            "Magic uses Magic Damage and the enemy's magic multiplier."
        },
        "Next Page"
    );

    if (player.returnToMainMenu) return false;

    showMessage(
        "STATS GUIDE 3/3: SURVIVAL",
        {
            "Health determines how much damage you can survive.",
            "Mana is spent on magic attacks.",
            "Potions restore Health or Mana from the inventory.",
            "Choose attacks that match each enemy's weakness."
        },
        "Fight the Guard"
    );

    if (player.returnToMainMenu) return false;

    Enemy guard = {
        "Stone Guard", 135, 18, 16, 8, 60, 2.0, 0.5, 100
    };

    result = fightEnemy(player, guard);

    if (result != 1) {
        return false;
    }

    int levelsGained = processLevelUps(player);

    if (levelsGained > 0) {
        showMessage(
            "LEVEL UP",
            {
                "You reached Level " + std::to_string(player.level) + "!",
                "Your combat stats increased and your Health was restored.",
                "EXP remaining: " + std::to_string(player.experience) +
                    "/" + std::to_string(player.levelUpRequirement)
            }
        );
    }

    if (player.returnToMainMenu) return false;

    showMessage(
        "LEVEL 1 COMPLETE",
        {
            "The Stone Guard breaks apart.",
            "The iron gate opens and Merrick waits beyond it."
        },
        "Visit Merrick"
    );

    if (player.returnToMainMenu) return false;

    merchant(player);
    if (player.returnToMainMenu) return false;
    return true;
}