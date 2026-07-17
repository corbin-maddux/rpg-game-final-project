#include "merchant.hpp"

#include <string>

#include "inventory.hpp"
#include "ui.hpp"
//this file controls the merchatn buying system
void merchant(Player& player) {
    showMessage(
        "MERRICK",
        //shows intro message to the merchant
        {
            "Name's Merrick. I follow the doors, and the doors lead me to coins.",
            "Lucky for you, I have a few things you might need."
        },
        "View Shop"
    );
//if esc pressed return to main menu
    if (player.returnToMainMenu) {
        return;
    }
//kepps showing the menu until user descides to leave
    while (true) {
        if (player.returnToMainMenu) {
            return;
        }
// Display the shop menu.
        // The player's current coin count is shown above the list of items
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
//if esc pressed leave the shop
        if (choice == -2 || choice == 3) {
            return;
        }
//variables that will show the selected items price and info
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
//makes sure the player has enough coins to buy a item
        if (player.coins < price) {
            showMessage(
                "NOT ENOUGH COINS",
                {
                    "Merrick: You are " +
                    std::to_string(price - player.coins) +
                    " coins short."
                }
            );
            //go back to shop menu
            continue;
        }
//the player can afford the item , remove the coins and add to inventory
        player.coins -= price;
        addItem(player, item);

        if (item.name == "Pyro Book for Dummies") {
            //gives a special dialogue if the user bought the pyro book
            showMessage(
                "PURCHASED: PYRO BOOK",
                {
                    "Merrick: Keep your eyebrows away from chapter three.",
                    "Open the inventory outside combat to equip the book."
                }
            );
        } else { //otherwise displays a normal confirmation purchase message
            showMessage(
                "PURCHASE COMPLETE",
                {
                    "You bought " + item.name + ".",
                    "Coins remaining: " + std::to_string(player.coins)
                }
            ); //after the message closes the while loop automatically repeats, showing the shop again.
        }
    }
}
