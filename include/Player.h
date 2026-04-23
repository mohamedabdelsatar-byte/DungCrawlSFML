
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class AudioManager;

class Player
{
    private:
    sf::Texture texture;
    sf::Vector2f velocity;
    std::vector<sf::Texture> animFrames;
    std::vector<sf::Texture> idleFrames; 
    

    public:
    sf::Sprite sprite;
    sf::RectangleShape boundRect;
    
    // Darkness effect
    sf::RenderTexture darknessRenderTexture;
    sf::Texture lightTexture;
    sf::Sprite lightSprite;
    sf::Sprite darknessFinalSprite;

    sf::FloatRect getHitBox() const;
    sf::Vector2i size;
    Player() : sprite(texture), lightSprite(lightTexture), darknessFinalSprite(darknessRenderTexture.getTexture()), velocity(0.f, 0.f) {}
    
    bool wasMoving = false;
    bool isControlDisabled = false; // Disable controls during sequences
    int currentFrame = 0;
    float animTimer = 0.f;
    float frameDuration = 124.f; // Duration of each frame in milliseconds
    
    public:
    void Init();
    void InitDarkness();
    void Load();
    void Draw(sf::RenderWindow& window);
    void DrawDarkness(sf::RenderWindow& window);
    void Update(float deltaTime, AudioManager& audioManager);
    void setVelocityY(float y) { velocity.y = y; }
    void setVelocity(sf::Vector2f v) { velocity = v; }
    sf::Vector2f getVelocity() const { return velocity; }

};