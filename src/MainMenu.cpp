#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu() : wasMousePressed(false) {}

void MainMenu::init(sf::RenderWindow& window) {
    if (!font.openFromFile("Assets/Fonts/ARCADECLASSIC.TTF")) {
        // Handle error
    }
    if (!monospaceFont.openFromFile("Assets/Fonts/Monaco.ttf")) {
        // Handle error
    }

    if (!backgroundTexture.loadFromFile("Assets/Textures/mainmenu.png")) {
        // Handle error
    }
    backgroundSprite.emplace(backgroundTexture);
    
    // Center the background properly
    sf::Vector2f viewSize = window.getDefaultView().getSize();
    sf::Vector2f viewCenter = window.getDefaultView().getCenter();
    
    sf::Vector2u texSize = backgroundTexture.getSize();
    backgroundSprite->setOrigin({texSize.x / 2.0f, texSize.y / 2.0f});
    backgroundSprite->setPosition(viewCenter);
    // Scale background to fit the window exactly
    backgroundSprite->setScale({viewSize.x / texSize.x, viewSize.y / texSize.y});

    // Setup PLAY button
    playButton.emplace(font, "PLAY", 70);
    playButton->setFillColor(sf::Color(128, 128, 128));
    sf::FloatRect playRect = playButton->getLocalBounds();
    playButton->setOrigin(playRect.position + playRect.size / 2.0f);
    playButton->setPosition({viewCenter.x, viewCenter.y});

    // Setup INFO button
    infoButton.emplace(font, "INFO", 70);
    infoButton->setFillColor(sf::Color(128, 128, 128));
    sf::FloatRect infoRect = infoButton->getLocalBounds();
    infoButton->setOrigin(infoRect.position + infoRect.size / 2.0f);
    infoButton->setPosition({viewCenter.x, viewCenter.y + 100.0f});

    // Setup QUIT button (for main menu)
    quitButton.emplace(font, "QUIT", 70);
    quitButton->setFillColor(sf::Color(128, 128, 128));
    sf::FloatRect quitRect = quitButton->getLocalBounds();
    quitButton->setOrigin(quitRect.position + quitRect.size / 2.0f);
    quitButton->setPosition({viewCenter.x, viewCenter.y + 200.0f});

    // Setup CONTINUE button (for pause menu)
    continueButton.emplace(font, "CONTINUE", 80);
    continueButton->setFillColor(sf::Color(128, 128, 128));
    sf::FloatRect continueRect = continueButton->getLocalBounds();
    continueButton->setOrigin(continueRect.position + continueRect.size / 2.0f);
    continueButton->setPosition({viewCenter.x, viewCenter.y - 80.0f});

    // Dim overlay for pause
    pauseDimOverlay.emplace(viewSize);
    pauseDimOverlay->setOrigin({viewSize.x / 2.0f, viewSize.y / 2.0f});
    pauseDimOverlay->setPosition(viewCenter);
    pauseDimOverlay->setFillColor(sf::Color(255, 255, 255, 100)); // soft dim white
    
    // Dim overlay for Info Screen
    infoDimOverlay.emplace(viewSize);
    infoDimOverlay->setOrigin({viewSize.x / 2.0f, viewSize.y / 2.0f});
    infoDimOverlay->setPosition(viewCenter);
    infoDimOverlay->setFillColor(sf::Color(0, 0, 0, 230)); // strong dim black

    // Setup EXIT button (for info screen)
    backButton.emplace(font, "EXIT", 50);
    backButton->setFillColor(sf::Color(128, 128, 128));
    sf::FloatRect backRect = backButton->getLocalBounds();
    backButton->setOrigin(backRect.position + backRect.size / 2.0f);
    backButton->setPosition({viewCenter.x, viewCenter.y + 400.0f});

    // Setup ECU Intro Screen elements
    introBg.emplace(sf::Vector2f(viewSize.x, viewSize.y));
    introBg->setFillColor(sf::Color::White);

    if (ecuTexture.loadFromFile("Assets/Textures/ecu.jpg")) {
        ecuTexture.setSmooth(true);
        ecuSprite.emplace(ecuTexture);

        if (ecuTexture.getSize().x > 0) {
            sf::Vector2u texSize = ecuTexture.getSize();
            float scale = std::min(viewSize.x / texSize.x, viewSize.y / texSize.y) * 0.70f;
            
            ecuSprite->setScale({scale, scale});
            
            sf::FloatRect bounds = ecuSprite->getLocalBounds();
            ecuSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
            
            // Positioned manually to the left offset by their request
            ecuSprite->setPosition({(viewSize.x / 2.0f) - 30.0f, viewSize.y / 2.0f});
        }
    }

    // Setup Info Text
    std::string infoString = R"(________________________________________________________________________________
|||__________________________________________________________________________|||
||| [][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]    |||
||| []|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[]     |||
||| ________________________________________________________________________ |||
|||                                                                          |||
|||                           DUNG CRAWLER                                   |||
|||                                                                          |||
|||    A Dungeon Based game where the player spawns in the middle           |||
|||    of a room, surrounded by 5 doors. One of which has the exit.          |||
|||                                                                          |||
|||    The player is met with challenges, puzzles. All            |||
|||    to obtain the single Key to escape and win the game.                  |||
|||                                                                          |||
||| ________________________________________________________________________ |||
|||                                                                          |||
|||                         _Project Made By_                                |||
|||                                                                          |||
|||   Mohamed Abdelsatar Fathy              192500268                        |||
|||   Mohamed Mahmoud Hamdy                 192500036                        |||
|||   Kahlid Waleed                         192500713                        |||
|||   Abdulrahman Sami Hamza                192500851                        |||
|||   Ahmed Hossam El-Din                   192500811                        |||
|||   Yassin Mohamed El-Sayed               192400580                        |||
|||                                                                          |||
||| ________________________________________________________________________ |||
||| []|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[]     |||
||| [][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]|[][][]    |||
|||__________________________________________________________________________|||
________________________________________________________________________________)";
    infoScreenText.emplace(monospaceFont, infoString, 20);
    infoScreenText->setFillColor(sf::Color::White);
    sf::FloatRect textBounds = infoScreenText->getLocalBounds();
    infoScreenText->setOrigin(textBounds.position + textBounds.size / 2.0f);
    infoScreenText->setPosition(viewCenter);
}

void MainMenu::updateButtonHover(sf::RenderWindow& window, std::optional<sf::Text>& button) {
    if (!button) return;
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosView = window.mapPixelToCoords(mousePosWindow, window.getDefaultView());
    
    // SFML 3 gets rect slightly differently
    sf::FloatRect bounds = button->getGlobalBounds();
    
    if (bounds.contains(mousePosView)) {
        button->setFillColor(sf::Color::White);
    } else {
        button->setFillColor(sf::Color(128, 128, 128));
    }
}

bool MainMenu::isButtonClicked(sf::RenderWindow& window, const std::optional<sf::Text>& button) {
    if (!button) return false;
    sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosView = window.mapPixelToCoords(mousePosWindow, window.getDefaultView());
    
    if (button->getGlobalBounds().contains(mousePosView)) {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && wasMousePressed) {
            return true;
        }
    }
    
    return false;
}

MainMenu::MenuResult MainMenu::update(sf::RenderWindow& window, bool isPaused) {
    MenuResult result = MenuResult::Nothing;

    if (showInfoScreen) {
        updateButtonHover(window, backButton);
        if (isButtonClicked(window, backButton)) {
            showInfoScreen = false;
        }
    } else if (!isPaused) {
        updateButtonHover(window, playButton);
        updateButtonHover(window, infoButton);
        updateButtonHover(window, quitButton);

        if (isButtonClicked(window, playButton)) result = MenuResult::Play;
        else if (isButtonClicked(window, infoButton)) showInfoScreen = true; // Handled internally
        else if (isButtonClicked(window, quitButton)) result = MenuResult::Quit;
    } else {
        updateButtonHover(window, continueButton);
        sf::Vector2f viewCenter = window.getDefaultView().getCenter();
        sf::FloatRect quitRect = quitButton->getLocalBounds();
        quitButton->setOrigin(quitRect.position + quitRect.size / 2.0f);
        quitButton->setPosition({viewCenter.x, viewCenter.y + 80.0f});
        updateButtonHover(window, quitButton);

        if (isButtonClicked(window, continueButton)) result = MenuResult::Continue;
        else if (isButtonClicked(window, quitButton)) result = MenuResult::Quit;
    }

    // Update global mouse state at the end of update
    wasMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    return result;
}

void MainMenu::draw(sf::RenderWindow& window, bool isPaused) {
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());
    
    if (showInfoScreen) {
        if (backgroundSprite) window.draw(*backgroundSprite);
        if (infoDimOverlay) window.draw(*infoDimOverlay);
        if (infoScreenText) window.draw(*infoScreenText);
        if (backButton) window.draw(*backButton);
    } else if (!isPaused) {
        if (backgroundSprite) window.draw(*backgroundSprite);
        sf::Vector2f viewCenter = window.getDefaultView().getCenter();
        
        if (quitButton) {
            sf::FloatRect quitRect = quitButton->getLocalBounds();
            quitButton->setOrigin(quitRect.position + quitRect.size / 2.0f);
            quitButton->setPosition({viewCenter.x, viewCenter.y + 200.0f});
        }
        
        if (playButton) window.draw(*playButton);
        if (infoButton) window.draw(*infoButton);
        if (quitButton) window.draw(*quitButton);
    } else {
        if (pauseDimOverlay) window.draw(*pauseDimOverlay);
        sf::Vector2f viewCenter = window.getDefaultView().getCenter();
        
        if (quitButton) {
            sf::FloatRect quitRect = quitButton->getLocalBounds();
            quitButton->setOrigin(quitRect.position + quitRect.size / 2.0f);
            quitButton->setPosition({viewCenter.x, viewCenter.y + 80.0f});
        }
        
        if (continueButton) window.draw(*continueButton);
        if (quitButton) window.draw(*quitButton);
    }
    
    window.setView(oldView);
}

void MainMenu::drawIntro(sf::RenderWindow& window) {
    if (introBg) {
        window.draw(*introBg);
    }
    if (ecuSprite) {
        window.draw(*ecuSprite);
    }
}
