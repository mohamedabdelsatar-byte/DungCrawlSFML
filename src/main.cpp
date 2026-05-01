#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <Player.h>
#include <Enemy.h>
#include <Rooms.h>
#include "AudioManager.h"
#include "MainMenu.h"

bool Escaped = false;

enum class AppState { Intro, MainMenu, Tutorial, PreGameIntro, Playing, Paused, WinSequence };
enum class FadeState { IntroFadingIn, IntroHold, IntroFadingOut, 
    IntroPause, None, FadingInMenu, FadingOutMenuToPlay, FadingInTutorial, TutorialWait, FadingOutTutorial, FadingInGame, FadingWhite };

int main(){
//----------------------- INITIALIZE ------------------------------
sf::RenderWindow window(sf::VideoMode({1920,1080}), "Dungeon Crawler");
window.setFramerateLimit(120);

ImGui::SFML::Init(window);

AudioManager audioManager;
Rooms rooms;
Player player;
GameState gameState;
PlayerData playerData;
MainMenu mainMenu;
AppState currentAppState = AppState::Intro;
mainMenu.init(window);

FadeState currentFadeState = FadeState::IntroFadingIn;
float fadeAlpha = 255.0f;
float fadeSpeed = 0.35f; 
float introTimer = 0.0f;

int winPhase = 0;
float winTimer = 0.0f;
size_t winCharIndex = 0;
std::string currentWinLineText = "";
float gameSongVolume = 10.0f; // Track game song volume
sf::Color currentFadeColor = sf::Color(0, 0, 0, 255);

sf::RectangleShape fadeRect(window.getDefaultView().getSize());
fadeRect.setFillColor(sf::Color(0, 0, 0, 255));

sf::Font pixelyFont;
if (!pixelyFont.openFromFile("Assets/Fonts/ARCADECLASSIC.TTF")) {
    std::cout << "Failed to load ARCADECLASSIC.TTF" << std::endl;
}
sf::Text preGameText(pixelyFont, "", 40);
preGameText.setFillColor(sf::Color::White);
sf::Vector2f viewCenter = window.getDefaultView().getCenter();
float preGameLineStart = viewCenter.y - 100.f;

int preGamePhase = 0;
float preGameCharTimer = 0.0f;
size_t preGameCharIndex = 0;
std::string currentLineText = "";
char nameBuffer[64] = "";
std::vector<std::string> preGameLines;

//----------------------- INITIALIZE ------------------------------

// Tutorial Screen elements
sf::Texture wasdTex, eTex, tutDoorTex;
if(!wasdTex.loadFromFile("Assets/Textures/WASD1.png")) std::cout << "Missing WASD1.png\n";
if(!eTex.loadFromFile("Assets/Textures/E.png")) std::cout << "Missing E.png\n";
if(!tutDoorTex.loadFromFile("Assets/Textures/doors.png")) std::cout << "Missing doors.png\n";

sf::Sprite wasdSprite(wasdTex);
sf::Sprite eSprite(eTex);
sf::Sprite tutDoorSprite(tutDoorTex);

// Doors sprite is a sheet, normal door unlocked rect is 288, 64, 32, 64
tutDoorSprite.setTextureRect(sf::IntRect({288, 64}, {32, 64}));
tutDoorSprite.setScale({4.7f, 4.7f});
tutDoorSprite.setColor(sf::Color::White); // Glow filter

// Textures are reasonably sized usually, let's keep scale 1.0f or check if it needs scaling
wasdSprite.setScale({1.5f, 1.5f});
eSprite.setScale({1.5f, 1.5f});

sf::Text tutMovementText(pixelyFont, "[Movement]", 40);
tutMovementText.setFillColor(sf::Color::White);
sf::Text tutInteractText(pixelyFont, "[Interact]", 40);
tutInteractText.setFillColor(sf::Color::White);

sf::Texture tutWalkTex[4];
for(int i = 0; i < 4; i++){
    tutWalkTex[i].loadFromFile("Assets/Textures/player_walk" + std::to_string(i+1) + ".png");
}
sf::Sprite tutPlayerMove(tutWalkTex[0]);
tutPlayerMove.setScale({4.0f, 4.0f});

sf::Texture tutIdleTex;
tutIdleTex.loadFromFile("Assets/Textures/player_idle1.png");
sf::Sprite tutPlayerInteract(tutIdleTex);
tutPlayerInteract.setScale({4.0f, 4.0f});

sf::Text tutContinueText(pixelyFont, "Press any key to continue...", 30);
tutContinueText.setFillColor(sf::Color(150, 150, 150));

// Centers and Origins
tutPlayerMove.setOrigin({tutPlayerMove.getLocalBounds().size.x / 2.0f, tutPlayerMove.getLocalBounds().size.y}); // use center-bottom for alignment
tutPlayerMove.setPosition({viewCenter.x - 400.0f, viewCenter.y - 50.0f});

tutMovementText.setOrigin({tutMovementText.getLocalBounds().size.x / 2.0f, tutMovementText.getLocalBounds().size.y / 2.0f});
tutMovementText.setPosition({viewCenter.x - 400.0f, viewCenter.y + 60.0f});

wasdSprite.setOrigin({wasdTex.getSize().x / 2.0f, wasdTex.getSize().y / 2.0f});
wasdSprite.setPosition({viewCenter.x - 400.0f, viewCenter.y + 200.0f});

// right side (interact) - Door+Player at top, text below them, E below text
tutDoorSprite.setOrigin({tutDoorSprite.getLocalBounds().size.x / 2.0f, tutDoorSprite.getLocalBounds().size.y});
tutDoorSprite.setPosition({viewCenter.x + 400.0f, viewCenter.y - 50.0f});

tutPlayerInteract.setOrigin({tutPlayerInteract.getLocalBounds().size.x / 2.0f, tutPlayerInteract.getLocalBounds().size.y});
tutPlayerInteract.setPosition({viewCenter.x + 400.0f, viewCenter.y - 50.0f});

tutInteractText.setOrigin({tutInteractText.getLocalBounds().size.x / 2.0f, tutInteractText.getLocalBounds().size.y / 2.0f});
tutInteractText.setPosition({viewCenter.x + 400.0f, viewCenter.y + 60.0f});

eSprite.setOrigin({eTex.getSize().x / 2.0f, eTex.getSize().y / 2.0f});
eSprite.setPosition({viewCenter.x + 400.0f, viewCenter.y + 200.0f});

tutContinueText.setOrigin({tutContinueText.getLocalBounds().size.x / 2.0f, tutContinueText.getLocalBounds().size.y / 2.0f});
tutContinueText.setPosition({viewCenter.x, viewCenter.y + 400.0f});

float tutAnimTimer = 0.0f;
int tutWalkFrame = 0;

//----------------------- LOAD ------------------------------

audioManager.loadAll();
// Intro handled below, bgMusic starts post-intro

rooms.load();
rooms.init();

player.Init();
player.InitDarkness();
player.Load();

//----------------------- LOAD ------------------------------

//----------------------- GAME LOOP ------------------------------
//----------------------- UPDATE ------------------------------

// ------- EVENT -------

sf::Clock clock;
while(window.isOpen()){

    sf::Time deltaTimeTimer = clock.restart();
    float deltaTime = deltaTimeTimer.asMilliseconds();

    // Fade logic
    if (currentFadeState == FadeState::IntroFadingIn) {
        fadeAlpha -= fadeSpeed * deltaTime;
        if (fadeAlpha <= 0.0f) { fadeAlpha = 0.0f; currentFadeState = FadeState::IntroHold; introTimer = 0.0f; }
    } else if (currentFadeState == FadeState::IntroHold) {
        fadeAlpha = 0.0f;
        introTimer += deltaTime;
        if (introTimer >= 2000.0f) { // 2 seconds
            currentFadeState = FadeState::IntroFadingOut;
        }
    } else if (currentFadeState == FadeState::IntroFadingOut) {
        fadeAlpha += fadeSpeed * deltaTime;
        if (fadeAlpha >= 255.0f) {
            fadeAlpha = 255.0f;
            currentFadeState = FadeState::IntroPause;
            introTimer = 0.0f;
        }
    } else if (currentFadeState == FadeState::IntroPause) {
        fadeAlpha = 255.0f;
        introTimer += deltaTime;
        if (introTimer >= 500.0f) { // 0.5 seconds
            currentFadeState = FadeState::FadingInMenu;
            currentAppState = AppState::MainMenu;
            audioManager.playBgMusic(); // Music starts here
        }
    } else if (currentFadeState == FadeState::FadingInMenu) {
        fadeAlpha -= fadeSpeed * deltaTime;
        if (fadeAlpha <= 0.0f) { fadeAlpha = 0.0f; currentFadeState = FadeState::None; }
    } else if (currentFadeState == FadeState::FadingOutMenuToPlay) {
        fadeAlpha += fadeSpeed * deltaTime;
        if (fadeAlpha >= 255.0f) {
            fadeAlpha = 255.0f;
            currentFadeState = FadeState::FadingInTutorial;
            currentAppState = AppState::Tutorial;
            // You can lower bg music when entering pregame, we don't need to do it here
        }
    } else if (currentFadeState == FadeState::FadingInTutorial) {
        fadeAlpha -= fadeSpeed * deltaTime;
        if (fadeAlpha <= 0.0f) { 
            fadeAlpha = 0.0f; 
            currentFadeState = FadeState::TutorialWait; 
        }
    } else if (currentFadeState == FadeState::TutorialWait) {
        // Just wait until skip
    } else if (currentFadeState == FadeState::FadingOutTutorial) {
        fadeAlpha += fadeSpeed * deltaTime;
        if (fadeAlpha >= 255.0f) {
            fadeAlpha = 255.0f;
            currentFadeState = FadeState::None;
            currentAppState = AppState::PreGameIntro;
            audioManager.bgMusic.setVolume(2.f); // Lower volume by 80%
        }
    } else if (currentFadeState == FadeState::FadingInGame) {
        fadeAlpha -= fadeSpeed * deltaTime;
        if (fadeAlpha <= 0.0f) { 
            fadeAlpha = 0.0f; 
            currentFadeState = FadeState::None; 
            gameSongVolume = 10.0f;
            audioManager.bgMusic.setVolume(gameSongVolume); 
        }
    } else if (currentFadeState == FadeState::FadingWhite) {
        // Nothing here, handled in AppState::WinSequence phase 1
    }
    
    currentFadeColor.a = static_cast<std::uint8_t>(fadeAlpha);
    fadeRect.setFillColor(currentFadeColor);

    while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();
            
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (currentAppState == AppState::Tutorial && currentFadeState == FadeState::TutorialWait) {
                    currentFadeState = FadeState::FadingOutTutorial;
                }
                
                if (keyEvent->code == sf::Keyboard::Key::Escape && currentFadeState == FadeState::None) {
                    if (currentAppState == AppState::Playing) {
                        currentAppState = AppState::Paused;
                        audioManager.pauseAll();
                    } else if (currentAppState == AppState::Paused) {
                        currentAppState = AppState::Playing;
                        audioManager.resumeAll();
                    }
                }
            }
            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (currentAppState == AppState::Tutorial && currentFadeState == FadeState::TutorialWait) {
                    currentFadeState = FadeState::FadingOutTutorial;
                }
            }
        }
    ImGui::SFML::Update(window, deltaTimeTimer);

// ------- EVENT -------

// ------- LOGIC -------
if (currentAppState == AppState::MainMenu) {
    if (currentFadeState == FadeState::None || currentFadeState == FadeState::FadingInMenu) {
        auto res = mainMenu.update(window, false);
        
        if (res != MainMenu::MenuResult::Nothing && currentFadeState == FadeState::None) {
            audioManager.playInteractSound();
        }
        
        if (res == MainMenu::MenuResult::Play && currentFadeState == FadeState::None) {
            currentFadeState = FadeState::FadingOutMenuToPlay;
        } else if (res == MainMenu::MenuResult::Quit && currentFadeState == FadeState::None) {
            window.close();
        } else if (res == MainMenu::MenuResult::Info) {
            // Placeholder for info
        }
    }
} else if (currentAppState == AppState::Paused) {
    auto res = mainMenu.update(window, true);
    
    if (res != MainMenu::MenuResult::Nothing) {
        audioManager.playInteractSound();
    }
    
        if (res == MainMenu::MenuResult::Continue) {
        currentAppState = AppState::Playing;
        audioManager.resumeAll();
    } else if (res == MainMenu::MenuResult::Quit) {
        window.close();
    }
} else if (currentAppState == AppState::Tutorial) {
    tutAnimTimer += deltaTime;
    if (tutAnimTimer > 150.0f) { // Swap frame every 150ms
        tutAnimTimer = 0.0f;
        tutWalkFrame = (tutWalkFrame + 1) % 4;
        tutPlayerMove.setTexture(tutWalkTex[tutWalkFrame], true); // reset rect if needed
    }
} else if (currentAppState == AppState::PreGameIntro) {
    std::string targetText;
    if (preGamePhase == 0) {
        targetText = "name? ";
    } else if (preGamePhase == 2) {
        targetText = std::string(nameBuffer) + " wakes up confused...";
    } else if (preGamePhase == 3) {
        targetText = "surrounded by 5 mysterious doors.";
    } else if (preGamePhase == 4) {
        targetText = "with one locked.";
    }

    if (preGamePhase == 0 || preGamePhase == 2 || preGamePhase == 3 || preGamePhase == 4) {
        preGameCharTimer += deltaTime;
        if (preGameCharTimer >= 80.0f) {
            preGameCharTimer = 0.0f;
            if (preGameCharIndex < targetText.size()) {
                currentLineText += targetText[preGameCharIndex];
                preGameCharIndex++;
                audioManager.typingSound.play();
            } else {
                preGameCharTimer = -1000.0f; // 1s pause
                preGameCharIndex = targetText.size() + 1; // marked done
            }
        } else if (preGameCharIndex > targetText.size() && preGameCharTimer >= 0.0f) {
            // Next phase
            preGameLines.push_back(currentLineText);
            currentLineText = "";
            preGameCharIndex = 0;
            if (preGamePhase == 4) {
                currentFadeState = FadeState::FadingInGame;
                currentAppState = AppState::Playing;
            } else {
                preGamePhase++;
                if (preGamePhase == 1) {
                    // preGamePhase 1 is handled in draw logic via ImGui
                }
            }
        }
    }
} else if (currentAppState == AppState::WinSequence) {
    if (winPhase == 0) {
        // Phase 0: Initialize fade and drop volume immediately
        currentFadeState = FadeState::FadingWhite;
        currentFadeColor = sf::Color(255, 255, 255, 0); // Fade to white
        fadeAlpha = 0.0f;
        winPhase = 1;
        winTimer = 0.0f;
        audioManager.winSound.play();
    } else if (winPhase == 1) {
        // Phase 1: Fading to white (about 2 seconds) and lowering volume
        fadeAlpha += (255.f / 2000.f) * deltaTime; // 255 alpha over 2000ms
        gameSongVolume -= (10.0f / 2000.f) * deltaTime; // 10 volume over 2000ms
        
        if (gameSongVolume <= 0.0f) gameSongVolume = 0.0f;
        audioManager.bgMusic.setVolume(gameSongVolume);
        
        if (fadeAlpha >= 255.0f) {
            fadeAlpha = 255.0f;
            currentFadeState = FadeState::None;
            winPhase = 2;
            winTimer = 0.0f;
        }
    } else if (winPhase == 2) {
        // Phase 2: Wait 2 seconds after fading to white
        winTimer += deltaTime;
        if (winTimer >= 2000.0f) {
            winPhase = 3;
            winTimer = 0.0f;
            winCharIndex = 0;
            currentWinLineText = "";
            audioManager.winLongSound.play(); // Already set to 20%
        }
    } else if (winPhase == 3) {
        // Phase 3: Slowly print "congratulations, you win" while playing winlong.mp3
        winTimer += deltaTime;
        std::string targetText = "congratulations, you win";
        if (winCharIndex < targetText.size()) {
            if (winTimer >= 100.0f) { // typing speed
                winTimer = 0.0f;
                currentWinLineText += targetText[winCharIndex];
                winCharIndex++;
            }
        } else {
            // Once typing is finished, wait 7 seconds
            if (winTimer >= 7000.0f) {
                winPhase = 4;
                winTimer = 0.0f;
            }
        }
    } else if (winPhase == 4) {
        // Phase 4: Fade out the song and the white screen to dark over 2 seconds
        winTimer += deltaTime;
        float fadeOutDuration = 2000.0f; // 2 seconds fade out
        
        // Volume fade out from 20 to 0
        float currentVol = 20.f - (20.f * (winTimer / fadeOutDuration));
        if (currentVol < 0.f) currentVol = 0.f;
        audioManager.winLongSound.setVolume(currentVol);
        
        // Color transition from white (255) to black (0)
        int colorVal = 255 - static_cast<int>(255.f * (winTimer / fadeOutDuration));
        if (colorVal < 0) colorVal = 0;
        
        currentFadeColor = sf::Color(colorVal, colorVal, colorVal, 255);
        
        if (winTimer >= fadeOutDuration) {
            winPhase = 5;
            winTimer = 0.0f;
        }
    } else if (winPhase == 5) {
        // Phase 5: Transition to main menu
        currentAppState = AppState::MainMenu;
        currentFadeState = FadeState::FadingInMenu;
        currentFadeColor = sf::Color(0, 0, 0, 255); // Back to black
        fadeAlpha = 255.0f;
        
        // Reset for next play
        gameState.Escaped = false;
        gameState.reset(); 
        playerData.hasKey = false;
        playerData.name = "";
        preGamePhase = 0;
        preGameLines.clear();
        currentLineText = "";
        audioManager.winLongSound.stop();
        audioManager.winLongSound.setVolume(20.f); // Reset volume for next time
        
        gameSongVolume = 10.0f;
        audioManager.bgMusic.setVolume(gameSongVolume); 
        audioManager.bgMusic.play();
    }
} else {
    if (currentFadeState == FadeState::None || currentFadeState == FadeState::FadingInGame) {
        player.Update(deltaTime, audioManager);
        rooms.update(deltaTime, player, gameState, playerData, audioManager);
        if (gameState.Escaped) {
            currentAppState = AppState::WinSequence;
            winPhase = 0;
            winTimer = 0.0f;
        }
    }
}

// ------- LOGIC -------

//----------------------- UPDATE ------------------------------

//----------------------- DRAW ------------------------------
window.clear(sf::Color::Black);

if (currentAppState == AppState::Intro) {
    mainMenu.drawIntro(window);
} else if (currentAppState == AppState::MainMenu) {
    mainMenu.draw(window, false);
} else if (currentAppState == AppState::Tutorial) {
    window.clear(sf::Color(10, 10, 10, 255)); // slight dark gray
    window.draw(wasdSprite);
    window.draw(tutPlayerMove);
    window.draw(tutMovementText);

    window.draw(tutDoorSprite);
    window.draw(tutPlayerInteract);
    window.draw(eSprite);
    window.draw(tutInteractText);

    window.draw(tutContinueText);

} else if (currentAppState == AppState::PreGameIntro) {
    window.clear(sf::Color::Black);
    
    sf::Font pixelyFont;
    if (pixelyFont.openFromFile("Assets/Fonts/Monaco.ttf")) {
        sf::Text preGameText(pixelyFont, "", 40);
        preGameText.setFillColor(sf::Color::White);
        sf::Vector2f viewCenter = window.getDefaultView().getCenter();
        float startY = viewCenter.y - 100.f;
        
        for (const auto& line : preGameLines) {
            preGameText.setString(line);
            sf::FloatRect bounds = preGameText.getLocalBounds();
            preGameText.setOrigin({bounds.position.x + bounds.size.x/2.0f, bounds.position.y + bounds.size.y/2.0f});
            preGameText.setPosition({viewCenter.x, startY});
            window.draw(preGameText);
            startY += 60.0f;
        }
        
        // draw current line
        preGameText.setString(currentLineText);
        sf::FloatRect bounds = preGameText.getLocalBounds();
        preGameText.setOrigin({bounds.position.x + bounds.size.x/2.0f, bounds.position.y + bounds.size.y/2.0f});
        preGameText.setPosition({viewCenter.x, startY});
        window.draw(preGameText);

        if (preGamePhase == 1) {
            ImGui::SetNextWindowPos(ImVec2(viewCenter.x - 150, startY + 50), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);
            ImGui::Begin("Name Input", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);
            ImGui::PushItemWidth(250);
            
            if (ImGui::IsWindowAppearing()) {
                ImGui::SetKeyboardFocusHere();
            }
            // Temporarily ignore SetKeyboardFocusHere() to see if it allows typing. Some input loops get stuck if it's called every frame.
            bool entered = ImGui::InputText("##Name", nameBuffer, IM_ARRAYSIZE(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::PopItemWidth();
            
            if ((entered || ImGui::Button("Confirm", ImVec2(100, 30))) && std::strlen(nameBuffer) > 0) {
                playerData.name = nameBuffer;
                preGameLines.back() += std::string(" ") + std::string(nameBuffer); // Append name to "name?" line
                currentLineText = "";
                preGameCharIndex = 0;
                preGameCharTimer = 0.0f;
                preGamePhase = 2; // Move to next line
            }
            ImGui::End();
        }
    }
} else if (currentAppState == AppState::WinSequence) {
    rooms.draw(window, gameState);
    player.Draw(window);
    
    // Draw darkness if not in Enemy Room
    if (rooms.getActiveRoom() != RoomID::SnakeRoom) {   
        player.DrawDarkness(window);
    }
    
    rooms.drawOverlay(window, gameState);
} else {
    rooms.draw(window, gameState);
    player.Draw(window);

    // Draw darkness if not in Enemy Room
    if (rooms.getActiveRoom() != RoomID::SnakeRoom) {   
        player.DrawDarkness(window);
    }

    rooms.drawOverlay(window, gameState);

    if (currentAppState == AppState::Paused) {
        mainMenu.draw(window, true);
    }
}
ImGui::SFML::Render(window);


if (currentFadeState != FadeState::None || currentAppState == AppState::WinSequence) {
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());
    
    // Draw fade rect
    if (currentFadeState != FadeState::None || winPhase > 0) {
        window.draw(fadeRect);
    }
    
    // Draw win text on top of the fade white screen if applicable
    if (currentAppState == AppState::WinSequence && winPhase >= 3) {
        sf::Font pixelyFont;
        if (pixelyFont.openFromFile("Assets/Fonts/Monaco.ttf")) {
            sf::Text winText(pixelyFont, currentWinLineText, 60);
            winText.setFillColor(sf::Color::Black); // Black text on white background
            sf::FloatRect bounds = winText.getLocalBounds();
            winText.setOrigin({bounds.position.x + bounds.size.x/2.0f, bounds.position.y + bounds.size.y/2.0f});
            winText.setPosition(window.getDefaultView().getCenter());
            window.draw(winText);
        }
    }
    
    window.setView(oldView);
}

window.display();

//----------------------- DRAW ------------------------------

}
ImGui::SFML::Shutdown();

//----------------------- GAME LOOP ------------------------------





    return 0;
}