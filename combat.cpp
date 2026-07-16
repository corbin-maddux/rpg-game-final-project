#include "combat.hpp"

// This implementation file contains the calculations declared in combat.hpp.

#include <algorithm>
#include <cmath>
#include <string>

namespace {

// Every successful attack deals at least 1 damage, and every spell costs 10
// mana. constexpr prevents these combat rules from changing at runtime.
constexpr int minimumDamage = 1;
constexpr int magicManaCost = 10;

AttackResult calculateAttack(
    int attackPower,
    Stats& defender
) {
    // This local result records the damage and message that the combat screen
    // will display after this particular physical attack.
    AttackResult result;
    result.turnUsed = true;
    result.damage = std::max(
        minimumDamage,
        attackPower - defender.defense
    );

    defender.health = std::max(
        0,
        defender.health - result.damage
    );

    result.message =
        "The attack dealt " +
        std::to_string(result.damage) +
        " damage!";

    // Send the completed attack report back to playerAttack/enemyAttack.
    return result;
}

} // namespace

AttackResult playerAttack(Player& player, Enemy& enemy) {
    const int baseAttackPower =
        player.stats.damage + player.weapon.damageBonus;

    const int attackPower = static_cast<int>(std::round(
        baseAttackPower * enemy.physicalDamageMultiplier
    ));

    return calculateAttack(
        attackPower,
        enemy.stats
    );
}

AttackResult playerMagicAttack(Player& player, Enemy& enemy) {
    // Stop the spell before spending a turn when the player cannot pay 10 mana.
    if (player.stats.mana < magicManaCost) {
        AttackResult result;
        result.turnUsed = false;
        result.message = "Not enough mana!";
        return result;
    }

    player.stats.mana -= magicManaCost;

    const int baseAttackPower =
        player.stats.magicAttack + player.weapon.magicAttackBonus;

    const int attackPower = static_cast<int>(std::round(
        baseAttackPower * enemy.magicDamageMultiplier
    ));

    // Ordinary defense protects against physical attacks. Magic damage is
    // controlled by the enemy's magic multiplier instead, so a base magic
    // attack of 10 against a 1.5x weakness deals exactly 15 damage.
    AttackResult result;
    result.turnUsed = true;
    result.damage = std::max(minimumDamage, attackPower);

    enemy.stats.health = std::max(
        0,
        enemy.stats.health - result.damage
    );

    result.message =
        "The magic attack dealt " +
        std::to_string(result.damage) +
        " damage!";

    result.manaSpent = magicManaCost;

    if (player.pyroBookEquipped) {
        // The damage calculation stays magical, but the equipped book changes
        // the feedback so the player sees that the spell became Fireball.
        result.message =
            "Your fireball exploded for " +
            std::to_string(result.damage) +
            " magic damage!";
    }

    return result;
}

AttackResult enemyAttack(Enemy& enemy, Player& player) {
    return calculateAttack(
        enemy.stats.damage,
        player.stats
    );
}

AttackResult enemyBackAttack(Enemy& enemy, Player& player) {
    return calculateAttack(
        enemy.stats.damage * 2,
        player.stats
    );
}

bool isDefeated(const Stats& stats) {
    return stats.health <= 0;
}
