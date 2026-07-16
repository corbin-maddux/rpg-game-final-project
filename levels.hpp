#pragma once

// Each level has its own .cpp file. These declarations make both level
// functions available to main.cpp and to one another.

#include "combat.hpp"

// Runs Level 0 using the supplied persistent player, then returns when the level
// ends, the player loses, or the user requests the main menu.
void levelZero(Player& player);
void levelOne(Player& player);
