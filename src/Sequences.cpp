#include "Sequences.h"
#include "Enemy.h"
#include "Player.h"
#include <iostream>


Sequences::Sequences()
{

}



void Sequences::init()
{
    activeSequence = SequenceID::None;
    isSequenceActive = false;
    currentStep = 0;
    timer = 0.f;

}

void Sequences::update(float deltaTime, Player& player, Enemy& enemy)
{
    if (!isSequenceActive) return;

    switch (activeSequence) {
        case SequenceID::SnakeRoomSeq:
            PlaySnakeRoomSequence(deltaTime, player, enemy);
            break;
        case SequenceID::SnakeRoomSeqWithPotion:
            PlaySnakeRoomSequenceWithPotion(deltaTime, player, enemy);
            break;
        case SequenceID::itemPickup:
            PlayItemPickupSequence(deltaTime, player);
            break;
        case SequenceID::puzzleOverlay:
            PlayPuzzleOverlaySequence(deltaTime, player);
            break;
        case SequenceID::WinScreen:
            PlayWinScreenSequence(deltaTime, player);
            break;
        default:
            break;
    }
}

void Sequences::PlaySnakeRoomSequence(float deltaTime, Player& player, Enemy& enemy)
{
    player.isControlDisabled = true; // Disable player input
    player.setVelocity({0.f, player.getVelocity().y}); // Stop any horizontal momentum

    std::cout << "[Seq DEBUG] Step: " << 
    currentStep << ", Timer: " << timer << 
    ", DeltaTime: " << deltaTime << 
    ", Enemy X: " << enemy.sprite.getPosition().x << "\n";

    switch(currentStep) {
        case 0:
            // STEP 0: THE AMBIENCE (1 Second)
            if (timer == 0.f) {
                player.sprite.setPosition({-200.f, 800.f}); 
                player.boundRect.setPosition(player.sprite.getPosition()); // Keep hitbox synced

                // Keep the enemy standing still (or pacing if you add that back later)
                enemy.isRunning = false;
            }

            timer += deltaTime;
            std::cout << "[Seq Step0] Timer now: " << timer << "\n";
            if (timer >= 1000.0f) { // 1 second
                currentStep = 1;
                timer = 0.f;
                std::cout << "[Sequence] Step 0→1: *SLAM* Door bursts open!\n";
            }
            break;

        case 1:
            // STEP 1: THE SLAM (2 Seconds)
            if (timer == 0.f) {
                player.sprite.setPosition({150.f, 800.f});
                player.boundRect.setPosition(player.sprite.getPosition());
                player.sprite.setScale({4.5f, 4.5f}); 
            }

            timer += deltaTime;
            std::cout << "[Seq Step1] Timer now: " << timer << "\n";
            if (timer >= 2000.0f) { // 2 seconds
                currentStep = 2;
                timer = 0.f;
                std::cout << "[Sequence] Step 1→2: Door opens, player appears...\n";
            }
            break;

        case 2:
            // STEP 2: THE STARE (1 Second)
            if (timer == 0.f) {
                std::cout << "[Sequence] Step 2: Enemy stops and stares...\n";
            }
            enemy.velocity.x = 0.f;               // Guard stops in his tracks
            enemy.isRunning = false;              // Play idle animation
            enemy.sprite.setScale({-9.0f, 9.0f}); // Snap his head left to look at the door!

            timer += deltaTime;
            std::cout << "[Seq Step2] Timer now: " << timer << "\n";
            if (timer >= 1000.0f) { // 1 second
                currentStep = 3;
                timer = 0.f;
                std::cout << "[Sequence] Step 2→3: Guard: WAAAARGH!\n";
            }
            break;

        case 3:
            // STEP 3: THE CHARGE (1 Second max or until close)
            if (timer == 0.f) {
                std::cout << "[Sequence] Step 3: Enemy charging! Enemy pos: " << enemy.sprite.getPosition().x << "\n";
            }
            enemy.velocity.x = -1.5f; // Slower, reasonable terrifying sprint instead of sonic speed!
            enemy.isRunning = true;     // Trigger his running animation

            timer += deltaTime;
            std::cout << "[Seq Step3] Timer: " << timer << ", Enemy X: " << enemy.sprite.getPosition().x << ", Checking if X <= 300: " << (enemy.sprite.getPosition().x <= 300.f ? "YES" : "NO") << "\n";

            // Wait until he gets uncomfortably close to the player (e.g., X = 300) OR 1 second passes
            if (enemy.sprite.getPosition().x <= 300.f || timer >= 1000.0f) { // 1 second
                currentStep = 4;
                std::cout << "[Sequence] Step 3→4: Enemy reached player! Pos: " << enemy.sprite.getPosition().x << "\n";
            }
            break;

        case 4:
            // STEP 4: THE NOPE
            // The movie is over! Let Rooms.cpp know it's time to kick the player out.
            std::cout << "[Sequence] Step 4: NOPE! Kicking player out!\n";
            player.isControlDisabled = false; // Give controls back to the player
            isSequenceActive = false;
            break;
    }
}

void Sequences::PlayItemPickupSequence(float deltaTime, Player& player)
{
    // TODO: Implement item pickup sequence
}

void Sequences::PlayPuzzleOverlaySequence(float deltaTime, Player& player)
{
    // TODO: Implement puzzle overlay sequence
}

void Sequences::PlayWinScreenSequence(float deltaTime, Player& player)
{
    // TODO: Implement win screen sequence
}

void Sequences::triggerSequence(SequenceID id) {
    activeSequence = id;
    isSequenceActive = true;
    currentStep = 0;
    timer = 0.f;
}

Sequences::~Sequences()
{

}
void Sequences::PlaySnakeRoomSequenceWithPotion(float deltaTime, Player& player, Enemy& enemy)
{
    player.isControlDisabled = true; // Disable player input
    player.setVelocity({0.f, player.getVelocity().y}); // Stop any horizontal momentum

    switch(currentStep) {
        case 0:
            // STEP 0: THE AMBIENCE (1 Second)
            if (timer == 0.f) {
                player.sprite.setPosition({-200.f, 800.f});
                player.boundRect.setPosition(player.sprite.getPosition()); // Keep hitbox synced

                // Make enemy start near pedestal. Preserve Y position so he's not floating!
                enemy.sprite.setPosition({800.f, 700.f});
                enemy.isRunning = false;
                enemy.velocity.x = 0.f;
            }

            timer += deltaTime;
            if (timer >= 1000.0f) { // 1 second
                currentStep = 1;
                timer = 0.f;
            }
            break;

        case 1:
            // STEP 1: THE SLAM (2 Seconds)
            if (timer == 0.f) {
                player.sprite.setPosition({150.f, 800.f});
                player.boundRect.setPosition(player.sprite.getPosition());
                player.sprite.setScale({4.5f, 4.5f}); 
            }

            timer += deltaTime;
            if (timer >= 2000.0f) { // 2 seconds
                currentStep = 2;
                timer = 0.f;
            }
            break;

        case 2:
            // STEP 2: THE STARE (1 Second)
            enemy.velocity.x = 0.f;               // Guard stops in his tracks
            enemy.isRunning = false;              // Play idle animation
            enemy.sprite.setScale({-9.0f, 9.0f}); // Snap his head left to look at the door!

            timer += deltaTime;
            if (timer >= 1000.0f) { // 1 second
                currentStep = 3;
                timer = 0.f;
            }
            break;

        case 3:
            // STEP 3: THE FAST CHARGE
            enemy.velocity.x = -3.5f; // Fast charge!
            enemy.isRunning = true;     // Trigger his running animation

            timer += deltaTime;
            // Wait until he gets uncomfortably close to the player
            if (enemy.sprite.getPosition().x <= 300.f || timer >= 1000.0f) { 
                currentStep = 4;
            }
            break;

        case 4:
            // STEP 4: THE FIGHT
            // Player gets control back, enemy continues charging to get killed
            player.isControlDisabled = false;
            isSequenceActive = false;
            break;
    }
}
