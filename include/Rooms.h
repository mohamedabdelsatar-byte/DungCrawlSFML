#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "GameData.h"
#include "Interactables.h"
#include "Enemy.h"
#include "Sequences.h"
#include "AudioManager.h"

class Player;

class Rooms
{   
    
    
    private: 
    //start room
    RoomID activeRoom = RoomID::MainRoom;
    //door textures and sprites
    sf::Sprite doorSprite;
    sf::Sprite exitDoorSprite;
    sf::Sprite floorSprite;
    sf::Sprite backgroundSprite;
    sf::Sprite wallSprite;
    
    std::vector<sf::Sprite> doorSprites;
    
    // Room textures
    sf::Texture doorTexture;
    sf::Texture backgroundTexture;

    sf::Texture mainFloorTexture; // For Plan 2
    sf::Texture sideFloorTexture; // For Plan 1

    sf::Texture wallTexture;

    sf::Texture mainRoomTexture;
    sf::Texture snakeRoomTexture;
    sf::Texture exitRoomTexture;
    sf::Texture chestToolroomTexture;
    sf::Texture keyLockRoomTexture;
    sf::Texture chestcombinationRoomTexture;
    sf::Texture puzzleOverlayTexture;
    sf::Texture winScreenTexture;
    
    // --- AUDIO CONTEXT ---
    bool hasOpenedChestOnce = false;

    // Obtainment Overlays
    sf::Texture keyOverlayTexture;
    sf::Texture potionOverlayTexture;
    sf::Sprite overlaySprite;
    sf::RectangleShape overlayBackground;
    
    // Obtainment Text
    sf::Font overlayFont;
    sf::Text overlayText;
    
    // Interaction Text
    sf::Text interactionText;
    float interactionTimer = 0.f;
    bool showInteraction = false;
    void showInteractionMessage(const std::string& message);

    // Additional textures
    sf::Texture tableTexture;
    sf::Texture keyTexture;
    std::vector<sf::RectangleShape> doors;
    std::vector<sf::RectangleShape> returnDoors;
    sf::RectangleShape floor;
    
    // door index 2 and 4 will have tables.
    Interactables tablePuzzleTwo;
    Interactables tablePuzzleFour; 
    Interactables chestClosed;
    Interactables chestOpen;
    Interactables keyItem; // The new animated key item
    Interactables pedestalItem; // Pedestal prop under the key
    Interactables snakePotionItem; // The potion inside the chest
    
    // Enemy for snakeRoom
    Enemy snakeRoomEnemy;
    // Sequence for snakeRoom encounter
    Sequences sequenceManager;


    // Walls for screen boundaries
    sf::RectangleShape leftWall;
    sf::RectangleShape rightWall;
    sf::RectangleShape topWall;
    sf::RectangleShape bottomWall;
    sf::RectangleShape exitZoneBox;

    
public:
    // Getters for hitboxes (COLLISION)
    bool eKeyWasPressed = false;
    sf::Vector2f playerEntryPosition = {960.f, 600.f}; // Store where player entered from
    
    sf::FloatRect getFloorHitbox() const;
    sf::FloatRect getDoorHitbox(int doorIndex) const;
    sf::FloatRect getLeftWallHitbox() const { return leftWall.getGlobalBounds(); }
    sf::FloatRect getRightWallHitbox() const { return rightWall.getGlobalBounds(); }
    sf::FloatRect getTopWallHitbox() const { return topWall.getGlobalBounds(); }
    sf::FloatRect getBottomWallHitbox() const { return bottomWall.getGlobalBounds(); }
    Rooms();


    /* Rooms();
    ~Rooms(); */

    public:
    void init();
    //std::string roomFile
    void load();
    void update(float deltaTime, Player& player, GameState& gameState, PlayerData& playerData, AudioManager& audioManager);
    void draw(sf::RenderWindow& window, const GameState& gameState);
    void drawOverlay(sf::RenderWindow& window, const GameState& gameState);
    void changeRoom(RoomID id, Player& player, AudioManager& audioManager);
    void handleDoorChoice(int index, Player& player, GameState& gameState, PlayerData& playerData, AudioManager& audioManager);
    void fixReturnPosition(Player& player);
    RoomID getActiveRoom() const { return activeRoom;}

};