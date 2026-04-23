#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Interactables
{
    private:
    sf::Texture Texture;
    sf::Texture TextureAlt;  // For alternate state (e.g., open chest)
    /* sf::Texture keyTexture;
    sf::Texture chestClosedTexture;
    sf::Texture chestOpenTexture; */
    sf::Vector2u size;

    public:
    sf::Sprite Sprite;
    /* sf::Sprite keySprite;
    sf::Sprite chestClosedSprite;
    sf::Sprite chestOpenSprite; */

    sf::RectangleShape boundRect;
    
    Interactables() : Sprite(Texture)
    {

    }
    
    // Position and scale properties
    sf::Vector2f position;
    bool isInteracting = false;
    bool isChestOpen = false;
    bool pickedUp = false; // For items like keys

    // Animation properties
    bool isAnimated = false;
    int numFrames = 1;
    int currentFrame = 0;
    float animTimer = 0.f;
    float frameDuration = 100.f; // in milliseconds
    sf::Vector2i frameSize;
    
    public:
    // Main game loop functions
    void Init();
    void Load(std::string file_path);
    void LoadAlternate(std::string file_path);  // Load alternate texture (e.g., open state)
    void SwitchToAlternate();  // Switch to alternate texture
    void ResetToDefault();  // Reset to primary texture
    void Draw(sf::RenderWindow& window);
    void Update(float deltaTime);

    // Setup Animation
    void MakeAnimated(int frames, float duration, int frameWidth, int frameHeight);
    
    // Getter and setter functions
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getHitBox() const;
    
    // Interaction functions
    void OnInteract();
    bool IsPlayerNearby(sf::Vector2f playerPos, float range);
};
