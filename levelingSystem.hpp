#pragma once

#include "combat.hpp"

// Grant `amount` EXP to the player and run the level-up check.
inline void grantEXP(Player& player, int amount) {
	player.playerEXP += amount;
	// run level up processing immediately
	// call the inline levelSystem below
	(void) [&]() {
		while (player.playerEXP >= player.levelUpRequirement) {
			player.playerLevel += 1;
			player.levelUpModifier += 2;

			player.stats.damage += 2;
			player.stats.speed += 2;
			player.stats.defense += 2;
			player.stats.magicAttack += 2;

			player.stats.maxHealth += 10;
			player.stats.health = player.stats.maxHealth;

			player.levelUpRequirement = player.levelUpRequirement + 15 + player.levelUpModifier;
		}
		return player.playerLevel;
	}();
}

// Ensure the player meets level requirements and apply level-ups.
// Returns the player's current level after processing.
inline int levelSystem(Player& player) {
	int gained = 0;
	while (player.playerEXP >= player.levelUpRequirement) {
		player.playerLevel += 1;
		gained += 1;
		player.levelUpModifier += 2;

		player.stats.damage += 2;
		player.stats.speed += 2;
		player.stats.defense += 2;
		player.stats.magicAttack += 2;

		player.stats.maxHealth += 10;
		player.stats.health = player.stats.maxHealth;

		player.levelUpRequirement = player.levelUpRequirement + 15 + player.levelUpModifier;
	}
	return player.playerLevel;
}