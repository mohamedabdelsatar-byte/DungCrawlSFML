#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class MainMenu {
public:
    enum class MenuResult { Nothing, Play, Info, Quit, Continue };

    MainMenu();
    void init(sf::RenderWindow& window);
    MenuResult update(sf::RenderWindow& window, bool isPaused);
    void draw(sf::RenderWindow& window, bool isPaused);
    void drawIntro(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::Font monospaceFont;
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> backgroundSprite;

    std::optional<sf::Text> playButton;
    std::optional<sf::Text> infoButton;
    std::optional<sf::Text> quitButton;
    std::optional<sf::Text> continueButton;

    // Info screen elements
    std::optional<sf::Text> infoScreenText;
    std::optional<sf::Text> backButton;
    std::optional<sf::RectangleShape> infoDimOverlay;
    bool showInfoScreen = false;
    
    // Intro screen elements
    sf::Texture ecuTexture;
    std::optional<sf::Sprite> ecuSprite;
    std::optional<sf::RectangleShape> introBg;

    std::optional<sf::RectangleShape> pauseDimOverlay;

    void updateButtonHover(sf::RenderWindow& window, std::optional<sf::Text>& button);
    bool isButtonClicked(sf::RenderWindow& window, const std::optional<sf::Text>& button);
    bool wasMousePressed;
};
