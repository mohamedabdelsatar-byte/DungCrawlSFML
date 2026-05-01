#include "Rooms.h"
#include "GameData.h"
#include "Player.h"
#include "Interactables.h"
#include <imgui.h>


#include <iostream>
#include <string>

Rooms::Rooms() : backgroundSprite(mainRoomTexture), 
floorSprite(mainFloorTexture), doorSprite(doorTexture), 
exitDoorSprite(doorTexture), wallSprite(wallTexture), overlaySprite(keyOverlayTexture), overlayText(overlayFont), interactionText(overlayFont), riddleScreenText(overlayFont)
{
}

void Rooms::init()
{
    snakeRoomEnemy.Init();

    tablePuzzleTwo.Init();
    tablePuzzleFour.Init();
    
    chestClosed.Init();
    chestClosed.Sprite.setScale({8.0f, 8.0f});
    
    chestOpen.Init();
    chestOpen.Sprite.setScale({8.0f, 8.0f});
    
    snakePotionItem.Init();
    snakePotionItem.Sprite.setScale({5.0f, 5.0f}); // Need to adj. depending on the png
    snakePotionItem.setPosition({960.f, 800.f}); // Roughly near chest center, will place correctly on load

    pedestalItem.Init();
    pedestalItem.Sprite.setScale({4.0f, 4.0f}); // Adjust scale if needed for the pedestal
    pedestalItem.setPosition({950.f, 850.f}); // Set to the same coordinates as the key
    
    keyItem.Init();
    keyItem.Sprite.setScale({4.0f, 4.0f}); // Scaled back down to a more appropriate size
    keyItem.setPosition({950.f, 750.f}); // Moved key slightly up so it visualizes on top of the pedestal

    wallSprite.setPosition({-100.f, 0.f});
    floor.setSize({1920.f, 50.f});
    floor.setPosition({0.f, 900.f});
    floor.setFillColor(sf::Color::Transparent); 

    // 2. Setup the 5 Doors from the Spritesheet
    float wallThickness = 50.f;
    float rightWallOffset = 230.f; 
    
    float startX = wallThickness; 
    float endX = 1920.f - wallThickness - rightWallOffset; 
    float playableWidth = endX - startX;
    float spacing = playableWidth / 6.f; 

    sf::IntRect lockedRect({256, 64}, {32, 64});
    sf::IntRect unlockedRect({288, 64}, {32, 64}); 

    float scaleFactor = 4.7f; // Scale up the pixel art to fit the screen
    float doorWidth = 32.f * scaleFactor;
    float doorHeight = 64.f * scaleFactor;
    
    // R O G B Y color setting
    doorColors[0] = sf::Color(255, 0, 0);    // Red
    doorColors[1] = sf::Color(255, 128, 0);  // Orange
    doorColors[2] = sf::Color(0, 255, 0);    // Green
    doorColors[3] = sf::Color(0, 0, 255);    // Blue
    doorColors[4] = sf::Color(255, 255, 0);  // Yellow

    for (int i = 0; i < 5; i++) {
        sf::Sprite door(doorTexture);
        
        // Door 0 is the locked Exit door
        if (i == 0) door.setTextureRect(lockedRect);
        else door.setTextureRect(unlockedRect);

        door.setScale({scaleFactor, scaleFactor});
        
        float xPos = startX + ((i + 1) * spacing) - (doorWidth / 2.f);
        float yPos = 900.f - doorHeight; 
        
        door.setPosition({xPos, yPos});
        
        door.setColor(sf::Color(150, 150, 150)); 

        doorSprites.push_back(door); 
    }
    
    exitZoneBox.setSize({doorWidth, doorHeight}); 
    exitZoneBox.setPosition({wallThickness, 900.f - doorHeight}); 
    exitZoneBox.setFillColor(sf::Color::Transparent);
    exitZoneBox.setOutlineColor(sf::Color::White);
    exitZoneBox.setOutlineThickness(0.f);

    sf::IntRect openRect({256, 0}, {32, 64}); 
    
    exitDoorSprite.setTextureRect(openRect);
    exitDoorSprite.setScale({scaleFactor, scaleFactor});
    exitDoorSprite.setPosition({wallThickness, 900.f - doorHeight});


}

void Rooms::load()
{
    snakeRoomEnemy.Load();
    tablePuzzleTwo.Load(std::string("Assets/Textures/table.png"));
    tablePuzzleFour.Load(std::string("Assets/Textures/table.png"));
    chestClosed.Load(std::string("Assets/Textures/chest_closed.png"));
    chestClosed.setPosition({960.f, 860.f});
    chestClosed.LoadAlternate(std::string("Assets/Textures/chest_opened.png"));  // Load open texture as alternate
    chestClosed.Sprite.setScale({8.0f, 8.0f});
    chestOpen.Load(std::string("Assets/Textures/chest_opened.png"));
    chestOpen.setPosition({960.f, 860.f});
    chestOpen.Sprite.setScale({8.0f, 8.0f});

    snakePotionItem.Load(std::string("Assets/Textures/snakekillerpotion.png"));
    snakePotionItem.setPosition({960.f, 800.f}); // Move it up from chest slightly
    
    pedestalItem.Load(std::string("Assets/Textures/pedestal.png"));
    pedestalItem.setPosition({750.f, 750.f}); // Set pedestal to X=750, Y=750

    keyItem.Load(std::string("Assets/Textures/key.png"));
    keyItem.MakeAnimated(9, 250.f, 16, 16); // 9 frames, 250ms per frame to slow it down, 16x16 size
    keyItem.setPosition({750.f, 720.f}); // Move the key slightly up so it rests on top

    //--LOADING TEXTURES--
    if (!doorTexture.loadFromFile("Assets/Textures/doors.png")) {
        std::cout << "ERROR: Could not find Assets/Textures/doors.png\n";
    }

    if (!mainFloorTexture.loadFromFile("Assets/Textures/Plan 2.png")) {
        std::cout << "ERROR: Could not find Plan 2.png\n";
    }
    if (!sideFloorTexture.loadFromFile("Assets/Textures/Plan 1.png")) {
        std::cout << "ERROR: Could not find Plan 1.png\n";
    }

    if (!wallTexture.loadFromFile("Assets/Textures/walls.png")) {
        std::cout << "ERROR: Could not find Assets/Textures/walls.png\n";
    }
    else{
        wallSprite.setTexture(wallTexture, true); 
    }

    // Set the default Hub Floor
    if (mainFloorTexture.getSize().x > 0) {
        floorSprite.setTexture(mainFloorTexture, true); 
        float floorScale = 1920.f / 576.f; 
        floorSprite.setScale({floorScale, floorScale});
        floorSprite.setPosition({0.f, -20.f}); 
    }

    //BACKGROUND
    if (!mainRoomTexture.loadFromFile("Assets/Textures/Plan 3.png")) {
        std::cout << "ERROR: Could not find Plan 3.png\n";
    } else {
        backgroundSprite.setTexture(mainRoomTexture, true); 
        float bgScale = 1920.f / 576.f; 
        backgroundSprite.setScale({bgScale, bgScale});
        backgroundSprite.setPosition({0.f, 0.f});
    }

    
    if (!snakeRoomTexture.loadFromFile("Assets/Textures/snake_room.png")) {
        std::cout << "WARNING: snake_room.png missing. Using placeholder.\n";
        snakeRoomTexture = mainRoomTexture; 
    }
    
    if (!chestToolroomTexture.loadFromFile("Assets/Textures/chest_room.png")) {
        chestToolroomTexture = mainRoomTexture; 
    }
    
    if (!keyLockRoomTexture.loadFromFile("Assets/Textures/key_room.png")) {
        keyLockRoomTexture = mainRoomTexture; 
    }
    
    if (!chestcombinationRoomTexture.loadFromFile("Assets/Textures/combination_room.png")) {
        chestcombinationRoomTexture = mainRoomTexture; 
    }

    if (!exitRoomTexture.loadFromFile("Assets/Textures/exit_room.png")) {
        exitRoomTexture = mainRoomTexture; 
    }

    if (!keyOverlayTexture.loadFromFile("Assets/Textures/keyoverlay.png")) {
        std::cout << "WARNING: keyoverlay.png missing.\n";
    }

    if (!potionOverlayTexture.loadFromFile("Assets/Textures/potionoverlay.png")) {
        std::cout << "WARNING: potionoverlay.png missing.\n";
    }

    // Set up the dark and semi-transparent overlay background
    overlayBackground.setSize({1920.f, 1080.f});
    overlayBackground.setFillColor(sf::Color(0, 0, 0, 0)); // Start invisible
    overlayBackground.setPosition({0.f, 0.f});
    
    // Set up Overlay Text
    if (!overlayFont.openFromFile("Assets/Fonts/Tuffy.ttf")) {
        std::cout << "WARNING: Tuffy.ttf missing inside Assets/Fonts/.\n";
    }
    overlayText.setFont(overlayFont);
    overlayText.setCharacterSize(60); 
    overlayText.setFillColor(sf::Color::White);
    overlayText.setOutlineColor(sf::Color::Black);
    overlayText.setOutlineThickness(4.f);
    overlayText.setString("");

    // Set up Interaction Text
    interactionText.setFont(overlayFont);
    interactionText.setCharacterSize(26); 
    interactionText.setFillColor(sf::Color::White);
    interactionText.setOutlineColor(sf::Color::Black);
    interactionText.setOutlineThickness(3.f);
    interactionText.setString("");

    // Load new blocky font
    if (!riddleFont.openFromFile("Assets/Fonts/ARCADECLASSIC.TTF")) {
        std::cout << "WARNING: ARCADECLASSIC.TTF missing inside Assets/Fonts/.\n";
    }

    // Set up Riddle Screen Text
    riddleScreenText.setFont(riddleFont);
    riddleScreenText.setCharacterSize(40);
    riddleScreenText.setFillColor(sf::Color::White);
    riddleScreenText.setOutlineColor(sf::Color::Black);
    riddleScreenText.setOutlineThickness(3.f); // slightly thicker outline for arcade font
    riddleScreenText.setString("");

    // --- Load Walls ---
    const float wallThickness = 50.f;
    const float screenWidth = 1920.f;
    const float screenHeight = 1080.f;
    
    leftWall.setSize({wallThickness, screenHeight});
    leftWall.setPosition({0.f, 0.f});
    leftWall.setFillColor(sf::Color(255, 0, 0, 150));
    
    rightWall.setSize({wallThickness, screenHeight});
    rightWall.setPosition({screenWidth - wallThickness - 230.f, 0.f});
    rightWall.setFillColor(sf::Color(255, 0, 0, 150));
    
    topWall.setSize({screenWidth, wallThickness});
    topWall.setPosition({0.f, 0.f});
    topWall.setFillColor(sf::Color(0, 255, 0, 150));
    
    bottomWall.setSize({screenWidth, wallThickness});
    bottomWall.setPosition({0.f, screenHeight - wallThickness});
    bottomWall.setFillColor(sf::Color(0, 0, 255, 150));
}

void Rooms::update(float deltaTime, Player& player, GameState& gameState, PlayerData& playerData, AudioManager& audioManager)
{
    // --- Overlay Update ---
    if (gameState.overlayActive) {
        player.isControlDisabled = true;
        gameState.overlayTimer -= deltaTime;
        
        // Handle fade-in (e.g. over the first 0.5s or proportional to 3 seconds)
        // Let's do a simple alpha calc over 3 seconds (3000ms).
        float progress = 1.0f - (gameState.overlayTimer / 3000.f); 
        if (progress > 1.0f) progress = 1.0f;
        if (progress < 0.0f) progress = 0.0f;
        
        // Darken screen up to 150 alpha
        unsigned char bgAlpha = static_cast<unsigned char>(150.f * ((progress * 2.f < 1.0f) ? (progress * 2.f) : 1.0f)); 
        overlayBackground.setFillColor(sf::Color(0, 0, 0, bgAlpha));
        
        // Fade in sprite (scale or alpha)
        unsigned char spAlpha = static_cast<unsigned char>(255.f * ((progress * 3.f < 1.0f) ? (progress * 3.f) : 1.0f));
        overlaySprite.setColor(sf::Color(255, 255, 255, spAlpha));
        
        // Scale logic: 48x48 centered
        // Let's scale it to make it pop! Base scale 5x or 10x? Let's say 10x (480x480)
        float currentScale = 10.f * ((progress * 3.f < 1.0f) ? (progress * 3.f) : 1.0f);
        overlaySprite.setScale({currentScale, currentScale});

        // Center on screen perceived playable area
        sf::FloatRect bounds = overlaySprite.getLocalBounds();
        overlaySprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
        overlaySprite.setPosition({845.f, 475.f}); // Visual center of the room instead of window center

        // Fade and scale text
        overlayText.setFillColor(sf::Color(255, 255, 255, spAlpha));
        overlayText.setOutlineColor(sf::Color(0, 0, 0, spAlpha));
        
        // Center text under the sprite (e.g. 480/2 + a bit of padding = ~260px down)
        sf::FloatRect textBounds = overlayText.getLocalBounds();
        overlayText.setOrigin({textBounds.size.x / 2.0f, 0});
        overlayText.setPosition({845.f, 475.f + (bounds.size.y * currentScale / 2.0f) + 20.f});

        if (gameState.overlayTimer <= 0.0f) {
            gameState.overlayActive = false;
            player.isControlDisabled = false;
            overlaySprite.setColor(sf::Color(255, 255, 255, 0)); // hide
            overlayText.setFillColor(sf::Color(255, 255, 255, 0)); 
            overlayText.setOutlineColor(sf::Color(0, 0, 0, 0));
        }
        // DO NOT RETURN HERE! Otherwise we skip floor collisions and player falls!
    }
    
    // --- Interaction Update ---
    if (showInteraction) {
        interactionTimer -= deltaTime;
        
        // Let's implement a gentle fade out in the last 0.5 seconds
        if (interactionTimer <= 500.f) {
            float fadeProgress = ((interactionTimer / 500.f) > 0.f) ? (interactionTimer / 500.f) : 0.f;
            unsigned char a = static_cast<unsigned char>(255.f * fadeProgress);
            interactionText.setFillColor(sf::Color(255, 255, 255, a));
            interactionText.setOutlineColor(sf::Color(0, 0, 0, a));
        } else {
            interactionText.setFillColor(sf::Color(255, 255, 255, 255));
            interactionText.setOutlineColor(sf::Color(0, 0, 0, 255));
        }
        
        if (interactionTimer <= 0.0f) {
            showInteraction = false;
        }
    }
   
    tablePuzzleTwo.Update(deltaTime);
    tablePuzzleFour.Update(deltaTime);
    chestClosed.Update(deltaTime);
    pedestalItem.Update(deltaTime); // Update pedestal (just to be safe)
    keyItem.Update(deltaTime); // Update animation
    
    chestOpen.Update(deltaTime);

    // Disable E key press if overlay is active
    bool eKeyIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && !gameState.overlayActive;

    for(int i = 0; i < (int)doorSprites.size(); i++) {
        if (mainDoorsColored) {
            sf::Color base = doorColors[i];
            doorSprites[i].setColor(sf::Color(base.r * 0.8f, base.g * 0.8f, base.b * 0.8f)); 
        } else {
            doorSprites[i].setColor(sf::Color(150, 150, 150));
        }
    }
//ALL COLLISION
    if(activeRoom == RoomID::MainRoom) {
        for(int i = 0; i < (int)doorSprites.size(); i++) {
            // Check collision using findIntersection
            if(player.getHitBox().findIntersection(getDoorHitbox(i))) {
                
                // Highlight the door
                if (mainDoorsColored) {
                    doorSprites[i].setColor(doorColors[i]); 
                } else {
                    doorSprites[i].setColor(sf::Color::White);
                }

                if (eKeyIsPressed && !eKeyWasPressed) {
                    std::cout << "Interacting with door: " << i << std::endl; 
                    handleDoorChoice(i, player, gameState, playerData, audioManager);
                    eKeyWasPressed = true; // Prevent immediate interaction inside the new room
                    return; // Skip remaining room logic this frame
                }
            }
        }
    }
    else {
        
        exitDoorSprite.setColor(sf::Color(150, 150, 150)); // Dim by default
        
        if (player.getHitBox().findIntersection(exitZoneBox.getGlobalBounds())) {
            
            exitDoorSprite.setColor(sf::Color::White); 
            
            if (eKeyIsPressed && !eKeyWasPressed) {
                fixReturnPosition(player); 
                changeRoom(RoomID::MainRoom, player, audioManager);
                eKeyWasPressed = true; // Prevent immediate interaction back in Hub
                return; // Skip remaining room logic this frame
            }
        }
    }
   
    
    

if (activeRoom == RoomID::SnakeRoom) {
    
    
    if (sequenceManager.isSequenceActive) {
        int previousStep = sequenceManager.getCurrentStep();
        sequenceManager.update(deltaTime, player, snakeRoomEnemy);
        
        // --- DELAYED DOOR SLAM --- 
        // Trigger precisely when the player teleports to the door (transition to Step 1)
        if (previousStep == 0 && sequenceManager.getCurrentStep() == 1) {
            audioManager.playDoorSlamSound();
        }
    } 
    else {
        
        if (sequenceManager.activeSequence == SequenceID::SnakeRoomSeq) {
        
            fixReturnPosition(player);
            changeRoom(RoomID::MainRoom, player, audioManager);
            
           
            sequenceManager.activeSequence = SequenceID::None; 
        } 
        else {
            if (sequenceManager.activeSequence == SequenceID::SnakeRoomSeqWithPotion) {
                sequenceManager.activeSequence = SequenceID::None; 
            }
            
        }
    }
}


if (activeRoom == RoomID::keyLockRoom) {
    tablePuzzleTwo.Sprite.setColor(sf::Color(150, 150, 150));
    if(player.getHitBox().findIntersection(tablePuzzleTwo.getHitBox())){
        tablePuzzleTwo.Sprite.setColor(sf::Color::White);
        if(eKeyIsPressed && !eKeyWasPressed){
            audioManager.playInteractSound();
            if (!rogbyCompleted) {
                if (!showROGBYPopup) {
                    // Start the riddle slow print (and replay if requested)
                    fullRiddleText = "  +------------------------------------------------+\n\n"
                                     "    The  answer  is  already  before  your  eyes.\n"
                                     "    Read  the  doors  in  order.\n"
                                     "    The  first  signs  will  reveal  the  path.\n\n"
                                     "  +------------------------------------------------+\n\n";
                    if (!rogbySeenOnce) {
                        currentRiddleText = "";
                        riddleCharIndex = 0;
                        riddleCharTimer = 0.f;
                        riddlePauseTimer = 0.f;
                        riddleDarkenAlpha = 0.f;
                        riddleFinishedWriting = false;
                        rogbySeenOnce = true;
                        
                        // Activate door colors immediately when first reading the hint at table 2
                        mainDoorsColored = true;
                        pendingMysterySound = true; // Queue the sound to play on exit
                    } else {
                        currentRiddleText = fullRiddleText;
                        riddleCharIndex = fullRiddleText.length();
                        riddleDarkenAlpha = 220.f;
                        riddleFinishedWriting = true;
                    }
                    riddleIsWriting = true;
                    showROGBYPopup = true; // Actively lock screen right away
                }
            } else {
                showROGBYPopup = true;
            }
        }
    }
    else{
        tablePuzzleTwo.isInteracting = false;
    }
} else {
    tablePuzzleTwo.isInteracting = false;
}
    
if (activeRoom == RoomID::chestcombinationRoom) {
    tablePuzzleFour.Sprite.setColor(sf::Color(150, 150, 150));
    if(player.getHitBox().findIntersection(tablePuzzleFour.getHitBox())){
        tablePuzzleFour.Sprite.setColor(sf::Color::White);
        if(eKeyIsPressed && !eKeyWasPressed){
            audioManager.playInteractSound();
            showRiddlePopup = true; // Play ducks riddle
        }
    }
    else{
        tablePuzzleFour.isInteracting = false;
    }
} else {
    tablePuzzleFour.isInteracting = false;
}
   
if (activeRoom == RoomID::chestToolroom) {
    // Check our boolean: Is the chest closed?
    if (!chestClosed.isChestOpen) {
        chestClosed.Sprite.setColor(sf::Color(150, 150, 150)); 
        if (player.getHitBox().findIntersection(chestClosed.getHitBox())) {
            chestClosed.Sprite.setColor(sf::Color::White); 
            if (eKeyIsPressed && !eKeyWasPressed) {
                // Instead of opening straight away, trigger the pop-up
                showChestCombinationPopup = true;
            }
        }
    } 
    else {
        // --- CHEST IS OPEN ---
        if (!snakePotionItem.pickedUp && player.getHitBox().findIntersection(snakePotionItem.getHitBox())) {
            std::cout << "You found the Snake Killer Potion!\n";
            playerData.inv[ItemIndex::SNAKE_TOOL] = true;
            snakePotionItem.pickedUp = true;
            
            gameState.overlayActive = true;
            gameState.overlayTimer = 3000.f; // 3 seconds
            gameState.currentOverlay = ItemIndex::SNAKE_TOOL;
            overlaySprite.setTexture(potionOverlayTexture, true);
            overlayText.setString("Obtained the potion!");
            audioManager.playObtainedItemSound();
        }

        if (player.getHitBox().findIntersection(chestClosed.getHitBox())) {
            chestClosed.Sprite.setColor(sf::Color::White);
        }
        else {
            chestClosed.Sprite.setColor(sf::Color(150, 150, 150));  // Dim it when not interacting
        }
    }
}

    // Key Collection Collision 
    if (activeRoom == RoomID::SnakeRoom && !keyItem.pickedUp && player.getHitBox().findIntersection(keyItem.getHitBox())) {
        std::cout << "You found the key!\n";
        playerData.hasKey = true;
        playerData.inv[ItemIndex::KEY] = true;
        keyItem.pickedUp = true; 
        
        
        gameState.overlayActive = true;
        gameState.overlayTimer = 3000.f; 
        gameState.currentOverlay = ItemIndex::KEY;
        overlaySprite.setTexture(keyOverlayTexture, true);
        overlayText.setString("Obtained the key!");
        audioManager.playObtainedItemSound();
    }


    // Floor Collision
    if (player.getHitBox().findIntersection(getFloorHitbox())) {
        sf::FloatRect playerBounds = player.getHitBox();
        sf::FloatRect floorBounds = getFloorHitbox();
        
        float floorTop = floorBounds.position.y;
        float playerBottom = playerBounds.position.y + playerBounds.size.y;
        
        float overlapDistance = playerBottom - floorTop;
        player.sprite.setPosition({player.sprite.getPosition().x, player.sprite.getPosition().y - overlapDistance});
        player.boundRect.setPosition(player.sprite.getPosition());
        player.setVelocityY(0.f); 
    }
    
    if (player.getHitBox().findIntersection(leftWall.getGlobalBounds())) {
        sf::FloatRect playerBounds = player.getHitBox();
        sf::FloatRect wallBounds = leftWall.getGlobalBounds();
        player.sprite.setPosition({wallBounds.position.x + wallBounds.size.x + playerBounds.size.x / 2.f, player.sprite.getPosition().y});
        player.boundRect.setPosition(player.sprite.getPosition());
    }
    
    if (player.getHitBox().findIntersection(rightWall.getGlobalBounds())) {
        sf::FloatRect playerBounds = player.getHitBox();
        sf::FloatRect wallBounds = rightWall.getGlobalBounds();
        player.sprite.setPosition({wallBounds.position.x - playerBounds.size.x / 2.f, player.sprite.getPosition().y});
        player.boundRect.setPosition(player.sprite.getPosition());
    }
    
    if (player.getHitBox().findIntersection(topWall.getGlobalBounds())) {
        sf::FloatRect playerBounds = player.getHitBox();
        sf::FloatRect wallBounds = topWall.getGlobalBounds();
        player.sprite.setPosition({player.sprite.getPosition().x, wallBounds.position.y + wallBounds.size.y + playerBounds.size.y / 2.f});
        player.boundRect.setPosition(player.sprite.getPosition());
    }

    if (activeRoom == RoomID::SnakeRoom && !snakeRoomEnemy.isDead) {
        snakeRoomEnemy.Update(deltaTime);

        // Apply collision detection for the enemy
        snakeRoomEnemy.handleFloorCollision(getFloorHitbox());
        snakeRoomEnemy.handleLeftWallCollision(getLeftWallHitbox());
        snakeRoomEnemy.handleRightWallCollision(getRightWallHitbox());
        snakeRoomEnemy.handleTopWallCollision(getTopWallHitbox());
        
        // Player vs Enemy Collision
        if (!sequenceManager.isSequenceActive && player.getHitBox().findIntersection(snakeRoomEnemy.getHitBox())) {
            if (playerData.inv[ItemIndex::SNAKE_TOOL]) {
                std::cout << "You used the Snake Killer Potion! The enemy is dead!\n";
                // --- DEATH SOUND PLAY ---
                audioManager.playDeathSound();
                snakeRoomEnemy.isDead = true;
            } else {
                std::cout << "💀 OUCH! The Guard caught you!\n";
                // Teleport player to the door as a penalty
                player.sprite.setPosition({150.f, player.sprite.getPosition().y});
            }
        }
    }

    // --- CHASE MUSIC LOGIC ---
    if (activeRoom == RoomID::SnakeRoom && !snakeRoomEnemy.isDead) {
        if (snakeRoomEnemy.isRunning) {
            audioManager.playChaseMusic();
        }
    } else {
        audioManager.stopChaseMusic();
    }

    // Draw the ImGui chest combination popup if active
    if (showChestCombinationPopup) {
        player.isControlDisabled = true; // freeze player
        
        // Center the window on screen
        ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f / 2.f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(400.f, 200.f), ImGuiCond_Appearing);
        
        ImGui::Begin("Locked Chest", &showChestCombinationPopup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        
        ImGui::SetWindowFontScale(1.5f); // Scale text inside safely
        
        ImGui::Text("Enter the 4-digit passcode:");
        
        static char passcodeInput[16] = "";  // Need this to persist across frames
        
        // Auto-focus the input box so the user doesn't have to click
        if (ImGui::IsWindowAppearing()) {
            ImGui::SetKeyboardFocusHere();
        }

        if (ImGui::InputText("##Passcode", passcodeInput, 16, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) {
            if (std::string(passcodeInput) == "3279") {
                // Chest unlocks!
                std::cout << "Chest Opened!\n";
                chestClosed.SwitchToAlternate();
                chestClosed.isChestOpen = true;
                player.chestUnlocked = true;
                if (!hasOpenedChestOnce) {
                    audioManager.playChestOpenSound();
                    hasOpenedChestOnce = true;
                }
                showChestCombinationPopup = false; 
                player.isControlDisabled = false;
            } else {
                std::cout << "Incorrect Passcode!\n";
                // Optionally clear the input or play error sound
                passcodeInput[0] = '\0';
            }
        }
        
        // Also provide a button in case they click instead of hitting Enter
        if (ImGui::Button("Unlock", ImVec2(120, 40))) {
            if (std::string(passcodeInput) == "3279") {
                std::cout << "Chest Opened!\n";
                chestClosed.SwitchToAlternate();
                chestClosed.isChestOpen = true;
                player.chestUnlocked = true;
                if (!hasOpenedChestOnce) {
                    audioManager.playChestOpenSound();
                    hasOpenedChestOnce = true;
                }
                showChestCombinationPopup = false;
                player.isControlDisabled = false;
            } else {
                std::cout << "Incorrect Passcode!\n";
                passcodeInput[0] = '\0';
            }
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel", ImVec2(120, 40))) {
            showChestCombinationPopup = false;
            player.isControlDisabled = false;
            passcodeInput[0] = '\0';
        }
        
        // Re-enable player if closed manually via "X" button
        if (!showChestCombinationPopup) {
            player.isControlDisabled = false;
        }

        ImGui::End();
    }

    // Draw the ImGui door combination popup if active
    if (showDoorCombinationPopup) {
        player.isControlDisabled = true; // freeze player
        
        // Center the window on screen
        ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f / 2.f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(400.f, 200.f), ImGuiCond_Appearing);
        
        ImGui::Begin("Locked Door", &showDoorCombinationPopup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        
        ImGui::SetWindowFontScale(1.5f); // Scale text inside safely
        
        ImGui::Text("Enter the 4-digit passcode:");
        
        static char doorPasscodeInput[16] = "";  // Need this to persist across frames
        
        // Auto-focus the input box so the user doesn't have to click
        if (ImGui::IsWindowAppearing()) {
            ImGui::SetKeyboardFocusHere();
        }

        if (ImGui::InputText("##DoorPasscode", doorPasscodeInput, 16, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) {
            if (std::string(doorPasscodeInput) == "4729") {
                std::cout << "Door Unlocked!\n";
                // audioManager.playDoorOpenSound(); // Optional if it exists
                gameState.visited[2] = true;
                changeRoom(RoomID::chestToolroom, player, audioManager);
                showDoorCombinationPopup = false; 
                player.isControlDisabled = false;
                doorPasscodeInput[0] = '\0';
            } else {
                std::cout << "Incorrect Passcode!\n";
                doorPasscodeInput[0] = '\0';
                audioManager.playDeniedSound();
            }
        }
        
        if (ImGui::Button("Unlock", ImVec2(120, 40))) {
            if (std::string(doorPasscodeInput) == "4729") {
                std::cout << "Door Unlocked!\n";
                gameState.visited[2] = true;
                changeRoom(RoomID::chestToolroom, player, audioManager);
                showDoorCombinationPopup = false; 
                player.isControlDisabled = false;
                doorPasscodeInput[0] = '\0';
            } else {
                std::cout << "Incorrect Passcode!\n";
                doorPasscodeInput[0] = '\0';
                audioManager.playDeniedSound();
            }
        }
        
        ImGui::SameLine();
        
        if (ImGui::Button("Cancel", ImVec2(120, 40))) {
            showDoorCombinationPopup = false;
            player.isControlDisabled = false;
            doorPasscodeInput[0] = '\0';
        }
        
        // Re-enable player if closed manually via "X" button
        if (!showDoorCombinationPopup) {
            player.isControlDisabled = false;
        }

        ImGui::End();
    }

    // Draw the ImGui riddle popup if active
    if (showRiddlePopup) {
        player.isControlDisabled = true; // freeze player
        
        struct Question {
            std::string text;
            std::string correctAnswer;
        };
        
        static std::vector<Question> gameStage = {
            {"How many ducks are in this scenario: 2 ducks in front of a duck, 2 ducks behind a duck, and 1 duck in the middle?", "3"},
            {"If you are running a race and you pass the person in second place, what place are you in?", "2"},
            {"If A = 1, B = 2, C = 3, etc., what is the sum of 'BAD'?", "7"},
            {"If you rotate the number 6 180 degrees, what number do you get?", "9"}
        };
        
        // Center the window on screen
        ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f / 2.f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(800.f, 300.f), ImGuiCond_Appearing);
        
        ImGui::Begin("Table Riddle", &showRiddlePopup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        
        ImGui::SetWindowFontScale(1.5f); // Scale text inside safely
        
        if (riddlesCompleted) {
            ImGui::TextWrapped("Alas, thou hast conquered the trial of wits and bested the table's riddles!");
            ImGui::TextWrapped("\nThy code of passage is: 3279");
            if (ImGui::Button("Close", ImVec2(120, 40))) {
                showRiddlePopup = false;
                player.isControlDisabled = false;
            }
        } else {
            ImGui::TextWrapped("Question %d/4:", currentRiddle + 1);
            ImGui::TextWrapped("%s", gameStage[currentRiddle].text.c_str());
            
            static char riddleInput[16] = "";  // Need this to persist across frames
            static bool setFocus = false;
            
            // Auto-focus the input box down so the user doesn't have to click
            if (ImGui::IsWindowAppearing() || setFocus) {
                ImGui::SetKeyboardFocusHere();
                setFocus = false;
            }

            if (ImGui::InputText("##RiddleAnswer", riddleInput, 16, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) {
                if (std::string(riddleInput) == gameStage[currentRiddle].correctAnswer) {
                    std::cout << "Correct!\n";
                    audioManager.playInteractSound();
                    currentRiddle++;
                    riddleInput[0] = '\0';
                    setFocus = true;
                    
                    if (currentRiddle >= gameStage.size()) {
                        riddlesCompleted = true;
                        std::cout << "All riddles solved!\n";
                    }
                } else {
                    std::cout << "Incorrect Answer!\n";
                    riddleInput[0] = '\0';
                    setFocus = true;
                    audioManager.playDeniedSound();
                }
            }
            
            if (ImGui::Button("Submit", ImVec2(120, 40))) {
                if (std::string(riddleInput) == gameStage[currentRiddle].correctAnswer) {
                    std::cout << "Correct!\n";
                    audioManager.playInteractSound();
                    currentRiddle++;
                    riddleInput[0] = '\0';
                    setFocus = true;
                    
                    if (currentRiddle >= gameStage.size()) {
                        riddlesCompleted = true;
                        std::cout << "All riddles solved!\n";
                    }
                } else {
                    std::cout << "Incorrect Answer!\n";
                    riddleInput[0] = '\0';
                    setFocus = true;
                    audioManager.playDeniedSound();
                }
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Cancel", ImVec2(120, 40))) {
                showRiddlePopup = false;
                player.isControlDisabled = false;
                riddleInput[0] = '\0';
            }
        }
        
        // Re-enable player if closed manually via "X" button
        if (!showRiddlePopup) {
            player.isControlDisabled = false;
        }

        ImGui::End();
    }

    // Draw the ImGui ROGBY popup if active
    if (showROGBYPopup) {
        player.isControlDisabled = true; // freeze player
        
        if (riddleIsWriting) {
            // Screen darken fade logic
            if (riddleDarkenAlpha < 220.f) {
                riddleDarkenAlpha += deltaTime * 0.2f;
                if (riddleDarkenAlpha > 220.f) riddleDarkenAlpha = 220.f;
            }
            
            if (!riddleFinishedWriting) {
                // Update logic for typing the riddle character by character
                if (riddlePauseTimer > 0.f) {
                    riddlePauseTimer -= deltaTime;
                } else if (riddleCharIndex < fullRiddleText.length()) {
                    riddleCharTimer -= deltaTime;
                    if (riddleCharTimer <= 0.f) {
                        char c = fullRiddleText[riddleCharIndex];
                        currentRiddleText += c;
                        riddleCharIndex++;
                        
                        if (c == '.' || c == '\n') {
                            riddlePauseTimer = 500.f; // Add delay at the end of sentence/line
                        } else {
                            riddleCharTimer = 20.f; // normal delay between letters
                        }
                    }
                } else {
                    riddleFinishedWriting = true;
                }
                riddleScreenText.setString(currentRiddleText);
            } else {
                // Show choice buttons AFTER finishing typing
                ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f - 200.f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ImGui::Begin("RiddleAction", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
                ImGui::SetWindowFontScale(1.8f);
                if (ImGui::Button("Enter Code", ImVec2(200, 60))) {
                    riddleIsWriting = false; // Transition to normal prompt
                }
                ImGui::SameLine(0, 50.f);
                if (ImGui::Button("Cancel", ImVec2(200, 60))) {
                    showROGBYPopup = false;
                    riddleIsWriting = false;
                    player.isControlDisabled = false;
                }
                ImGui::End();
            }
        } else {
            // ImGui popup for actual riddle text input
            ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f / 2.f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(800.f, 300.f), ImGuiCond_Appearing);
            
            ImGui::Begin("Table Riddle", &showROGBYPopup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
            
            ImGui::SetWindowFontScale(1.5f); // Scale text inside safely
            
            if (rogbyCompleted) {
                ImGui::TextWrapped("Alas, thou hast conquered the trial of wits and bested the table's riddles!");
                ImGui::TextWrapped("\nThy code of passage is: 4729");
                if (ImGui::Button("Close", ImVec2(120, 40))) {
                    showROGBYPopup = false;
                    player.isControlDisabled = false;
                }
            } else {
                ImGui::TextWrapped("What is the answer to the colored doors?");
                
                static char rogbyInput[16] = "";  // Need this to persist across frames
                static bool setFocus = false;
                
                // Auto-focus the input box down so the user doesn't have to click
                if (ImGui::IsWindowAppearing() || setFocus) {
                    ImGui::SetKeyboardFocusHere();
                    setFocus = false;
                }

                if (ImGui::InputText("##ROGBYAnswer", rogbyInput, 16, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsUppercase)) {
                    if (std::string(rogbyInput) == "ROGBY") {
                        std::cout << "Correct!\n";
                        audioManager.playInteractSound();
                        rogbyInput[0] = '\0';
                        setFocus = true;
                        rogbyCompleted = true;
                    } else {
                        std::cout << "Incorrect Answer!\n";
                        rogbyInput[0] = '\0';
                        setFocus = true;
                        audioManager.playDeniedSound();
                    }
                }
                
                if (ImGui::Button("Submit", ImVec2(120, 40))) {
                    if (std::string(rogbyInput) == "ROGBY") {
                        std::cout << "Correct!\n";
                        audioManager.playInteractSound();
                        rogbyInput[0] = '\0';
                        setFocus = true;
                        rogbyCompleted = true;
                    } else {
                        std::cout << "Incorrect Answer!\n";
                        rogbyInput[0] = '\0';
                        setFocus = true;
                        audioManager.playDeniedSound();
                    }
                }
                
                ImGui::SameLine();
                
                if (ImGui::Button("Cancel", ImVec2(120, 40))) {
                    showROGBYPopup = false;
                    player.isControlDisabled = false;
                    rogbyInput[0] = '\0';
                }
            }
            
            // Re-enable player if closed manually via "X" button
            if (!showROGBYPopup) {
                player.isControlDisabled = false;
            }

            ImGui::End();
        }
    }


    if (!showROGBYPopup && pendingMysterySound) {
        audioManager.playMysteryFoundSound();
        pendingMysterySound = false;
    }

    eKeyWasPressed = eKeyIsPressed;
}

void Rooms::draw(sf::RenderWindow& window, const GameState& gameState)
{
    // 1. Draw Background Image
    window.draw(backgroundSprite); 
    
    // Draw exit doors first so they appear behind the floor (Plan 1.png in side rooms)
    if (activeRoom != RoomID::MainRoom && activeRoom != RoomID::winScreen && activeRoom != RoomID::puzzleOverlay) {
        window.draw(exitDoorSprite); 
    }

    // 2. Draw Floor Image and Walls
    window.draw(floorSprite);
    window.draw(wallSprite);
    
    // 3. Draw Doors/Exits and Interactables
    if (activeRoom == RoomID::MainRoom) {
        for (const auto& door : doorSprites) {
            window.draw(door);
        }
    } 
    else if(activeRoom == RoomID::keyLockRoom) {
        tablePuzzleTwo.Draw(window);
    }
    else if(activeRoom == RoomID::chestcombinationRoom){
        tablePuzzleFour.Draw(window);
    }
    else if(activeRoom == RoomID::chestToolroom){
        chestClosed.Draw(window);  // Always draw chestClosed, it switches texture when opened
        
        if (chestClosed.isChestOpen && !snakePotionItem.pickedUp) {
            snakePotionItem.Draw(window);
        }
    }
    else if(activeRoom == RoomID::SnakeRoom){
        snakeRoomEnemy.Draw(window);
        pedestalItem.Draw(window);
        keyItem.Draw(window); 
    }

}

void Rooms::drawOverlay(sf::RenderWindow& window, const GameState& gameState)
{
    // --- DRAW OVERLAY LAST (ON TOP OF EVERYTHING) ---
    if (gameState.overlayActive) {
        window.draw(overlayBackground);
        window.draw(overlaySprite);
        window.draw(overlayText);
    }
    
    // Draw Riddle writing effect screen darken
    if (showROGBYPopup && riddleIsWriting) {
        sf::RectangleShape darken(sf::Vector2f(1920.f, 1080.f));
        // Use the animated alpha for a dramatic fade instead of immediate
        darken.setFillColor(sf::Color(0, 0, 0, static_cast<unsigned char>(riddleDarkenAlpha)));
        window.draw(darken);
        
        // Only draw the text if it has content, helps avoid small dot
        if (!currentRiddleText.empty()) {
            // Center text on screen
            sf::FloatRect textBounds = riddleScreenText.getLocalBounds();
            riddleScreenText.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
            riddleScreenText.setPosition({1920.f / 2.f, 1080.f / 2.f});
            
            // Fade text in alongside darkness for a cooler effect
            unsigned char textAlpha = static_cast<unsigned char>(255.f * (riddleDarkenAlpha / 220.f));
            riddleScreenText.setFillColor(sf::Color(255, 255, 255, textAlpha));
            riddleScreenText.setOutlineColor(sf::Color(0, 0, 0, textAlpha));
            
            window.draw(riddleScreenText);
        }
    }
    
    // Draw Interaction Message on top
    if (showInteraction) {
        window.draw(interactionText);
    }
}

void Rooms::showInteractionMessage(const std::string& message) {
    interactionText.setString(message);
    
    // Bottom-right positioning
    sf::FloatRect textBounds = interactionText.getLocalBounds();
    interactionText.setOrigin({textBounds.size.x, textBounds.size.y});

    interactionText.setPosition({1640.f, 920.f}); 

    showInteraction = true;
    interactionTimer = 3000.f; // 3 seconds
}

void Rooms::changeRoom(RoomID id, Player& player, AudioManager& audioManager)
{
    // --- AUDIO TRIGGERS ---
    // Exiting a room (returning to MainRoom)
    if (activeRoom != RoomID::MainRoom && id == RoomID::MainRoom) {
        audioManager.playDoorCloseSound();
    }
    // Entering a room (leaving MainRoom)
    else if (activeRoom == RoomID::MainRoom && id != RoomID::MainRoom) {
        if (id == RoomID::SnakeRoom) {
            // DO NOT play doorSlamSound here. We delay it until the sequence hits Step 1!
        } else {
            audioManager.playDoorOpenSound();
        }
    }

    // 1. Update the active room state
    activeRoom = id;
    
    // 2. Handle Player Positioning
    if (activeRoom != RoomID::MainRoom && 
        activeRoom != RoomID::winScreen && 
        activeRoom != RoomID::puzzleOverlay) 
    {
        // Store current position so they return to the correct door later
        playerEntryPosition = player.sprite.getPosition();
        
        // Push player to the left side of the new room so they don't immediately trigger the exit!
        player.sprite.setPosition({150.f, player.sprite.getPosition().y}); 
    }

    // 3. Swap the Floor and Background Textures
    if (activeRoom == RoomID::MainRoom) {
       
        floorSprite.setTexture(mainFloorTexture, true);
        backgroundSprite.setTexture(mainRoomTexture, true); 
        floorSprite.setPosition({0.f, -20.f});
    } 
    else {
        // --- WE ARE IN A SIDE ROOM ---
        
        // All side rooms use the "Plan 1" floor
        floorSprite.setTexture(sideFloorTexture, true);
        floorSprite.setPosition({0.f, -90.f});
        
        // Find exactly which side room we are in to set the background
        switch(activeRoom)
        {
            case RoomID::SnakeRoom:
                backgroundSprite.setTexture(snakeRoomTexture, true);

                break;
            case RoomID::ExitRoom:
                backgroundSprite.setTexture(exitRoomTexture, true);
                break;
            case RoomID::chestToolroom:
                backgroundSprite.setTexture(chestToolroomTexture, true);
                if (!player.chestUnlocked) {
                    chestClosed.isChestOpen = false;  // Keep closed if it wasn't unlocked
                    chestClosed.ResetToDefault();
                } else {
                    chestClosed.isChestOpen = true;  // Keep opened
                    chestClosed.SwitchToAlternate();
                }
                break;
            case RoomID::keyLockRoom:
                backgroundSprite.setTexture(keyLockRoomTexture, true);
                break;
            case RoomID::chestcombinationRoom:
                backgroundSprite.setTexture(chestcombinationRoomTexture, true);
                break;
            case RoomID::puzzleOverlay:
                backgroundSprite.setTexture(puzzleOverlayTexture, true);
                break;
            case RoomID::winScreen:
                backgroundSprite.setTexture(winScreenTexture, true);
                break;
            default:
                break;
        }
    }
}

//collision detection for floor and doors
sf::FloatRect Rooms::getFloorHitbox() const { return floor.getGlobalBounds(); }
sf::FloatRect Rooms::getDoorHitbox(int doorIndex) const
{
    if (doorIndex >= 0 && doorIndex < static_cast<int>(doorSprites.size())) {
        return doorSprites[doorIndex].getGlobalBounds();
    }
    return sf::FloatRect();
}


void Rooms::handleDoorChoice(int index, Player& player, GameState& gameState, PlayerData& playerData, AudioManager& audioManager)
{
    // Door layout: 0 = Exit, 1-4 = other rooms
    switch(index) {
        case 0:
        if(playerData.hasKey) {
            gameState.Escaped = true; // Tell main.cpp to trigger the win sequence
        } else {
            showInteractionMessage("This door is locked, should look around");
            std::cout << "The door is locked. You need a key to enter.\n";
            audioManager.playDeniedSound();
        }
        break;
        case 1:
            if (!gameState.visited[0]) {
                changeRoom(RoomID::SnakeRoom, player, audioManager);
                if (!playerData.inv[ItemIndex::SNAKE_TOOL]) {
                    sequenceManager.triggerSequence(SequenceID::SnakeRoomSeq);
                } else {
                    std::cout << "Triggering the fast potion sequence!\n";
                    if (!snakeRoomEnemy.isDead) { // Just in case it's possible to have both
                        sequenceManager.triggerSequence(SequenceID::SnakeRoomSeqWithPotion);
                    }
                }
                gameState.visited[0] = true;
            } else if (playerData.inv[ItemIndex::SNAKE_TOOL]) {
                // If the player has the Snake Tool, they can re-enter safely
                changeRoom(RoomID::SnakeRoom, player, audioManager);
                if (!snakeRoomEnemy.isDead) {
                    sequenceManager.triggerSequence(SequenceID::SnakeRoomSeqWithPotion);
                }
            } else {
                // Already visited, but NO tool
                showInteractionMessage("Rather eat bugs than see him again");
                std::cout << "Can't enter. I need a tool!\n";
                audioManager.playDeniedSound();
            }
            break;
        case 2:
            if (!gameState.visited[2]) {
                showDoorCombinationPopup = true;
            } else {
                changeRoom(RoomID::chestToolroom, player, audioManager);
            }
            break;
        case 3:
            // showInteractionMessage("The door is locked"); // (Disabled text for now)
            changeRoom(RoomID::keyLockRoom, player, audioManager);
            gameState.visited[3] = true;
            break;
        case 4:
            // showInteractionMessage("The door is locked"); // (Disabled text for now)
            changeRoom(RoomID::chestcombinationRoom, player, audioManager);
            gameState.visited[4] = true;
            break;
    }
}

void Rooms::fixReturnPosition(Player& player)
{
    // Return player to the door they entered from
    player.sprite.setPosition(playerEntryPosition);
    player.boundRect.setPosition(player.sprite.getPosition());
}