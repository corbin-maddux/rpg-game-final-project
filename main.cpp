#include <string>
#include <vector>

// main.cpp owns the reusable FTXUI screens and the program's main menu. Level
// story content is kept in level_zero.cpp and level_one.cpp to reduce crowding.

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "combat.hpp"
#include "game_ui.hpp"
#include "inventory.hpp"
#include "inventory_ui.hpp"
#include "levels.hpp"
#include "merchant.hpp"

// These lines allow string, vector, Button, text, and other frequently used
// names to be written without their std:: or ftxui:: prefixes in this file.
using namespace std;
using namespace ftxui;

// Screen and story functions stay here. Combat data and calculations live in
// combat.hpp/combat.cpp.
// These declarations let main call functions whose full definitions appear
// later in this file. Player& keeps every screen connected to one player state.
void mainMenu(Player& player);
void startGame(Player& player);

void displayStory(
    Player& player,
    const string& title,
    const vector<string>& lines,
    const string& buttonLabel
);

void displayCombatTip(
    Player& player,
    const string& enemyName,
    const string& tip
);

void displayItem(
    Player& player,
    const string& itemName,
    const string& description
);

bool enemyEncounter(Player& player, Enemy enemy);

void displayReward(
    Player& player,
    const Enemy& enemy
);

void displayDefeat(Player& player, const Enemy& enemy);
void displayCoinExplanation(Player& player);
Element coinTracker(const Player& player);

int main() {
    // Create the one Player object whose stats, inventory, and coins persist
    // while moving through menus, levels, encounters, and merchant screens.
    Player player;
    mainMenu(player);
    // Returning 0 tells the operating system that the game closed successfully.
    return 0;
}

Element coinTracker(const Player& player) {
    // Hide the tracker until at least one coin has actually been collected.
    if (player.coins == 0) {
        return text("");
    }

    // Build the exact FTXUI text element reused in each screen's top-right area.
    Element tracker = text("Coins: " + to_string(player.coins));

    if (player.highlightCoins) {
        return tracker | color(Color::White) | bold;
    }

    return tracker | dim;
}

Element inventoryPanel(const Player& player) {
    // Elements is the ordered collection of rows that becomes the right panel.
    Elements itemLines = {
        text("INVENTORY") | bold | center,
        separator()
    };

    // Read the real inventory list without copying or modifying it.
    const vector<InventoryItem>& items = player.inventory.getItems();
    const InventoryItem* selected = player.inventory.getSelectedItem();

    // Choose between an empty label and a rendered row for every owned item.
    if (items.empty()) {
        itemLines.push_back(text("(Empty)") | dim);
    } else {
        // Visit each stored item once to create its visible inventory row.
        for (const InventoryItem& item : items) {
            // Mark only the object whose address matches the selected object.
            string marker = (&item == selected) ? "> " : "  ";
            string equipped =
                item.name == "Pyro Book for Dummies" &&
                player.pyroBookEquipped
                    ? " [Equipped]"
                    : "";

            if (item.type == ItemType::Weapon &&
                player.weapon.name == item.name) {
                equipped = " [Equipped]";
            }
            itemLines.push_back(
                text(
                    marker + item.name +
                    " x" + to_string(item.quantity) + equipped
                )
            );
        }

        itemLines.push_back(separator());
        itemLines.push_back(text("INSPECT") | bold);
        itemLines.push_back(text(selected->name) | bold);
        itemLines.push_back(text(itemTypeName(selected->type)) | dim);
        itemLines.push_back(paragraph(selected->description));

        if (selected->name == "Pyro Book for Dummies") {
            itemLines.push_back(text(
                player.pyroBookEquipped
                    ? "Magic attack: 20 | Equipped"
                    : "Magic attack: 0 | Unequipped"
            ) | (player.pyroBookEquipped ? bold : dim));
        }

        if (selected->type == ItemType::Weapon) {
            int totalDamage =
                player.stats.damage + selected->weaponDamageBonus;

            itemLines.push_back(text(
                "Melee damage when equipped: " +
                to_string(totalDamage)
            ));
        }
    }

    if (!player.inventoryMessage.empty()) {
        itemLines.push_back(separator());
        itemLines.push_back(paragraph(player.inventoryMessage));
    }

    itemLines.push_back(filler());
    itemLines.push_back(separator());
    itemLines.push_back(text("[ / ]  Inspect items") | dim);
    itemLines.push_back(text("U      Use selected potion") | dim);
    itemLines.push_back(text("E      Equip/unequip gear") | dim);

    return vbox(itemLines) |
           size(WIDTH, EQUAL, inventoryPanelWidth) |
           size(HEIGHT, EQUAL, gameWindowHeight) |
           border;
}

Component withInventory(
    Player& player,
    Component gameScreen,
    ScreenInteractive& screen,
    bool showMenuShortcut
) {
    // Build a temporary screen from level-provided text and keep it open until
    // its custom-labeled continuation button or Escape closes it.
    // auto infers the FTXUI Component type. This renderer permanently joins the
    // current game screen, inventory panel, and optional menu shortcut box.
    auto layout = Renderer(gameScreen, [&player, gameScreen, showMenuShortcut] {
        Element gameColumn = gameScreen->Render();

        // Gameplay screens add the Escape box; the main menu deliberately does not.
        if (showMenuShortcut) {
            gameColumn = vbox({
                gameColumn,
                hbox({
                    text(" Esc  Return to Main Menu ") | bold | border,
                    filler()
                })
            });
        }

        return hbox({
            gameColumn,
            inventoryPanel(player)
        });
    });

    // Wrap the visual layout with shared keyboard handling, then return that
    // interactive component to the current ScreenInteractive loop.
    return CatchEvent(layout, [&player, &screen, showMenuShortcut](Event event) {
        if (showMenuShortcut && event == Event::Escape) {
            player.returnToMainMenu = true;
            screen.Exit();
            return true;
        }

        // [ and ] move the inventory cursor without interrupting the left screen.
        if (event == Event::Character('[')) {
            player.inventory.selectPrevious();
            return true;
        }

        if (event == Event::Character(']')) {
            player.inventory.selectNext();
            return true;
        }

        // E manages both equipment slots, but combat locks equipment changes.
        if (event == Event::Character('e') ||
            event == Event::Character('E')) {
            if (player.inCombat) {
                player.inventoryMessage =
                    "You cannot change equipment during a fight.";
                return true;
            }

            const InventoryItem* selected =
                player.inventory.getSelectedItem();

            if (selected == nullptr) {
                player.inventoryMessage = "There is nothing to equip.";
                return true;
            }

            // The Pyro Book toggles the magic slot and directly changes Magic Attack.
            if (selected->name == "Pyro Book for Dummies") {
                if (player.pyroBookEquipped) {
                    player.pyroBookEquipped = false;
                    player.stats.magicAttack = 0;
                    player.inventoryMessage =
                        "You close the Pyro Book. Magic attack returned to 0.";
                } else {
                    player.pyroBookEquipped = true;
                    player.stats.magicAttack = 20;
                    player.inventoryMessage =
                        "You equip the Pyro Book. Your magic now becomes Fireball!";
                }

                return true;
            }

            // Any present or future Weapon item uses its stored bonuses here.
            if (selected->type == ItemType::Weapon) {
                // Selecting the active weapon unequips it and restores 5-damage fists.
                if (player.weapon.name == selected->name) {
                    player.weapon = Weapon{"Fists", -5, 0, 0};
                    player.inventoryMessage =
                        "You unequip " + selected->name +
                        ". Fists equipped. Melee damage: 5.";
                } else {
                    player.weapon = Weapon{
                        selected->name,
                        selected->weaponDamageBonus,
                        selected->weaponSpeedBonus,
                        0
                    };

                    player.inventoryMessage =
                        "You equip " + selected->name +
                        ". Melee damage: " +
                        to_string(
                            player.stats.damage +
                            selected->weaponDamageBonus
                        ) + ".";
                }

                return true;
            }

            player.inventoryMessage =
                selected->name + " is not equippable.";

            return true;
        }

        // U consumes the selected potion only when it has a useful valid effect.
        if (event == Event::Character('u') ||
            event == Event::Character('U')) {
            const InventoryItem* selected =
                player.inventory.getSelectedItem();

            if (selected == nullptr) {
                player.inventoryMessage = "There is nothing to use.";
                return true;
            }

            if (selected->type != ItemType::Potion) {
                player.inventoryMessage =
                    selected->name + " cannot be used here.";
                return true;
            }

            if (!selected->restoreFullHealth &&
                !selected->restoreFullMana &&
                selected->manaRestore <= 0) {
                player.inventoryMessage =
                    selected->name + " has no usable effect yet.";
                return true;
            }

            if (selected->restoreFullHealth &&
                player.stats.health >= player.stats.maxHealth) {
                player.inventoryMessage = "Your health is already full.";
                return true;
            }

            if (selected->restoreFullMana &&
                player.stats.mana >= player.stats.maxMana) {
                player.inventoryMessage = "Your mana is already full.";
                return true;
            }

            InventoryItem usedItem;

            if (player.inventory.consumeSelectedItem(usedItem)) {
                if (usedItem.restoreFullHealth) {
                    player.stats.health = player.stats.maxHealth;
                }

                if (usedItem.restoreFullMana) {
                    player.stats.mana = player.stats.maxMana;
                } else {
                    player.stats.mana += usedItem.manaRestore;
                }

                if (player.stats.mana > player.stats.maxMana) {
                    player.stats.mana = player.stats.maxMana;
                }

                if (usedItem.restoreFullHealth) {
                    player.inventoryMessage =
                        "Used " + usedItem.name + ". Health fully restored.";
                } else if (usedItem.restoreFullMana) {
                    player.inventoryMessage =
                        "Used " + usedItem.name + ". Mana fully restored.";
                } else {
                    player.inventoryMessage =
                        "Used " + usedItem.name + ". Mana restored: " +
                        to_string(usedItem.manaRestore) + ".";
                }
            }

            return true;
        }

        return false;
    });
}

void mainMenu(Player& player) {
    // This flag controls the menu's lifetime; only the Quit button makes it true.
    bool quitSelected = false;

    // Rebuild and show the menu again whenever a level returns through Escape.
    while (!quitSelected) {
        auto screen = ScreenInteractive::FitComponent();
        bool startSelected = false;

        // [&] lets this button callback change local flags and exit this screen.
        auto startButton = Button("Start Game", [&] {
            startSelected = true;
            screen.Exit();
        });

        auto quitButton = Button("Quit", [&] {
            quitSelected = true;
            screen.Exit();
        });

    auto menuContainer = Container::Vertical({
        startButton,
        quitButton
    });

    auto renderer = Renderer(menuContainer, [&] {
        return vbox({
            text("================================="),
            text("          CAVE OF SHADOWS"),
            text("================================="),
            separator(),
            menuContainer->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

        screen.Loop(withInventory(player, renderer, screen, false));

        if (startSelected) {
            // Replace old progress with a fresh default Player for each new run.
            player = Player{};
            startGame(player);
            player.returnToMainMenu = false;
        }
    }
}

void startGame(Player& player) {
    // This screen presents the prologue before Level 0 begins.
    auto screen = ScreenInteractive::FitComponent();
    bool enterDungeon = false;

    auto enterButton = Button("Open Your Eyes", [&] {
        enterDungeon = true;
        screen.Exit();
    });

    auto container = Container::Vertical({enterButton});

    auto renderer = Renderer(container, [&] {
        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text("PROLOGUE: THE WAKING DARK") | bold,
            separator(),
            paragraph(
                "Cold stone presses against your cheek. When you open your "
                "eyes, the room is almost completely dark. Water drips "
                "somewhere nearby, slow and steady, and every drop echoes "
                "through a hallway you cannot see. You sit up carefully. "
                "Your head aches, your pockets are empty, and you cannot "
                "remember how you got here."
            ),
            paragraph(
                "A weak blue light appears on the back of your hand. It forms "
                "a broken circle crossed by a thin line. Before you can touch "
                "it, something scratches behind the wall. Then a tired voice "
                "speaks through a narrow crack in the stone."
            ),
            text("\"If you're awake, say something. I can't see you from here.\"") |
                dim,
            separator(),
            container->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));

    // Continue only when the story button—not the global Escape shortcut—closed it.
    if (enterDungeon && !player.returnToMainMenu) {
        levelZero(player);
    }
}

void displayStory(
    Player& player,
    const string& title,
    const vector<string>& lines,
    const string& buttonLabel
) {
    auto screen = ScreenInteractive::FitComponent();

    auto continueButton = Button(buttonLabel, [&] {
        screen.Exit();
    });

    auto container = Container::Vertical({continueButton});

    auto renderer = Renderer(container, [&] {
        Elements storyLines;

        // Turn each supplied paragraph string into a wrapping FTXUI paragraph.
        for (const string& line : lines) {
            storyLines.push_back(paragraph(line));
        }

        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text(title) | bold,
            separator(),
            vbox(storyLines),
            separator(),
            container->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));
}

bool levelEntrance(
    Player& player,
    const string& title,
    const string& description
) {
    // The returned bool tells the level whether Enter Level was chosen instead
    // of the shared Escape shortcut.
    auto screen = ScreenInteractive::FitComponent();
    bool continueLevel = false;

    auto continueButton = Button("Enter Level", [&] {
        continueLevel = true;
        screen.Exit();
    });

    auto container = Container::Vertical({continueButton});
    auto renderer = Renderer(container, [&] {
        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text(title) | bold,
            separator(),
            paragraph(description),
            filler(),
            separator(),
            container->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));
    return continueLevel && !player.returnToMainMenu;
}

void displayCombatTip(
    Player& player,
    const string& enemyName,
    const string& tip
) {
    // Present an enemy-specific weakness/resistance lesson before combat starts.
    auto screen = ScreenInteractive::FitComponent();

    auto continueButton = Button("Got It", [&] {
        screen.Exit();
    });

    auto container = Container::Vertical({continueButton});

    auto renderer = Renderer(container, [&] {
        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text("COMBAT TIP: " + enemyName) | bold | color(Color::Yellow),
            separator(),
            paragraph(tip),
            separator(),
            container->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));
}

void displayItem(
    Player& player,
    const string& itemName,
    const string& description
) {
    // Show discovery text before the calling level inserts the item into Inventory.
    auto screen = ScreenInteractive::FitComponent();

    auto collectButton = Button("Collect", [&] {
        screen.Exit();
    });

    auto container = Container::Vertical({collectButton});

    auto renderer = Renderer(container, [&] {
        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text("ITEM FOUND!") | bold,
            separator(),
            text(itemName) | bold,
            paragraph(description),
            separator(),
            container->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));
}

// Returns true when the enemy is defeated and false when the player loses.
// The UI is handled here; combat.hpp/combat.cpp handle the combat rules.
bool enemyEncounter(Player& player, Enemy enemy) {
    // Enemy is passed by value, so reducing this fight's HP does not alter the
    // original enemy template created in its level file.
    auto screen = ScreenInteractive::FitComponent();
    player.inCombat = true;

    // This changing string is redrawn after every player and enemy action.
    string combatMessage = "The creature notices you and moves closer.";
    bool playerWon = false;
    bool encounterFinished = false;

    // Effective speed includes the equipped weapon's bonus or penalty.
    const int playerSpeed =
        player.stats.speed + player.weapon.speedBonus;

    // A combatant with greater speed acts first. The player's button press is
    // still their first chosen action when their speed is tied or higher.
    // A strictly faster enemy receives one opening attack before button input.
    if (enemy.stats.speed > playerSpeed) {
        AttackResult openingAttack = enemyAttack(enemy, player);

        combatMessage =
            enemy.name + " is faster and attacks first!\n" +
            enemy.name + ": " + openingAttack.message;

        if (isDefeated(player.stats)) {
            encounterFinished = true;
        }

        // Pause for the three contextual tutorial pages the first time this rule
        // occurs, then remember not to repeat them in later fast encounters.
        if (!player.statsExplanationShown) {
            displayStory(
                player,
                "STATS GUIDE 1/3: SPEED",
                {
                    "The " + enemy.name + " attacked before you because its "
                    "Speed is " + to_string(enemy.stats.speed) + ", while your "
                    "current Speed is " + to_string(playerSpeed) + ".",
                    "The combatant with the higher Speed acts first. Weapon "
                    "bonuses or penalties can change your effective Speed. "
                    "Your Iron Sword, for example, lowers Speed by 2.",
                    "This is why your attack comes second in this encounter. "
                    "After the opening strike, combat continues in normal turns."
                },
                "Next Page"
            );

            if (player.returnToMainMenu) {
                player.inCombat = false;
                return false;
            }

            displayStory(
                player,
                "STATS GUIDE 2/3: ATTACKS",
                {
                    "Physical damage starts with your Damage stat plus the bonus "
                    "from your equipped melee weapon. Enemy weaknesses and "
                    "resistances multiply that amount before Defense is removed.",
                    "Magic damage starts with Magic Attack and uses the enemy's "
                    "magic multiplier. Ordinary Defense does not reduce magic, "
                    "but resistant enemies can cut the spell's damage.",
                    "Slimes take extra magic damage. Guards take double melee "
                    "damage but only half magic damage, so changing tactics is "
                    "more effective than repeating the same attack."
                },
                "Next Page"
            );

            if (player.returnToMainMenu) {
                player.inCombat = false;
                return false;
            }

            displayStory(
                player,
                "STATS GUIDE 3/3: SURVIVAL",
                {
                    "Health shows how much damage you can survive. Defense lowers "
                    "incoming physical damage, while Mana is spent whenever you "
                    "cast a magic attack.",
                    "Weapons can raise Damage while lowering Speed. Magic gear can "
                    "change Magic Attack, and potions can restore Health or Mana "
                    "when selected from the inventory.",
                    "Check the enemy tip, compare your current stats, and choose "
                    "the attack that matches its weakness. The Stone Guard is "
                    "fast, but your Iron Sword can break it quickly."
                },
                "Return to Fight"
            );

            player.statsExplanationShown = true;

            if (player.returnToMainMenu) {
                player.inCombat = false;
                return false;
            }
        }
    }

    // Every valid player action calls this to check victory, run the
    // enemy response, check defeat, and update the active combat screen.
    auto finishTurn = [&] {
        if (isDefeated(enemy.stats)) {
            playerWon = true;
            encounterFinished = true;
            screen.Exit();
            return;
        }

        AttackResult enemyResult = enemyAttack(enemy, player);
        combatMessage += "\n" + enemy.name + ": " + enemyResult.message;

        if (isDefeated(player.stats)) {
            encounterFinished = true;
            screen.Exit();
        }
    };

    // Physical Attack always consumes the player's turn.
    auto attackButton = Button("Attack", [&] {
        AttackResult result = playerAttack(player, enemy);
        combatMessage = "You: " + result.message;
        finishTurn();
    });

    // The same button calls magic calculations, but its label becomes Fireball
    // while the Pyro Book is equipped.
    auto magicButton = Button(
        player.pyroBookEquipped ? "Fireball" : "Magic",
        [&] {
        AttackResult result = playerMagicAttack(player, enemy);
        combatMessage = "You: " + result.message;

        // A failed spell, such as insufficient mana, does not consume a turn.
        if (result.turnUsed) {
            finishTurn();
        }
    });

    // Running never exits combat; it gives the enemy a doubled back attack.
    auto runButton = Button("Run Away", [&] {
        AttackResult result = enemyBackAttack(enemy, player);

        combatMessage =
            "You turn your back and make a run for it.\n" +
            enemy.name + ": " + result.message + "\n" +
            "\"Coward. That's what you get for trying to run.\"";

        if (isDefeated(player.stats)) {
            encounterFinished = true;
            screen.Exit();
        }
    });

    auto container = Container::Vertical({
        attackButton,
        magicButton,
        runButton
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text("ENEMY ENCOUNTER") | bold,
            separator(),
            text(enemy.name) | bold,
            text(
                "Enemy HP: " + to_string(enemy.stats.health) +
                "/" + to_string(enemy.stats.maxHealth) +
                " | Your HP: " + to_string(player.stats.health) +
                "/" + to_string(player.stats.maxHealth) +
                " | Mana: " + to_string(player.stats.mana) +
                "/" + to_string(player.stats.maxMana)
            ),
            separator(),
            paragraph(combatMessage),
            separator(),
            container->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));
    player.inCombat = false;

    // Escape closes the screen without victory or defeat, signaling the level
    // to unwind to the main menu.
    if (!encounterFinished) {
        return false;
    }

    if (!playerWon) {
        displayDefeat(player, enemy);
        return false;
    }

    // Remember whether this reward is the game's first coin pickup so the UI can
    // highlight the tracker and show its one-time explanation.
    bool firstCoinDrop = player.coins == 0 && enemy.coinDrop > 0;
    player.coins += enemy.coinDrop;

    if (enemy.extraDrop == "Healing Potion") {
        player.inventory.addItem({
            "Healing Potion",
            "A sealed red potion. Fully restores health when used.",
            ItemType::Potion,
            1,
            0,
            true,
            false
        });
    }

    if (firstCoinDrop) {
        player.highlightCoins = true;
    }

    displayReward(player, enemy);

    if (player.returnToMainMenu) {
        return false;
    }

    if (!player.coinExplanationShown && enemy.coinDrop > 0) {
        displayCoinExplanation(player);
        if (player.returnToMainMenu) {
            return false;
        }
        player.coinExplanationShown = true;
        player.highlightCoins = false;
    }

    return true;
}

void displayReward(
    Player& player,
    const Enemy& enemy
) {
    // Assemble victory rows from this enemy's coin and optional item rewards.
    auto screen = ScreenInteractive::FitComponent();

    auto continueButton = Button("Continue", [&] {
        screen.Exit();
    });

    auto container = Container::Vertical({continueButton});

    auto renderer = Renderer(container, [&] {
        Elements rewards = {
            text("You defeated the " + enemy.name + "!"),
            separator(),
            text("Coins collected: " + to_string(enemy.coinDrop))
        };

        // Add the item row only for enemies configured with an extra drop.
        if (!enemy.extraDrop.empty()) {
            rewards.push_back(text("Item collected: " + enemy.extraDrop));
        }

        rewards.push_back(separator());
        rewards.push_back(container->Render());

        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text("VICTORY") | bold,
            separator(),
            vbox(rewards)
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));
}

void displayDefeat(Player& player, const Enemy& enemy) {
    // Show which enemy ended the run before control unwinds to the main menu.
    auto screen = ScreenInteractive::FitComponent();

    auto quitButton = Button("Return", [&] {
        screen.Exit();
    });

    auto container = Container::Vertical({quitButton});

    auto renderer = Renderer(container, [&] {
        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text("DEFEAT") | bold | color(Color::Red),
            separator(),
            paragraph("You were defeated by the " + enemy.name + "."),
            separator(),
            container->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));
}

void displayCoinExplanation(Player& player) {
    // This one-time screen teaches that the coin count now lives in the header, avoiding a permanent extra coin window that previously overflowed the UI.
    auto screen = ScreenInteractive::FitComponent();

    auto continueButton = Button("Continue", [&] {
        screen.Exit();
    });

    auto container = Container::Vertical({continueButton});

    auto renderer = Renderer(container, [&] {
        return vbox({
            hbox({filler(), coinTracker(player)}),
            separator(),
            text("NEW ITEM DISCOVERED: COIN") | bold,
            separator(),
            paragraph(
                "This is a coin, an item used to buy things. Surely you "
                "aren't so confused that you forgot what one of these is, "
                "so I don't know why I'm explaining this to you."
            ),
            separator(),
            text("Your coin total is shown in the top-right corner."),
            separator(),
            container->Render()
        }) |
               size(WIDTH, EQUAL, gameWindowWidth) |
               size(HEIGHT, EQUAL, gameWindowHeight) |
               border;
    });

    screen.Loop(withInventory(player, renderer, screen));
}