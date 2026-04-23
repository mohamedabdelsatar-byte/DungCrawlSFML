cmake_minimum_required(VERSION 3.24)
project(SFML_Sandbox1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
#include_directories(include)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
)

FetchContent_MakeAvailable(SFML)

add_executable(SFML_Sandbox1
        src/main.cpp
        #src/Game.cpp
        src/Player.cpp
        src/MainMenu.cpp
        src/Sequences.cpp
        src/Rooms.cpp
        src/Interactables.cpp
        src/Enemy.cpp
        src/AudioManager.cpp
)

target_link_libraries(SFML_Sandbox1 PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

target_include_directories(SFML_Sandbox1 PRIVATE include)

# Copy Assets folder to build folder after build
add_custom_command(TARGET SFML_Sandbox1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:SFML_Sandbox1>/Assets
)