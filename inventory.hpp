#pragma once

#include "game.hpp"

void addItem(Player& player, Item item);
void showInventory(Player& player);
void inventoryMenu(Player& player, bool inCombat);

void selectPreviousItem(Player& player);
void selectNextItem(Player& player);
void useSelectedPotion(Player& player);
void equipSelectedItem(Player& player);
