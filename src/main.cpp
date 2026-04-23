#include <SFML/Graphics.hpp>
#include <Player.h>
#include <Enemy.h>
#include <Rooms.h>
#include "AudioManager.h"
#include "MainMenu.h"

bool Escaped = false;

enum class AppState { Intro, MainMenu, Playing, Paused };
enum class FadeState { IntroFadingIn, IntroHold, IntroFadingOut, 
    IntroPause, None, FadingInMenu, FadingOutMenuToPlay, FadingInGame };

int main(){
//----------------------- INITIALIZE ------------------------------
sf::RenderWindow window(sf::VideoMode({1920,1080}), "Dungeon Crawler");
window.setFramerateLimit(120);

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

sf::RectangleShape fadeRect(window.getDefaultView().getSize());
fadeRect.setFillColor(sf::Color(0, 0, 0, 255));

//----------------------- INITIALIZE ------------------------------

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
            currentFadeState = FadeState::FadingInGame;
            currentAppState = AppState::Playing;
        }
    } else if (currentFadeState == FadeState::FadingInGame) {
        fadeAlpha -= fadeSpeed * deltaTime;
        if (fadeAlpha <= 0.0f) { fadeAlpha = 0.0f; currentFadeState = FadeState::None; }
    }
    fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(fadeAlpha)));

    while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
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
        }

// ------- EVENT -------

// ------- LOGIC -------
if (currentAppState == AppState::MainMenu) {
    if (currentFadeState == FadeState::None || currentFadeState == FadeState::FadingInMenu) {
        auto res = mainMenu.update(window, false);
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
    if (res == MainMenu::MenuResult::Continue) {
        currentAppState = AppState::Playing;
        audioManager.resumeAll();
    } else if (res == MainMenu::MenuResult::Quit) {
        window.close();
    }
} else {
    if (currentFadeState == FadeState::None || currentFadeState == FadeState::FadingInGame) {
        player.Update(deltaTime, audioManager);
        rooms.update(deltaTime, player, gameState, playerData, audioManager);
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

if (currentFadeState != FadeState::None) {
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());
    window.draw(fadeRect);
    window.setView(oldView);
}

window.display();

//----------------------- DRAW ------------------------------

}

//----------------------- GAME LOOP ------------------------------





    return 0;
}