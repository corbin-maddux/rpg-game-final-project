#include "merchant.hpp"

#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "inventory.hpp"
#include "inventory_ui.hpp"

using namespace std;
using namespace ftxui;

// This file builds Merrick's reusable shop screen and transfers purchased
// InventoryItems into the persistent Player inventory.

namespace {

ButtonOption merchantButtonStyle(Player& player, int price) {
    // price is the cost attached to this particular shop button.
    ButtonOption option = ButtonOption::Simple();

    option.transform = [&player, price](const EntryState& state) {
        Element label = text(state.label);

        // Dim unaffordable choices while still allowing a click to explain how
        // many more coins the player needs.
        if (player.coins < price) {
            label |= dim;
        }

        if (state.focused) {
            label |= inverted;
        }

        // Return the fully styled label that FTXUI draws for this button.
        return label | border;
    };

    return option;
}

} // namespace

void merchantEncounter(Player& player) {
    // auto asks C++ to infer ScreenInteractive's long type from the right side.
    auto screen = ScreenInteractive::FitComponent();

    // This text variable changes after introductions, purchases, and failures.
    string merchantMessage;

    // Use the longer introduction once; later visits use the shorter else text.
    if (!player.merchantIntroduced) {
        merchantMessage =
            "A lantern flares to life beside a wooden counter. The man behind "
            "it straightens his patched red coat. \"Name's Merrick. I follow "
            "the doors, and the doors lead me to people with coins. Lucky for "
            "you, I have something you might need.\"";
        player.merchantIntroduced = true;
    } else {
        merchantMessage =
            "Merrick: Back again? Let's see how many coins survived the trip.";
    }

    // This local function-like lambda shares the same payment logic among all
    // shop buttons. [&] lets it update player and merchantMessage directly.
    auto buyItem = [&](const InventoryItem& item, int price) {
        if (player.coins < price) {
            merchantMessage =
                "Merrick: You're " + to_string(price - player.coins) +
                " coins short for that one.";
            // false tells a button callback that no purchase occurred.
            return false;
        }

        player.coins -= price;
        player.inventory.addItem(item);
        merchantMessage =
            "Purchased " + item.name + " for " + to_string(price) +
            " coins.";
        // true allows an item-specific success message after payment succeeds.
        return true;
    };

    auto manaPotionButton = Button(
        "Mana Restoration Potion - 15 coins",
        [&] {
            buyItem(
                {
                    "Mana Restoration Potion",
                    "Fully restores mana. Select it and press U to drink it.",
                    ItemType::Potion,
                    1,
                    0,
                    false,
                    true
                },
                15
            );
        },
        merchantButtonStyle(player, 15)
    );

    auto pyroBookButton = Button(
        "Pyro Book for Dummies - 100 coins",
        [&] {
            // Remember whether payment succeeded before showing book dialogue.
            bool purchased = buyItem(
                {
                    "Pyro Book for Dummies",
                    "An equippable magic weapon. Press E outside combat to "
                    "turn your magic into a 20-damage Fireball attack.",
                    ItemType::Book,
                    1,
                    0
                },
                100
            );

            if (purchased) {
                merchantMessage =
                    "Merrick lowers his voice. \"Keep your eyebrows away from "
                    "chapter three. Select the book and press E when you're "
                    "ready to equip it.\"";
            }
        },
        merchantButtonStyle(player, 100)
    );

    auto warriorAxeButton = Button(
        "Axe of the Warrior - 150 coins",
        [&] {
            buyItem(
                {
                    "Axe of the Warrior",
                    "A heavy battle axe. Adds 35 physical damage but reduces "
                    "speed by 5. Select it and press E to equip it.",
                    ItemType::Weapon,
                    1,
                    0,
                    false,
                    false,
                    35,
                    -5
                },
                150
            );
        },
        merchantButtonStyle(player, 150)
    );

    auto leaveButton = Button("Leave Shop", [&] {
        screen.Exit();
    });

    auto container = Container::Vertical({
        manaPotionButton,
        pyroBookButton,
        warriorAxeButton,
        leaveButton
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            hbox({
                text("MERRICK'S SHOP") | bold,
                filler(),
                text("Coins: " + to_string(player.coins)) | bold
            }),
            separator(),
            paragraph(merchantMessage),
            separator(),
            manaPotionButton->Render(),
            pyroBookButton->Render(),
            warriorAxeButton->Render(),
            separator(),
            leaveButton->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    // Loop keeps processing keyboard/button events until Leave or Escape exits.
    screen.Loop(renderer);
}