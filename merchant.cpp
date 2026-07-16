#include "merchant.hpp"

#include <string>

#include "inventory.hpp"
#include "ui.hpp"

void merchant(Player& player) {
    showMessage(
        "MERRICK",
        {
            "Name's Merrick. I follow the doors, and the doors lead me to coins.",
            "Lucky for you, I have a few things you might need."
        },
        "View Shop"
    );

    if (player.returnToMainMenu) {
        return;
    }

    while (true) {
        if (player.returnToMainMenu) {
            return;
        }

        int choice = showMenu(
            "MERRICK'S SHOP",
            {"Your coins: " + std::to_string(player.coins)},
            {
                "Mana Potion - 15 coins",
                "Pyro Book for Dummies - 100 coins",
                "Axe of the Warrior - 150 coins",
                "Leave Shop"
            }
        );

        if (choice == -2 || choice == 3) {
            return;
        }

        int price = 0;
        Item item;

        if (choice == 0) {
            price = 15;
            item = {"Mana Potion", "Potion", 1, 0, 0};
        } else if (choice == 1) {
            price = 100;
            item = {"Pyro Book for Dummies", "Book", 1, 0, 0};
        } else if (choice == 2) {
            price = 150;
            item = {"Axe of the Warrior", "Weapon", 1, 35, -5};
        }

        if (player.coins < price) {
            showMessage(
                "NOT ENOUGH COINS",
                {
                    "Merrick: You are " +
                    std::to_string(price - player.coins) +
                    " coins short."
                }
            );
            continue;
        }

        player.coins -= price;
        addItem(player, item);

        if (item.name == "Pyro Book for Dummies") {
            showMessage(
                "PURCHASED: PYRO BOOK",
                {
                    "Merrick: Keep your eyebrows away from chapter three.",
                    "Open the inventory outside combat to equip the book."
                }
            );
        } else {
            showMessage(
                "PURCHASE COMPLETE",
                {
                    "You bought " + item.name + ".",
                    "Coins remaining: " + std::to_string(player.coins)
                }
            );
        }
    }
}