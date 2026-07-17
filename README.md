# Cave of Shadows (Demo)
Cave of Shadows is a terminal-based role-playing game written in C++17. It uses the FTXUI library to render buttons, menus, fixed-size game windows, and an inventory panel that remains visible throughout the game within the terminal.

>[!NOTE]
>This beginner-friendly demo contains Level 0 and Level 1.

## Features
- Turn-based combat

- Melee and magic attacks

- Enemy weaknesses and resistances

- Speed-based turn order

- Player health, mana, damage, speed, and defense

- Coins and merchant purchases

- Experience and leveling

- Weapons, spell books, and potions

- Equippable Iron Sword

- Equippable Pyro Book

- Always-visible inventory panel

- Two complete levels

- Keyboard-controlled FTXUI interface

## Requirements
>[!NOTE]
>These steps are necessary to ensure that the game runs

### Before building the game, install:
- Git

- CMake 3.14 or newer

- A C++ compiler that supports C++17

- An internet connection for the first build

- CMake downloads FTXUI version 6.1.9 automatically.

## Downloading the Project

git clone ```<your-repository-url>```

```cd <your-repository-folder>/beginner-version```

Replace ```<your-repository-url>``` and ```<your-repository-folder>``` with the actual information for this repository.


## Building the Game
### Run these commands from inside the demo-version folder:
```bash
cmake -S . -B build
cmake --build build
```

### Running the Game
On Linux or GitHub Codespaces:

```bash
./build/game
```

### Rebuilding After Changing Code
After changing a .cpp or .hpp file, run:
```bash
cmake --build build
./build/game
```

>[!TIP]
>### If the build configuration becomes broken or outdated, perform a clean build:
>```bash
>rm -rf build
>cmake -S . -B build
>cmake --build build
>./build/game
>```


>[!NOTE]
>## Start of Guide
# Gameplay
The player explores two levels, fights enemies, collects coins and items, gains experience, levels up, and visits Merrick’s shop.

## Controls
Button           | Function
-----------------|---------
Control          | Action
Arrow keys / Tab | Move between buttons
Enter            | Activate the selected button
[                | Select the previous inventory item
]                | Select the next inventory item
U                | Use the selected potion
E                | Equip or unequip the selected weapon or book
Escape           | Return to the main menu

## Inventory
The inventory remains visible on the right side of every game screen.

### It displays:

-Item names

-Item quantities

-The selected item

-Equipped items

-Current weapon

-Health

-Mana

-Coins

-Level

-EXP progress

-Inventory messages

-Inventory controls

-Item Types

## Level 0: The First Chamber
Level 0 introduces the player to the story, inventory, melee combat, magic combat, enemy weaknesses, coins, items, leveling, and the merchant.

### The player begins with the following basic statistics:

- 100 Health

- 50 Mana

- 10 Damage

- 10 Magic Damage

- 10 Speed

- 5 Defense

The player begins with fists equipped. Fists have a -5 damage bonus, causing them to deal 5 base melee damage.

>[!NOTE]
>Equipment cannot be changed during combat.

## Iron Sword
The Iron Sword is automatically collected and equipped.

It provides:

+20 melee damage

-2 Speed

The player can select the sword in the inventory and press E to unequip it. Unequipping it returns the player to fists.

## Combat
During combat, the player can choose:

- Melee Attack

- Magic Attack

- Run Away

- Return to Main Menu

- Melee Damage

Melee damage begins with the player’s Damage plus the equipped weapon bonus.

The enemy’s melee multiplier is applied, and then the enemy’s Defense is subtracted.
```bash
damage = player.damage + player.weaponDamageBonus;
damage = static_cast<int>(damage * enemy.meleeMultiplier);
damage -= enemy.defense;
```
Every successful attack deals at least 1 damage.

### Magic Damage

Magic attacks cost 10 Mana.
```bash
player.mana -= 10;
damage = static_cast<int>(
    player.magicDamage * enemy.magicMultiplier
);
```

If the player does not have 10 Mana, the magic attack does not use the turn.

When the Pyro Book is equipped, the player’s Magic Attack becomes a Fireball with 20 base damage.

### Speed
The player’s effective Speed includes the equipped weapon’s Speed bonus.

```bash
int playerSpeed = player.speed + player.weaponSpeedBonus;
```

If the enemy has more Speed than the player, the enemy attacks before the player’s first turn.

### Running Away

The player can choose to flee the battle.

If the player fails to escape, the enemy attacks the player’s back for double base damage, and the encounter continues.

## Enemies Pt.1
### Dungeon Rat
The Dungeon Rat is the first basic enemy.

It introduces the normal combat system without special damage multipliers.

### Cave Slime

The Cave Slime teaches enemy weaknesses and resistances.

- Melee damage is multiplied by 0.5

- Magic damage is multiplied by 2.5

The Cave Slime drops a Healing Potion after it is defeated.


## First Level Up

The player receives enough experience to reach the first level-up requirement after completing Level 0.

The game displays a two-page tutorial explaining:

- Which statistics increase

- How experience is earned

- Where Level and EXP are displayed

## Level 1: The Hollow Halls

Level 1 introduces stronger enemies, Speed-based first attacks, and a detailed statistics tutorial.

## Enemies Pt.2
### Hollow Bat

The Hollow Bat is stronger than the Level 0 enemies but does not have special resistance multipliers.

### Stone Guard

The Stone Guard teaches the player to choose attacks based on enemy weaknesses.

- Melee damage is multiplied by 2.0

- Magic damage is multiplied by 0.5

- The guard has enough Speed to attack first

>[!TIP]
>Before the fight, the game displays a three-page explanation of:
>
>- Speed
>
>- Melee and magic damage
>
>- Health, Mana, potions, and survival
>
>Make sure to pay attention to this explanation because it gives you important information.


## The game uses three main item types:

- Weapon

- Potion

- Book

Items are stored in the player’s inventory using:

```bash
std::vector<Item> inventory;
```

Items with the same name are stacked by increasing their quantity.

## Potions

### Healing Potion

A Healing Potion restores Health to maximum.

```bash
player.health = player.maxHealth;
```
### Mana Potion

A Mana Potion restores Mana to maximum.
```bash
player.mana = player.maxMana;
```

Using a potion lowers its quantity by one. The item is removed from the inventory when its quantity reaches zero.

## Equipment

### Weapons

Equipping a weapon copies its Damage and Speed bonuses into the player.

Unequipping the current weapon returns the player to fists:

```bash
player.weaponName = "Fists";
player.weaponDamageBonus = -5;
player.weaponSpeedBonus = 0;
```

### Pyro Book for Dummies

Equipping the Pyro Book changes the player’s Magic Damage to 20 and unlocks the Fireball attack.

Unequipping it changes Magic Damage to zero.

>[!NOTE]
>Equipment cannot be changed during combat.

## Experience and Leveling

Enemies award experience when defeated.
```bash
grantExperience(player, enemy.experienceDrop);
```
When the player has enough EXP, processLevelUps performs a level-up.

A while loop allows a large EXP reward to cause more than one level-up.

### Each level provides:

- +2 Damage

- +2 Speed

- +2 Defense

- +2 Magic Damage

- +10 maximum Health

- Full Health restoration

>[!NOTE]
>The EXP requirement also increases after every level.

## Merrick’s Shop

Merrick appears at the end of both levels.

### He sells:

Item                  | Price
----------------------|----------
Mana Potion           | 15 coins
Pyro Book for Dummies | 100 coins
Axe of the Warrior    | 150 coins

>[!NOTE]
>The purchase is rejected if the player does not have enough coins.

>If the purchase succeeds, the price is subtracted and the item is added to the inventory.

>[!NOTE]
>## End of Guide

# Project Files
## main.cpp

Contains the program entry point.

### It:

- Creates the Player

- Opens the main menu

- Resets the Player when a new game starts

- Runs Level 0

- Runs Level 1

- Returns to the main menu after completion or defeat

## game.hpp

### Defines the shared data structures:

- Item

- Player

- Enemy

These structures store the information used by the other game systems.

## ui.hpp and ui.cpp

Contain the FTXUI interface.

### They:
- Create menus and buttons

- Display story messages

- Draw the fixed-size game window

- Draw the inventory panel

- Handle inventory controls

- Handle Escape

- Return the selected menu choice

The advanced FTXUI code is kept here so the gameplay files remain easier to understand.

## combat.hpp and combat.cpp

Contain the combat system.

### Important functions include:

- fightEnemy

- enemyTurn

### The combat system handles:

- Player attacks

- Enemy attacks

- Mana costs

- Damage multipliers

- Defense

- Speed

- Running

- Victory

- Defeat

- Coin rewards

- EXP rewards

## inventory.hpp and inventory.cpp

Contain the inventory system.

### Important functions include:

- addItem

- selectPreviousItem

- selectNextItem

- useSelectedPotion

- equipSelectedItem

- showInventory

- inventoryMenu

## leveling.hpp and leveling.cpp

Contain the experience and leveling system.

### Important functions include:

- grantExperience

- processLevelUps

- showFirstLevelUpTutorial

## merchant.hpp and merchant.cpp

Contain Merrick’s shop.

### The merchant function:

- Displays the shop

- Reads the selected purchase

- Calculates the price

- Checks the player’s coins

- Subtracts the cost

- Adds the purchased item

## levels.hpp

### Declares:

- levelZero

- levelOne

- This allows main.cpp to call both level functions.

## level_zero.cpp

Contains the complete Level 0 sequence.

## level_one.cpp

Contains the complete Level 1 sequence.

## CMakeLists.txt

Configures the project.

### It:

- Requires CMake 3.14

- Uses C++17

- Downloads FTXUI 6.1.9

- Compiles every .cpp file

- Creates the game executable

- Links the required FTXUI libraries

# Important Functions

## showMenu

```bash
int showMenu(
    const std::string& title,
    const std::vector<std::string>& lines,
    const std::vector<std::string>& choices
);
```

Creates an FTXUI menu and returns the index of the selected choice.

## showMessage
```bash
void showMessage(
    const std::string& title,
    const std::vector<std::string>& lines,
    const std::string& buttonText = "Continue"
);
```

Creates a message screen with one button by calling showMenu.

## fightEnemy
```bash
int fightEnemy(Player& player, Enemy enemy);
```
Runs an entire enemy encounter.

### It returns:

- 1 for victory

- 0 for defeat

- -1 for returning to the main menu

## addItem
```bash
void addItem(Player& player, Item item);
```
Adds an item to the inventory. If the item already exists, its quantity is increased.

## processLevelUps
```bash
int processLevelUps(Player& player);
```
Checks the player’s EXP and applies every available level-up. It returns the number of levels gained.

## merchant
```bash
void merchant(Player& player);
```
Runs Merrick’s shop and changes the player’s Coins and Inventory when a purchase succeeds.

# Troubleshooting

## FTXUI header not found

### If this appears:
```
fatal error: ftxui/component/component.hpp: No such file or directory
```

Do not compile only main.cpp with g++. Build through CMake so FTXUI is downloaded and linked.

## Source file not found

If CMake cannot find a .cpp file, make sure its name in CMakeLists.txt exactly matches the real filename.

## Multiple definition

This means the same function was implemented in more than one .cpp file.

Keep one implementation and place only the function declaration in the matching header.

## Undefined reference

### This normally means:

- A function was declared but not implemented

- The implementation has a different name or parameter list

- The required .cpp file is missing from CMakeLists.txt

## CMake parse error

Make sure every CMake function has a closing parenthesis.

# Possible Future Improvements

## The project can be expanded by adding:

- More levels

- More enemies

- More weapons

- Additional spell books

- More merchants or shop items

- Saving and loading

- Elemental damage

- Random item drops

- A final boss

- Additional story dialogue

>[!NOTE]
>The project separates the interface, combat, inventory, leveling, merchant, and level code so these additions can be made without rewriting the entire game.
