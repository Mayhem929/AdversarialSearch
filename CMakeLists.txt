cmake_minimum_required(VERSION 2.8)

project(Parchis_Game)

# Should build Doxygen
option(BUILD_DOC "Build documentation" OFF)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14")  # Pasamos de Wall y Wextra mejor xd
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Enable debug symbols by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" 
      FORCE
    )
endif()

# Find SFML shared libraries
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})
add_definitions(${SFML_DEFINITIONS})
if(NOT SFML_FOUND)
  message(FATAL_ERROR "SFML not found")
endif()

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND AND BUILD_DOC)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND AND BUILD_DOC)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND AND BUILD_DOC)

  file(GLOB SOURCES "src/*.cpp")
  file(GLOB SOURCES_main "src/main.cpp")
  file(GLOB SOURCES_ParchisGUI "src/ParchisGUI.cpp")
  file(GLOB SOURCES_Parchis "src/Parchis.cpp")
  file(GLOB SOURCES_Board "src/Board.cpp")
  file(GLOB SOURCES_Dice "src/Dice.cpp")
  file(GLOB SOURCES_Piece "src/Piece.cpp")
  file(GLOB SOURCES_BoardTrap "src/BoardTrap.cpp")
  file(GLOB SOURCES_ClickableSprite "src/ClickableSprite.cpp")
  file(GLOB SOURCES_BoardSprite "src/BoardSprite.cpp")
  file(GLOB SOURCES_DiceSprite "src/DiceSprite.cpp")
  file(GLOB SOURCES_PieceSprite "src/PieceSprite.cpp")
  file(GLOB SOURCES_SpecialItemSprite "src/SpecialItemSprite.cpp")
  file(GLOB SOURCES_BoardTrapSprite "src/BoardTrapSprite.cpp")
  file(GLOB SOURCES_ExplosionSprite "src/ExplosionSprite.cpp")
  file(GLOB SOURCES_Player "src/Player.cpp")
  file(GLOB SOURCES_AIPlayer "src/AIPlayer.cpp")
  file(GLOB SOURCES_Ninja "src/Ninja.cpp")
  file(GLOB SOURCES_GUIPlayer "src/GUIPlayer.cpp")
  file(GLOB SOURCES_RemotePlayer "src/RemotePlayer.cpp")
  file(GLOB SOURCES_SpriteAnimator "src/SpriteAnimator.cpp")
  file(GLOB SOURCES_Button "src/Button.cpp")
  file(GLOB SOURCES_Connection "src/Connection.cpp")
  file(GLOB SOURCES_GameSelector "src/GameSelector.cpp")
  include_directories("include")

  add_library(main STATIC ${SOURCES_main})
  add_library(ParchisGUI STATIC ${SOURCES_ParchisGUI})
  add_library(Parchis STATIC ${SOURCES_Parchis})
  add_library(Board STATIC ${SOURCES_Board})
  add_library(Dice STATIC ${SOURCES_Dice})
  add_library(Piece STATIC ${SOURCES_Piece})
  add_library(BoardTrap STATIC ${SOURCES_BoardTrap})
  add_library(ClickableSprite STATIC ${SOURCES_ClickableSprite})
  add_library(BoardSprite STATIC ${SOURCES_BoardSprite})
  add_library(DiceSprite STATIC ${SOURCES_DiceSprite})
  add_library(PieceSprite STATIC ${SOURCES_PieceSprite})
  add_library(SpecialItemSprite STATIC ${SOURCES_SpecialItemSprite})
  add_library(BoardTrapSprite STATIC ${SOURCES_BoardTrapSprite})
  add_library(ExplosionSprite STATIC ${SOURCES_ExplosionSprite})
  add_library(Player STATIC ${SOURCES_Player})
  add_library(AIPlayer STATIC ${SOURCES_AIPlayer})
  add_library(Ninja STATIC ${SOURCES_Ninja})
  add_library(GUIPlayer STATIC ${SOURCES_GUIPlayer})
  add_library(RemotePlayer STATIC ${SOURCES_RemotePlayer})
  add_library(SpriteAnimator STATIC ${SOURCES_SpriteAnimator})
  add_library(Button STATIC ${SOURCES_Button})
  add_library(Connection STATIC ${SOURCES_Connection})
  add_library(GameSelector STATIC ${SOURCES_GameSelector})

  target_link_libraries(ParchisGUI sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
  add_executable(ParchisGame ${SOURCES})
  target_link_libraries(ParchisGame main ParchisGUI Board Dice Piece BoardTrap ClickableSprite BoardSprite DiceSprite PieceSprite SpecialItemSprite BoardTrapSprite ExplosionSprite Player AIPlayer Ninja GUIPlayer RemotePlayer SpriteAnimator Button Connection GameSelector)

