#include "ui.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "inventory.hpp"

using namespace ftxui;

namespace {

Player* uiPlayer = nullptr;

Element drawInventory() {
    Elements rows = {
        text("INVENTORY") | bold | center,
        separator()
    };

    if (uiPlayer == nullptr || uiPlayer->inventory.empty()) {
        rows.push_back(text("(Empty)") | dim);
    } else {
        if (uiPlayer->selectedItem < 0 ||
            uiPlayer->selectedItem >= static_cast<int>(uiPlayer->inventory.size())) {
            uiPlayer->selectedItem = 0;
        }

        for (int i = 0; i < static_cast<int>(uiPlayer->inventory.size()); i++) {
            const Item& item = uiPlayer->inventory[i];
            std::string label = item.name;
            label += " x" + std::to_string(item.quantity);

            if (item.name == uiPlayer->weaponName) {
                label += " [Equipped]";
            }

            if (item.name == "Pyro Book for Dummies" &&
                uiPlayer->pyroBookEquipped) {
                label += " [Equipped]";
            }

            if (i == uiPlayer->selectedItem) {
                label = "> " + label;
            } else {
                label = "  " + label;
            }

            rows.push_back(paragraph(label));
        }

        const Item& selected = uiPlayer->inventory[uiPlayer->selectedItem];
        rows.push_back(separator());
        rows.push_back(text("SELECTED") | bold);
        rows.push_back(paragraph(selected.name));
        rows.push_back(text("Type: " + selected.type) | dim);
    }

    if (uiPlayer != nullptr) {
        rows.push_back(filler());
        rows.push_back(separator());
        rows.push_back(text("Weapon: " + uiPlayer->weaponName));
        rows.push_back(text(
            "HP: " + std::to_string(uiPlayer->health) +
            "/" + std::to_string(uiPlayer->maxHealth)
        ));
        rows.push_back(text(
            "Mana: " + std::to_string(uiPlayer->mana) +
            "/" + std::to_string(uiPlayer->maxMana)
        ));
        rows.push_back(text("Coins: " + std::to_string(uiPlayer->coins)));

        if (!uiPlayer->inventoryMessage.empty()) {
            rows.push_back(separator());
            rows.push_back(paragraph(uiPlayer->inventoryMessage));
        }

        rows.push_back(separator());
        rows.push_back(text("[ / ]  Select item") | dim);
        rows.push_back(text("U      Use potion") | dim);
        rows.push_back(text("E      Equip/unequip") | dim);
    }

    return vbox(rows) |
           size(WIDTH, EQUAL, 34) |
           size(HEIGHT, EQUAL, 27) |
           border;
}

} // namespace

void setUiPlayer(Player* player) {
    uiPlayer = player;
}

int showMenu(
    const std::string& title,
    const std::vector<std::string>& lines,
    const std::vector<std::string>& choices
) {
    auto screen = ScreenInteractive::FitComponent();
    int selectedChoice = -1;
    std::vector<Component> buttons;

    for (int i = 0; i < static_cast<int>(choices.size()); i++) {
        buttons.push_back(Button(choices[i], [&, i] {
            selectedChoice = i;
            screen.Exit();
        }));
    }

    auto buttonContainer = Container::Vertical(buttons);

    auto renderer = Renderer(buttonContainer, [&] {
        Elements textLines;

        for (const std::string& line : lines) {
            textLines.push_back(paragraph(line));
        }

        Element mainWindow = vbox({
            text(title) | bold | center,
            separator(),
            vbox(textLines),
            filler(),
            separator(),
            buttonContainer->Render()
        }) |
               size(WIDTH, EQUAL, 80) |
               size(HEIGHT, EQUAL, 24) |
               border;

        Element gameColumn = vbox({
            mainWindow,
            hbox({
                text(" Esc  Return to Main Menu ") | bold | border,
                filler()
            })
        });

        return hbox({
            gameColumn,
            drawInventory()
        });
    });

    auto controls = CatchEvent(renderer, [&](Event event) {
        if (uiPlayer == nullptr) {
            return false;
        }

        if (event == Event::Escape) {
            uiPlayer->returnToMainMenu = true;
            selectedChoice = -2;
            screen.Exit();
            return true;
        }

        if (event == Event::Character('[')) {
            selectPreviousItem(*uiPlayer);
            return true;
        }

        if (event == Event::Character(']')) {
            selectNextItem(*uiPlayer);
            return true;
        }

        if (event == Event::Character('u') ||
            event == Event::Character('U')) {
            useSelectedPotion(*uiPlayer);
            return true;
        }

        if (event == Event::Character('e') ||
            event == Event::Character('E')) {
            equipSelectedItem(*uiPlayer);
            return true;
        }

        return false;
    });

    screen.Loop(controls);
    return selectedChoice;
}

void showMessage(
    const std::string& title,
    const std::vector<std::string>& lines,
    const std::string& buttonText
) {
    showMenu(title, lines, {buttonText});
}