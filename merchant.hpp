#pragma once

// This small header lets levels call the merchant without knowing how his UI
// and purchasing logic are implemented in merchant.cpp.

#include "combat.hpp"

// Call this function at the end of each level. Merrick introduces himself only
// on the first visit, but his shop opens every time.
// Opens Merrick's shop and modifies the same Player object when coins are spent
// or purchased items are inserted into the inventory.
void merchantEncounter(Player& player);
