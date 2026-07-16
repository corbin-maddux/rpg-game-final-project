cmake_minimum_required(VERSION 3.14)

project(beginner_rpg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v6.1.9
)

FetchContent_MakeAvailable(ftxui)

add_executable(game
    main.cpp
    ui.cpp
    combat.cpp
    inventory.cpp
    leveling.cpp
    merchant.cpp
    level_zero.cpp
    level_one.cpp
)

target_link_libraries(game PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)