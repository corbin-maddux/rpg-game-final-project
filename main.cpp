#include "game.hpp"
#include "levels.hpp"
#include "ui.hpp"

int main() {
    Player player;
    setUiPlayer(&player);

    while (true) {
        player.returnToMainMenu = false;

        int choice = showMenu(
            "CAVE OF SHADOWS",
            {"A small terminal RPG made with C++ and FTXUI."},
            {"Start Game", "Quit"}
        );

        if (choice == -2) {
            continue;
        }

        if (choice == 1) {
            return 0;
        }

        player = Player();

        bool levelFinished = levelZero(player);

        if (!levelFinished) {
            continue;
        }

        levelFinished = levelOne(player);

        if (!levelFinished) {
            continue;
        }

        showMessage(
            "CURRENT GAME COMPLETE",
            {"You completed Level 0 and Level 1."},
            "Return to Main Menu"
        );
    }
}