#pragma once

#include <string>

enum class RoomID
{
    MainRoom, //0 visited[5]
    SnakeRoom, //1 visited[0]
    ExitRoom, //2 visited[1]
    chestToolroom, //3 visited[2]
    keyLockRoom, //4 visited[3]
    chestcombinationRoom, //5 visited[4]
    puzzleOverlay, //6
    winScreen //7
};

enum ItemIndex{
    KEY=0,
    //NONE=1,
    //NONE=2,
    //NONE=3,
    SNAKE_TOOL=4,
    //NONE=5,
    //NONE=6,
    //NONE=7,
};

extern bool Escaped;

struct PlayerData{
    std::string name;
    //char gender;
    bool hasKey = false;
    int Health = 5;
    bool inv [7] = {false, false, false, false, false, false, false};

    void reset() {
        name = "";
        hasKey = false;
        Health = 5;
        for (int i=0; i<7; i++) inv[i] = false;
    }
};

struct GameState{
    bool visited[5] = {false, false, false, false, false};
    bool completed[4]= {false, false, false, false};
    bool Escaped = false;
    bool keepPlaying = true;
    
    // Overlay State
    bool overlayActive = false;
    float overlayTimer = 0.0f;
    int currentOverlay = -1; // e.g. ItemIndex::KEY or ItemIndex::SNAKE_TOOL

    void reset() {
        for (int i=0; i<5; i++) visited[i] = false;
        for (int i=0; i<4; i++) completed[i] = false;
        Escaped = false;
        keepPlaying = true;
        overlayActive = false;
        overlayTimer = 0.0f;
        currentOverlay = -1;
    }
};