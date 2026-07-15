//g++ FINALinventory.cpp -std=c++20 -lftxui-component -lftxui-dom -lftxui-screen -o inventory
//./inventory
#include <iostream>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace std;
using namespace ftxui;

struct Weapon {
    string name;
    int damage;
};
struct Potion {
    string name;
    string effect;
};
struct Book {
    string title;
    string description;
};
struct Inventory {
    int currency = 0;
    vector<Weapon> weapons;
    vector<Potion> potions;
    vector<Book> books;
    int equippedWeapon = 0;
};

//FUNCTIONS

void equipWeapon(Inventory& inv, int index){
    if(index >= 0 && index < (int)inv.weapons.size()){
        inv.equippedWeapon = index;
    }
}
void usePotion(Inventory& inv, int index){
    if(index >= 0 && index < (int)inv.potions.size()){
        cout << "Used "
             << inv.potions[index].name
             << endl;
        inv.potions.erase(inv.potions.begin() + index);
    }
}

// BUILD LABELS

vector<string> buildWeaponLabels(const Inventory& inv){
    vector<string> labels;
    for(const auto& weapon : inv.weapons){
        labels.push_back(
            weapon.name +
            " (" +
            to_string(weapon.damage) +
            " DMG)"
        );
    }
    return labels;
}

vector<string> buildPotionLabels(const Inventory& inv){
    vector<string> labels;
    for(const auto& potion : inv.potions){
        labels.push_back(
            potion.name +
            " - " +
            potion.effect
        );
    }
    return labels;
}

vector<string> buildBookLabels(const Inventory& inv){
    vector<string> labels;
    for(const auto& book : inv.books){
        labels.push_back(
            book.title +
            " - " +
            book.description
        );
    }
    return labels;
}

// MAIN

int main(){
    Inventory inv;
    inv.currency = 150;
    inv.weapons.push_back({"Iron Sword",20});
    inv.weapons.push_back({"Steel Axe",30});
    inv.weapons.push_back({"Hunter Bow",18});
    inv.potions.push_back(
        {"Healing Potion","Restores 50 HP"}
    );
    inv.potions.push_back(
        {"Strength Potion","+10 Damage"}
    );
    inv.books.push_back(
        {"Dungeon Journal","Old explorer notes"}
    );
    inv.books.push_back(
        {"Ancient Magic","Strange symbols"}
    );
    auto screen =
        ScreenInteractive::Fullscreen();

    int weaponSelected = 0;
    int potionSelected = 0;
    int bookSelected = 0;

    // IMPORTANT:
    // Store these vectors FIRST.
    // Do NOT pass buildWeaponLabels()
    // directly into Menu().

    vector<string> weaponLabels =
        buildWeaponLabels(inv);
    vector<string> potionLabels =
        buildPotionLabels(inv);
    vector<string> bookLabels =
        buildBookLabels(inv);
    auto weaponsMenu =
        Menu(&weaponLabels, &weaponSelected);
    auto potionsMenu =
        Menu(&potionLabels, &potionSelected);
    auto booksMenu =
        Menu(&bookLabels, &bookSelected);

    // BUTTONS

    auto equipButton = Button(
        "Equip Weapon",
        [&] {
            equipWeapon(inv, weaponSelected);
        }
    );
    auto usePotionButton = Button(
        "Use Potion",
        [&] {
            if(!inv.potions.empty())
            {
                usePotion(inv, potionSelected);
                potionLabels = buildPotionLabels(inv);
                if(potionSelected >= (int)potionLabels.size())
                {
                    potionSelected = 0;
                }
            }
        }
    );

    auto exitButton = Button(
        "Exit Inventory",
        screen.ExitLoopClosure()
    );

    // CONTAINER

    auto container = Container::Vertical({
        weaponsMenu,
        equipButton,
        potionsMenu,
        usePotionButton,
        booksMenu,
        exitButton
    });

    // RENDERER

    auto renderer = Renderer(container,[&]{
        return vbox({
            text("===================================") | bold,
            text("        INVENTORY") | bold,
            text("==================================="),
            separator(),
            text("Coins: " + to_string(inv.currency)),
            separator(),
            text("WEAPONS") | bold,
            weaponsMenu->Render(),
            separator(),
            inv.weapons.empty()
            ? text("Equipped: None")
            : text(
                "Equipped: " +
                inv.weapons[inv.equippedWeapon].name
              ),
            equipButton->Render(),
            separator(),
            text("POTIONS") | bold,
            potionsMenu->Render(),
            usePotionButton->Render(),
            separator(),
            text("BOOKS") | bold,
            booksMenu->Render(),
            separator(),
            exitButton->Render()
        }) | border;

    });

    // START UI

    screen.Loop(renderer);
    return 0;
}
