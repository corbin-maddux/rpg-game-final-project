#include <iostream>
#include <string>


// FTXUI libraries
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
using namespace std;
using namespace ftxui;
// Function prototypes
void mainMenu();
void startGame();
void levelZero();
void displayItem(
    string itemName,
    string description
);
void enemyEncounter(
    string enemyName,
    int hp,
    int damage,
    string drop
);
int main() {
    mainMenu();
    return 0;
}
// MAIN MENU
void mainMenu() {
    auto screen = ScreenInteractive::TerminalOutput();
    auto startButton = Button(
        "Start Game",
        [&] {
            screen.Exit();
            startGame();
        }
    );
    auto quitButton = Button(
        "Quit",
        [&] {
            screen.Exit();
        }
    );
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
        })
        | border;
    });
    screen.Loop(renderer);
}
// START GAME
void startGame() {
    auto screen = ScreenInteractive::TerminalOutput();
    auto continueButton = Button(
        "Enter Dungeon",
        [&] {
            screen.Exit();
        }
    );
    auto container = Container::Vertical({
        continueButton
    });
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("LEVEL 0"),
            separator(),
            text("\033[3mThe dungeon walls surround you as you awaken in darkness...\033[0m"),
            text("You do not remember how you arrived here."),
            text("A cold wind moves through the empty halls."),
            separator(),
            continueButton->Render()
        })
        | border;
    });
    screen.Loop(renderer);
    // Begin first level
    levelZero();
}
// LEVEL 0 STORY

void levelZero() {
    // Player finds first weapon
    displayItem(
        "Iron Sword",
        "A basic weapon found inside the dungeon.\n\nDamage: 20"
    );
    // First enemy
    enemyEncounter(
        "Dungeon Rat",
        30,
        10,
        "5 Coins"
    );
    // Second enemy
    enemyEncounter(
        "Cave Slime",
        50,
        20,
        "10 Coins + Healing Potion"
    );
}
// ITEM DISCOVERY
void displayItem(
    string itemName,
    string description
) {
    auto screen = ScreenInteractive::TerminalOutput();
    auto collectButton = Button(
        "Collect",
        [&] {
            screen.Exit();
        }
    );
    auto container = Container::Vertical({
        collectButton
    });
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("ITEM FOUND!"),
            separator(),
            text(itemName),
            text(description),
            separator(),
            collectButton->Render()
        })
        | border;
    });
    screen.Loop(renderer);
}
// TEMPORARY ENEMY SYSTEM
void enemyEncounter(
    string enemyName,
    int hp,
    int damage,
    string drop
) {
    auto screen = ScreenInteractive::TerminalOutput();
    auto killButton = Button(
        "Kill",
        [&] {
            screen.Exit();
        }
    );
    auto container = Container::Vertical({
        killButton
    });
    auto renderer = Renderer(container, [&] {
        return vbox({
            text("ENEMY ENCOUNTER"),
            separator(),
            text(enemyName),
            text(
                "HP: " + to_string(hp)
            ),
            text(
                "Damage: " + to_string(damage)
            ),
            separator(),
            text("\033[3mThe creature approaches...\033[0m"),
            killButton->Render()

        })
        | border;
    });
    screen.Loop(renderer);
    // Temporary victory text
    cout << "\nYou defeated the "
         << enemyName
         << "!\n";
    cout << "Dropped: "
         << drop
         << "\n\n";
}