#pragma once
#include "Enemy.h"
#include "Player.h"

enum class SequenceID {
    None,
    SnakeRoomSeq,
    SnakeRoomSeqWithPotion,
    itemPickup,
    puzzleOverlay,
    WinScreen
};

class Sequences {
public:
    Sequences();
    ~Sequences();

    void init();
    void update(float deltaTime, Player& player, Enemy& enemy);
    void triggerSequence(SequenceID id);
    
    int getCurrentStep() const { return currentStep; }

    bool isSequenceActive;
    SequenceID activeSequence;

private:
    int currentStep;
    float timer;

    void PlaySnakeRoomSequence(float deltaTime, Player& player, Enemy& snakeEnemy);
    void PlaySnakeRoomSequenceWithPotion(float deltaTime, Player& player, Enemy& snakeEnemy);
    void PlayItemPickupSequence(float deltaTime, Player& player);
    void PlayPuzzleOverlaySequence(float deltaTime, Player& player);
    void PlayWinScreenSequence(float deltaTime, Player& player);
};