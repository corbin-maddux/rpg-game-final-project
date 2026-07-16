#include "levels.hpp"

#include "game_ui.hpp"
#include "merchant.hpp"

// Level 1 reuses the shared level/UI systems while introducing stronger enemies,
// functional turn order, guard weaknesses, and the three-page stats tutorial.

void levelOne(Player& player) {
    // levelEntrance returns false when Escape is used instead of Enter Level.
    if (!levelEntrance(
            player,
            "LEVEL 1: THE HOLLOW HALLS",
            "The stairway descends into halls cut from black stone. The enemies "
            "below have more health, stronger attacks, and better defenses.")) {
        return;
    }

    displayStory(player, "WINGS IN THE DARK", {
        "The stair ends in a vaulted hall. Scratches cover the ceiling, far above "
        "the reach of any ordinary creature.",
        "A leathery shape drops from the darkness and circles once before diving. "
        "Its eyes follow the blue mark on your hand.",
        "\"A hollow bat,\" the stranger mutters. \"Faster than the rat, tougher "
        "too. Watch the ceiling.\""
    }, "Face the Hollow Bat");
    if (player.returnToMainMenu) return;

    // This local Enemy exists only during Level 1 and is copied into its fight.
    Enemy hollowBat{
        "Hollow Bat",
        Stats{65, 65, 14, 7, 4, 0, 0, 0},
        45,
        ""
    };
    if (!enemyEncounter(player, hollowBat)) return;

    displayStory(player, "THE STONE GUARD", {
        "Past the hall stands a gate bound in iron. A statue before it turns its "
        "head as you approach, shedding dust from its armored shoulders.",
        "The stone guard tears an axe from the wall. The floor shakes beneath each "
        "step, and old runes flare across its chest.",
        "\"That thing was built to stop intruders,\" the stranger says. \"At the "
        "moment, that means you.\""
    }, "Challenge the Stone Guard");
    if (player.returnToMainMenu) return;

    // The guard's 16 Speed is higher than the sword user's effective Speed of 8,
    // so enemyEncounter gives the guard the opening attack.
    Enemy stoneGuard{
        "Stone Guard",
        Stats{135, 135, 18, 16, 8, 0, 0, 0},
        60,
        ""
    };
    // Double melee and half magic encourage a different choice than the slime.
    stoneGuard.physicalDamageMultiplier = 2.0;
    stoneGuard.magicDamageMultiplier = 0.5;

    displayCombatTip(player, stoneGuard.name,
        "Guards are weak against melee attacks and take double physical damage. "
        "Their enchanted armor resists spells, so magic deals only half damage. "
        "This guard is also faster than you and will attack first.");
    if (player.returnToMainMenu) return;
    if (!enemyEncounter(player, stoneGuard)) return;

    displayStory(player, "LEVEL 1 COMPLETE", {
        "The guard breaks apart and the iron gate groans open. Warm lantern light "
        "spills across the cold floor from a familiar merchant stall beyond it.",
        "The stranger's voice disappears beneath the sound of deeper machinery. "
        "Whatever controls this place is still awake—and now it knows you are here."
    }, "Visit the Merchant");
    if (player.returnToMainMenu) return;

    merchantEncounter(player);
}