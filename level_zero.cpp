#include "levels.hpp"

#include "game_ui.hpp"
#include "inventory.hpp"
#include "merchant.hpp"

// Level 0 introduces story screens, the first weapon, coins, physical combat,
// slime weaknesses, inventory rewards, and the first merchant visit.

void levelZero(Player& player) {
    // displayStory pauses progression until its single button is activated.
    displayStory(player, "THE FIRST CHAMBER", {
        "You answer, but the stranger does not give you a name. They only tell "
        "you that the chamber has one exit and that it will not stay quiet for long.",
        "A chipped iron sword lies beneath an empty torch bracket. The symbol "
        "on your hand is carved into its pommel, glowing with the same blue light.",
        "\"Take it,\" the stranger says. \"Whatever made those scratches sounds "
        "hungry, and I doubt it plans to wait for an introduction.\""
    }, "Reach for the Sword");
    // Every screen checks this flag so Escape stops the whole level cleanly.
    if (player.returnToMainMenu) return;

    displayItem(player, "Iron Sword",
        "The chipped blade still feels solid. Damage bonus: 20. Speed penalty: 2.");
    if (player.returnToMainMenu) return;

    // Store the sword as inventory data so E can later equip or unequip it.
    player.inventory.addItem({
        "Iron Sword",
        "A chipped iron sword. Adds 20 physical damage but reduces speed by 2.",
        ItemType::Weapon,
        1,
        0,
        false,
        false,
        20,
        -2
    });
    // Equip it immediately: +20 damage, -2 speed, and no magic bonus.
    player.weapon = Weapon{"Iron Sword", 20, -2, 0};

    // This variable packages the rat's name, stats, and victory rewards.
    Enemy dungeonRat{
        "Dungeon Rat",
        Stats{30, 30, 10, 7, 2, 0, 0, 0},
        5,
        ""
    };

    displayStory(player, "SOMETHING STIRS", {
        "The iron door rattles. Dust falls from its hinges as a low growl rolls "
        "through the corridor beyond it.",
        "A dungeon rat forces itself through the gap. Its fur is patchy, one ear "
        "is missing, and its blackened teeth scrape against the stone.",
        "\"Keep the sharp end pointed at it,\" the stranger says. \"And try not "
        "to die in the doorway. You're blocking the only exit.\""
    }, "Stand Your Ground");
    if (player.returnToMainMenu) return;

    // A false encounter result means defeat or menu return, so stop the level.
    if (!enemyEncounter(player, dungeonRat)) return;

    displayStory(player, "A TRAIL IN THE DARK", {
        "The rat collapses. Several coins roll from beneath it and settle near "
        "a pale green trail that shines along the corridor.",
        "The trail leads to a sealed door, then rises into a trembling mass of "
        "slime that swallows loose bones and rusted armor as it grows.",
        "\"Don't rely on the sword,\" the stranger warns. \"A blade passes "
        "through slime. Use magic—assuming you can figure out how.\""
    }, "Approach the Door");
    if (player.returnToMainMenu) return;

    Enemy caveSlime{
        "Cave Slime",
        Stats{50, 50, 20, 6, 5, 0, 0, 0},
        10,
        "Healing Potion"
    };
    // The multipliers create the slime rule: resist melee and amplify magic.
    caveSlime.physicalDamageMultiplier = 0.5;
    caveSlime.magicDamageMultiplier = 2.5;

    displayCombatTip(player, caveSlime.name,
        "Slimes reduce physical damage to 50%, but take 2.5 times magic damage.");
    if (player.returnToMainMenu) return;

    if (!enemyEncounter(player, caveSlime)) return;

    displayStory(player, "THE SEALED DOOR", {
        "The slime spreads into a harmless film. A warm glass vial remains in "
        "its center, untouched by the fight.",
        "The mark on your hand flashes. Stone gears grind, the sealed door splits, "
        "and cold air spills through the widening gap.",
        "\"You made it,\" the stranger says. \"Come through quickly. What lives "
        "on the next floor is worse than rats.\""
    }, "Meet the Merchant");
    if (player.returnToMainMenu) return;

    merchantEncounter(player);
    if (player.returnToMainMenu) return;
    // Carry this same Player, inventory, health, mana, and coins into Level 1.
    levelOne(player);
}