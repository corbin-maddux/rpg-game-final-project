# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/workspaces/rpg-game-final-project/build/_deps/ftxui-src"
  "/workspaces/rpg-game-final-project/build/_deps/ftxui-build"
  "/workspaces/rpg-game-final-project/build/_deps/ftxui-subbuild/ftxui-populate-prefix"
  "/workspaces/rpg-game-final-project/build/_deps/ftxui-subbuild/ftxui-populate-prefix/tmp"
  "/workspaces/rpg-game-final-project/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp"
  "/workspaces/rpg-game-final-project/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src"
  "/workspaces/rpg-game-final-project/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/workspaces/rpg-game-final-project/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/workspaces/rpg-game-final-project/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
