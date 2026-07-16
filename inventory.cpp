#include "inventory.hpp"

#include <string>
#include <vector>

#include "ui.hpp"

void selectPreviousItem(Player& player) {
    if (player.inventory.empty()) {
        return;
    }

    player.selectedItem--;

    if (player.selectedItem < 0) {
        player.selectedItem = static_cast<int>(player.inventory.size()) - 1;
    }
}

void selectNextItem(Player& player) {
    if (player.inventory.empty()) {
        return;
    }

    player.selectedItem++;

    if (player.selectedItem >= static_cast<int>(player.inventory.size())) {
        player.selectedItem = 0;
    }
}

void useSelectedPotion(Player& player) {
    if (player.inventory.empty()) {
        player.inventoryMessage = "There is nothing to use.";
        return;
    }

    Item& item = player.inventory[player.selectedItem];

    if (item.type != "Potion") {
        player.inventoryMessage = item.name + " is not a potion.";
        return;
    }

    if (item.name == "Healing Potion") {
        player.health = player.maxHealth;
        player.inventoryMessage = "Health fully restored.";
    } else {
        player.mana = player.maxMana;
        player.inventoryMessage = "Mana fully restored.";
    }

    item.quantity--;

    if (item.quantity <= 0) {
        player.inventory.erase(player.inventory.begin() + player.selectedItem);

        if (player.selectedItem >= static_cast<int>(player.inventory.size())) {
            player.selectedItem = 0;
        }
    }
}

void equipSelectedItem(Player& player) {
    if (player.inCombat) {
        player.inventoryMessage = "Equipment cannot be changed during combat.";
        return;
    }

    if (player.inventory.empty()) {
        player.inventoryMessage = "There is nothing to equip.";
        return;
    }

    Item& item = player.inventory[player.selectedItem];

    if (item.type == "Weapon") {
        if (player.weaponName == item.name) {
            player.weaponName = "Fists";
            player.weaponDamageBonus = -5;
            player.weaponSpeedBonus = 0;
            player.inventoryMessage = "Fists equipped. Melee damage: 5.";
        } else {
            player.weaponName = item.name;
            player.weaponDamageBonus = item.damageBonus;
            player.weaponSpeedBonus = item.speedBonus;
            player.inventoryMessage = item.name + " equipped.";
        }
        return;
    }

    if (item.name == "Pyro Book for Dummies") {
        if (player.pyroBookEquipped) {
            player.pyroBookEquipped = false;
            player.magicDamage = 0;
            player.inventoryMessage = "Pyro Book unequipped. Magic damage: 0.";
        } else {
            player.pyroBookEquipped = true;
            player.magicDamage = 20;
            player.inventoryMessage = "Pyro Book equipped. Fireball unlocked.";
        }
        return;
    }

    player.inventoryMessage = item.name + " cannot be equipped.";
}

void addItem(Player& player, Item newItem) {
    for (int i = 0; i < static_cast<int>(player.inventory.size()); i++) {
        if (player.inventory[i].name == newItem.name) {
            player.inventory[i].quantity += newItem.quantity;
            return;
        }
    }

    player.inventory.push_back(newItem);
}

void showInventory(Player& player) {
    std::vector<std::string> lines;

    if (player.inventory.empty()) {
        lines.push_back("Your inventory is empty.");
    }

    for (int i = 0; i < static_cast<int>(player.inventory.size()); i++) {
        std::string line = player.inventory[i].name;
        line += " x" + std::to_string(player.inventory[i].quantity);

        if (player.inventory[i].name == player.weaponName) {
            line += " [EQUIPPED]";
        }

        if (player.inventory[i].name == "Pyro Book for Dummies" &&
            player.pyroBookEquipped) {
            line += " [EQUIPPED]";
        }

        lines.push_back(line);
    }

    showMessage("INVENTORY", lines, "Back");
}

void inventoryMenu(Player& player, bool inCombat) {
    while (true) {
        if (player.returnToMainMenu) {
            return;
        }

        if (player.inventory.empty()) {
            showMessage("INVENTORY", {"Your inventory is empty."}, "Back");
            return;
        }

        std::vector<std::string> lines = {
            "Choose an item to use, equip, or unequip."
        };
        std::vector<std::string> choices;

        for (int i = 0; i < static_cast<int>(player.inventory.size()); i++) {
            std::string label = player.inventory[i].name;
            label += " x" + std::to_string(player.inventory[i].quantity);

            if (player.inventory[i].name == player.weaponName ||
                (player.inventory[i].name == "Pyro Book for Dummies" &&
                 player.pyroBookEquipped)) {
                label += " [Equipped]";
            }

            choices.push_back(label);
        }

        choices.push_back("Back");
        int choice = showMenu("INVENTORY", lines, choices);

        if (choice == -2 ||
            choice == static_cast<int>(player.inventory.size())) {
            return;
        }

        Item& item = player.inventory[choice];

        if (item.type == "Potion") {
            std::string message;

            if (item.name == "Healing Potion") {
                player.health = player.maxHealth;
                message = "Your health was fully restored.";
            } else {
                player.mana = player.maxMana;
                message = "Your mana was fully restored.";
            }

            item.quantity--;

            if (item.quantity <= 0) {
                player.inventory.erase(player.inventory.begin() + choice);
            }

            showMessage("POTION USED", {message});
            return;
        }

        if (inCombat) {
            showMessage(
                "EQUIPMENT LOCKED",
                {"You cannot change equipment during combat."}
            );
            continue;
        }

        if (item.type == "Weapon") {
            if (player.weaponName == item.name) {
                player.weaponName = "Fists";
                player.weaponDamageBonus = -5;
                player.weaponSpeedBonus = 0;
                showMessage(
                    "WEAPON UNEQUIPPED",
                    {"Fists equipped. Your melee damage is now 5."}
                );
            } else {
                player.weaponName = item.name;
                player.weaponDamageBonus = item.damageBonus;
                player.weaponSpeedBonus = item.speedBonus;
                showMessage("WEAPON EQUIPPED", {item.name + " equipped."});
            }
            continue;
        }

        if (item.name == "Pyro Book for Dummies") {
            if (player.pyroBookEquipped) {
                player.pyroBookEquipped = false;
                player.magicDamage = 0;
                showMessage(
                    "BOOK UNEQUIPPED",
                    {"Magic damage returned to 0."}
                );
            } else {
                player.pyroBookEquipped = true;
                player.magicDamage = 20;
                showMessage(
                    "BOOK EQUIPPED",
                    {"Your magic attack is now Fireball with 20 base damage."}
                );
            }
            continue;
        }

        showMessage("ITEM", {"You cannot use that item."});
    }
}