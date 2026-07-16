#pragma once

// This header lists reusable screens implemented in main.cpp. Level files can
// call these functions without containing FTXUI rendering code themselves.

#include <string>
#include <vector>

#include "combat.hpp"

void displayStory(
    Player& player,
    // The title, dialogue lines, and button label are supplied by each level;
    // displayStory reads them to build one fixed-size FTXUI story window.
    const std::string& title,
    const std::vector<std::string>& lines,
    const std::string& buttonLabel
);

void displayCombatTip(
    Player& player,
    const std::string& enemyName,
    const std::string& tip
);

void displayItem(
    Player& player,
    const std::string& itemName,
    const std::string& description
);

// Runs an encounter on a copy of Enemy so its changing HP is local to this
// fight. It returns true only when the player wins and may continue the level.
bool enemyEncounter(Player& player, Enemy enemy);

// Displays a level entrance with a choice to continue or return to the menu.
bool levelEntrance(
    Player& player,
    const std::string& title,
    const std::string& description
);