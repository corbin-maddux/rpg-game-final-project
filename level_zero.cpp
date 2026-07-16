#include "levels.hpp"

#include "combat.hpp"
#include "inventory.hpp"
#include "merchant.hpp"
#include "leveling.hpp"
#include "ui.hpp"

bool levelZero(Player& player) {
    showMessage(
        "LEVEL 0: THE FIRST CHAMBER",
        {
            "You wake on a cold stone floor.",
            "A chipped sword rests beneath an empty torch bracket.",
            "A stranger whispers through the wall: Pick it up. Quickly."
        },
        "Take the Sword"
    );

    if (player.returnToMainMenu) return false;

    Item sword = {"Iron Sword", "Weapon", 1, 20, -2};
    addItem(player, sword);
    player.weaponName = sword.name;
    player.weaponDamageBonus = sword.damageBonus;
    player.weaponSpeedBonus = sword.speedBonus;

    showMessage(
        "ITEM FOUND",
        {"Iron Sword equipped.", "Damage bonus: 20", "Speed penalty: 2"}
    );

    if (player.returnToMainMenu) return false;

    Enemy rat = {
        "Dungeon Rat", 30, 10, 7, 2, 5, 1.0, 1.0, 40
    };

    int result = fightEnemy(player, rat);

    if (result != 1) {
        return false;
    }

    showMessage(
        "COMBAT TIP: CAVE SLIME",
        {
            "Slimes resist physical attacks.",
            "Melee damage is reduced to one half.",
            "Magic damage is multiplied by 2.5."
        },
        "Fight the Slime"
    );

    if (player.returnToMainMenu) return false;

    Enemy slime = {
        "Cave Slime", 50, 20, 6, 5, 10, 0.5, 2.5, 60
    };

    result = fightEnemy(player, slime);

    if (result != 1) {
        return false;
    }

    addItem(player, {"Healing Potion", "Potion", 1, 0, 0});
    showMessage(
        "ITEM FOUND",
        {"The slime dropped a Healing Potion."},
        "Meet Merrick"
    );

    if (player.returnToMainMenu) return false;

    int levelsGained = processLevelUps(player);

    if (levelsGained > 0) {
        showFirstLevelUpTutorial(player);
    }

    if (player.returnToMainMenu) return false;

    merchant(player);
    if (player.returnToMainMenu) return false;
    return true;
}