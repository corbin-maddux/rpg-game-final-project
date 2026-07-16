#pragma once

#include <string>
#include <vector>

#include "game.hpp"

// Gives the shared UI access to the one Player whose inventory should remain
// visible beside every screen.
void setUiPlayer(Player* player);

void showMessage(
    const std::string& title,
    const std::vector<std::string>& lines,
    const std::string& buttonText = "Continue"
);

int showMenu(
    const std::string& title,
    const std::vector<std::string>& lines,
    const std::vector<std::string>& choices
);
