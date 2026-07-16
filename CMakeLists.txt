# Reject configuration on CMake versions older than the dependency setup needs.
cmake_minimum_required(VERSION 3.14)

# Name the project and state that it is written in C++.
project(rpg_game LANGUAGES CXX)

# Use C++17 because this game's syntax and FTXUI setup are written for it.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Load CMake's dependency helper so this repository need not contain FTXUI code.
include(FetchContent)

# Pin FTXUI v6.1.9 so future upstream changes do not unexpectedly break the UI.
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v6.1.9
)

# Make FTXUI's targets available to this project.
FetchContent_MakeAvailable(ftxui)

# Compile these six .cpp files together into the ./build/game executable.
add_executable(game
    main.cpp
    combat.cpp
    inventory.cpp
    merchant.cpp
    level_zero.cpp
    level_one.cpp
)

# Supply the three FTXUI modules used by buttons, boxes/text, and terminal output.
target_link_libraries(game PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)
