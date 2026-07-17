#include "inventory.hpp"

#include <string>
#include <vector>

#include "ui.hpp"

//if the inventory has no items there is nothing to select
void selectPreviousItem(Player& player) {
    if (player.inventory.empty()) {
        return;
    }
//moves the slelected inventory slot back by one
    player.selectedItem--;
//if went before the first item in the invenetory, wrap around to the last one in the inventory
    if (player.selectedItem < 0) {
        player.selectedItem = static_cast<int>(player.inventory.size()) - 1;
    }
}

void selectNextItem(Player& player) {
    //if there are no items leave the function
    if (player.inventory.empty()) {
        return;
    }
//move the selected inventory slot foreward
    player.selectedItem++;
//if past the last inventory item go to the first item
    if (player.selectedItem >= static_cast<int>(player.inventory.size())) {
        player.selectedItem = 0;
    }
}

void useSelectedPotion(Player& player) {
    //making sure there is something in the inventory
    if (player.inventory.empty()) {
        player.inventoryMessage = "There is nothing to use.";
        return;
    }
//creates a shortcut to the current selected item
// & means a reference to real time not a copy
    Item& item = player.inventory[player.selectedItem];
//only potions can be used
    if (item.type != "Potion") {
        player.inventoryMessage = item.name + " is not a potion.";
        return;
    }
//decinding what potion was selected
    if (item.name == "Healing Potion") {
        player.health = player.maxHealth;
        player.inventoryMessage = "Health fully restored.";
    } else {
        player.mana = player.maxMana;
        player.inventoryMessage = "Mana fully restored.";
    }
//reduce the amount of the potion by 1
    item.quantity--;
//if no potions are left remove them from the inventory
    if (item.quantity <= 0) {
        player.inventory.erase(player.inventory.begin() + player.selectedItem);

        if (player.selectedItem >= static_cast<int>(player.inventory.size())) {
            player.selectedItem = 0;
        }
    }
}

void equipSelectedItem(Player& player) {
    //not allowing the user to change equipment during combat
    if (player.inCombat) {
        player.inventoryMessage = "Equipment cannot be changed during combat.";
        return;
    }
//making sute there is something to equip
    if (player.inventory.empty()) {
        player.inventoryMessage = "There is nothing to equip.";
        return;
    }
//creates a shortcut to the selected inventory item
    Item& item = player.inventory[player.selectedItem];
//if the item selected is a weapon, equip or unequip it
    if (item.type == "Weapon") {
        if (player.weaponName == item.name) {
            player.weaponName = "Fists";
            player.weaponDamageBonus = -5;
            player.weaponSpeedBonus = 0;
            player.inventoryMessage = "Fists equipped. Melee damage: 5.";
        } else { //otherwise equip the selcted weapon
            player.weaponName = item.name;
            player.weaponDamageBonus = item.damageBonus;
            player.weaponSpeedBonus = item.speedBonus;
            player.inventoryMessage = item.name + " equipped.";
        }
        return;
    }

    if (item.name == "Pyro Book for Dummies") {
        //if already eqiup then unequip it
        if (player.pyroBookEquipped) {
            player.pyroBookEquipped = false;
            player.magicDamage = 0;
            player.inventoryMessage = "Pyro Book unequipped. Magic damage: 0.";
        } else { //equip the book
            player.pyroBookEquipped = true;
            player.magicDamage = 20;
            player.inventoryMessage = "Pyro Book equipped. Fireball unlocked.";
        }
        return;
    }
//if the selected item was not a book or weapon it cannot be equip
    player.inventoryMessage = item.name + " cannot be equipped.";
}
//loop throught every item in the inventory
void addItem(Player& player, Item newItem) {
    for (int i = 0; i < static_cast<int>(player.inventory.size()); i++) {
        //if the user already has the item then increase the quantity
        if (player.inventory[i].name == newItem.name) {
            player.inventory[i].quantity += newItem.quantity;
            return;
        }
    }
//if the item wasnt found add it to the end of the inventory
    player.inventory.push_back(newItem);
}

void showInventory(Player& player) {
    //this vector will store all the text that will appear on the inventory screen
    std::vector<std::string> lines;
//if the inventory is empty display
    if (player.inventory.empty()) {
        lines.push_back("Your inventory is empty.");
    }
//loop through every inventory item
    for (int i = 0; i < static_cast<int>(player.inventory.size()); i++) {
        //start building the line with the items name
        std::string line = player.inventory[i].name;
        //add the quantity
        line += " x" + std::to_string(player.inventory[i].quantity);
//if its the equiped item display it by the name
        if (player.inventory[i].name == player.weaponName) {
            line += " [EQUIPPED]";
        }
//same for the pyro book
        if (player.inventory[i].name == "Pyro Book for Dummies" &&
            player.pyroBookEquipped) {
            line += " [EQUIPPED]";
        }
//save this finished line into the list
        lines.push_back(line);
    }
//display every line on screen
    showMessage("INVENTORY", lines, "Back");
}
//this function controls the entire inventory menu, and keeps displaying until the user chooses to leave
void inventoryMenu(Player& player, bool inCombat) {
    while (true) {
        //if the player presses esc leave the inventory
        if (player.returnToMainMenu) {
            return;
        }
//if there are no items show a message and exit
        if (player.inventory.empty()) {
            showMessage("INVENTORY", {"Your inventory is empty."}, "Back");
            return;
        }
//text that appears about the menu choices
        std::vector<std::string> lines = {
            "Choose an item to use, equip, or unequip."
        };
        //this vector will hold every selectable menu option
        std::vector<std::string> choices;
//loop through every item in the inventory and build the list of menu choices the player can select
        for (int i = 0; i < static_cast<int>(player.inventory.size()); i++) {
            //start the label with the itmes name
            std::string label = player.inventory[i].name;
            //add quantity to the label
            label += " x" + std::to_string(player.inventory[i].quantity);
//if a weapon or book is equip show it beside the name
            if (player.inventory[i].name == player.weaponName ||
                (player.inventory[i].name == "Pyro Book for Dummies" &&
                 player.pyroBookEquipped)) {
                label += " [Equipped]";
            }
//save the finished label into the menu choices
            choices.push_back(label);
        }
//add option to leave the inventory
        choices.push_back("Back");
        int choice = showMenu("INVENTORY", lines, choices);
//if esc pressed then close the inventory menu
        if (choice == -2 ||
            choice == static_cast<int>(player.inventory.size())) {
            return;
        }
//creates a shortcut to the selected inventory item
        Item& item = player.inventory[choice];
//if slected item is potion use it immediately
        if (item.type == "Potion") {
            std::string message;

            if (item.name == "Healing Potion") {
                player.health = player.maxHealth;
                message = "Your health was fully restored.";
            } else {
                player.mana = player.maxMana;
                message = "Your mana was fully restored.";
            }
//reduce the potion quantity by one
            item.quantity--;
//if no potions remain remove item from inventory
            if (item.quantity <= 0) {
                player.inventory.erase(player.inventory.begin() + choice);
            }
//comfirmation message
            showMessage("POTION USED", {message});
            return;
        }
//equipment cannot be changed in combat
        if (inCombat) {
            showMessage(
                "EQUIPMENT LOCKED",
                {"You cannot change equipment during combat."}
            );
            //go back to the top of the loop and redraw the menu
            continue;
        }
//if weapon already equip switch back to fists
        if (item.type == "Weapon") {
            if (player.weaponName == item.name) {
                player.weaponName = "Fists";
                player.weaponDamageBonus = -5;
                player.weaponSpeedBonus = 0;
                showMessage(
                    "WEAPON UNEQUIPPED",
                    {"Fists equipped. Your melee damage is now 5."}
                );
            } else { //otherwise equip selected weapon
                player.weaponName = item.name;
                player.weaponDamageBonus = item.damageBonus;
                player.weaponSpeedBonus = item.speedBonus;
                showMessage("WEAPON EQUIPPED", {item.name + " equipped."});
            }
            continue; //return to inventory menu
        }
//if already equip, unequip it
        if (item.name == "Pyro Book for Dummies") {
            if (player.pyroBookEquipped) {
                player.pyroBookEquipped = false;
                player.magicDamage = 0;
                showMessage(
                    "BOOK UNEQUIPPED",
                    {"Magic damage returned to 0."}
                );
            } else { //otherwise equip it
                player.pyroBookEquipped = true;
                player.magicDamage = 20;
                showMessage(
                    "BOOK EQUIPPED",
                    {"Your magic attack is now Fireball with 20 base damage."}
                );
            }
            continue;
        }
//if selected item is not a weapon or book display
        showMessage("ITEM", {"You cannot use that item."});
    }
}
