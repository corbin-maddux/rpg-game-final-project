#include "combat.hpp"

#include <string>
#include <vector>

#include "ui.hpp"

int enemyTurn(Player& player, Enemy& enemy) {
    int damage = enemy.damage - player.defense;

    if (damage < 1) {
        damage = 1;
    }

    player.health -= damage;

    if (player.health < 0) {
        player.health = 0;
    }

    return damage;
}

int fightEnemy(Player& player, Enemy enemy) {
    player.inCombat = true;
    std::string combatMessage = enemy.name + " moves toward you.";
    int playerSpeed = player.speed + player.weaponSpeedBonus;

    if (enemy.speed > playerSpeed) {
        int damage = enemyTurn(player, enemy);
        combatMessage = enemy.name + " is faster and attacks first for ";
        combatMessage += std::to_string(damage) + " damage.";

        showMessage(
            "SPEED",
            {
                enemy.name + " has " + std::to_string(enemy.speed) + " Speed.",
                "You have " + std::to_string(playerSpeed) + " effective Speed.",
                "The faster fighter gets the opening attack."
            },
            "Continue Fight"
        );

        if (player.returnToMainMenu) {
            player.inCombat = false;
            return -1;
        }
    }

    while (player.health > 0 && enemy.health > 0) {
        std::vector<std::string> information = {
            "Enemy: " + enemy.name,
            "Enemy HP: " + std::to_string(enemy.health),
            "Your HP: " + std::to_string(player.health) +
                "/" + std::to_string(player.maxHealth),
            "Mana: " + std::to_string(player.mana) +
                "/" + std::to_string(player.maxMana),
            combatMessage
        };

        std::string magicButton = "Magic Attack";

        if (player.pyroBookEquipped) {
            magicButton = "Fireball";
        }

        int choice = showMenu(
            "ENEMY ENCOUNTER",
            information,
            {
                "Melee Attack",
                magicButton,
                "Run Away",
                "Return to Main Menu"
            }
        );

        bool turnUsed = false;

        if (choice == -2 || choice == 3) {
            player.inCombat = false;
            return -1;
        }

        if (choice == 0) {
            int damage = player.damage + player.weaponDamageBonus;
            damage = static_cast<int>(damage * enemy.meleeMultiplier);
            damage -= enemy.defense;

            if (damage < 1) {
                damage = 1;
            }

            enemy.health -= damage;
            combatMessage = "You dealt " + std::to_string(damage);
            combatMessage += " melee damage.";
            turnUsed = true;
        } else if (choice == 1) {
            if (player.mana < 10) {
                combatMessage = "You do not have enough mana.";
            } else {
                player.mana -= 10;
                int damage = static_cast<int>(
                    player.magicDamage * enemy.magicMultiplier
                );

                if (damage < 1) {
                    damage = 1;
                }

                enemy.health -= damage;

                if (player.pyroBookEquipped) {
                    combatMessage = "Your fireball dealt ";
                } else {
                    combatMessage = "Your magic dealt ";
                }

                combatMessage += std::to_string(damage) + " magic damage.";
                turnUsed = true;
            }
        } else if (choice == 2) {
            int damage = (enemy.damage * 2) - player.defense;

            if (damage < 1) {
                damage = 1;
            }

            player.health -= damage;
            combatMessage = "Coward. You cannot escape. ";
            combatMessage += enemy.name + " hit your back for ";
            combatMessage += std::to_string(damage) + " damage.";
        }

        if (enemy.health <= 0) {
            break;
        }

        if (turnUsed) {
            int damage = enemyTurn(player, enemy);
            combatMessage += " " + enemy.name + " dealt ";
            combatMessage += std::to_string(damage) + " damage.";
        }
    }

    if (player.health <= 0) {
        player.inCombat = false;
        showMessage(
            "DEFEAT",
            {"You were defeated by " + enemy.name + "."},
            "Return to Main Menu"
        );
        return 0;
    }

    player.coins += enemy.coinDrop;
    player.inCombat = false;
    showMessage(
        "VICTORY",
        {
            "You defeated " + enemy.name + "!",
            "Coins found: " + std::to_string(enemy.coinDrop),
            "Total coins: " + std::to_string(player.coins)
        }
    );
    return 1;
}