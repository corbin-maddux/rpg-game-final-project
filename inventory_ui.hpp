#pragma once

// This header exposes the persistent inventory UI used beside every screen.

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "combat.hpp"

// These fixed dimensions keep dialogue length from resizing the game window.
// inline allows both main.cpp and merchant.cpp to use the same constants.
inline const int gameWindowWidth = 78;
inline const int gameWindowHeight = 24;
inline const int inventoryPanelWidth = 34;

ftxui::Element inventoryPanel(const Player& player);
ftxui::Component withInventory(
    Player& player,
    ftxui::Component gameScreen,
    ftxui::ScreenInteractive& screen,
    // The main menu passes false; gameplay screens use true so the shared
    // Escape-key return box remains visible below the left panel.
    bool showMenuShortcut = true
);