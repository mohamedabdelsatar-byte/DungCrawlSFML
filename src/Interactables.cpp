#include "Interactables.h"
#include "GameData.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <cmath>




void Interactables::Init()
{   
    // Visual Debugging hitbox
    boundRect.setFillColor(sf::Color::Transparent);
    boundRect.setOutlineColor(sf::Color::Green);
    boundRect.setOutlineThickness(1.0f);
    
    // Default scaling
    Sprite.setScale({6.0f, 6.0f});
    boundRect.setScale({3.0f, 3.0f});
    
    // Initialize position
    position = {960.f, 860.f};
}

void Interactables::Load(std::string file_path)
{
    // --- LOAD TEXTURE ---
    /* std::string texturePath = "Assets/Textures/table.png"; */
    /* std::string chestOpenTexturePath = "Assets/Textures/chest_open.png";
    std::string chestClosedTexturePath = "Assets/Textures/chest_closed.png"; */
    
    
    if (!Texture.loadFromFile(file_path)) {
        std::cout << "ERROR: Could not find " << file_path << "\n";
    } else {
        Sprite.setTexture(Texture, true);
        size = Texture.getSize();
        std::cout << "Successfully loaded: " << file_path << "\n";
    }
    
    // --- SET SPRITE PROPERTIES ---
    if (size.x > 0 && size.y > 0) {
        Sprite.setOrigin({size.x / 2.f, size.y / 2.f}); // Center the sprite
    }
    Sprite.setPosition(position);
    
    
    // --- SETUP HITBOX ---
    if (size.x > 0 && size.y > 0) {
        boundRect.setSize({static_cast<float>(size.x * Sprite.getScale().x), static_cast<float>(size.y * Sprite.getScale().y)});
        boundRect.setOrigin({boundRect.getSize().x / 2.f, boundRect.getSize().y / 2.f});
        boundRect.setPosition(position);
    }
}

void Interactables::LoadAlternate(std::string file_path)
{
    if (!TextureAlt.loadFromFile(file_path)) {
        std::cout << "ERROR: Could not find alternate texture " << file_path << "\n";
    } else {
        std::cout << "Successfully loaded alternate texture: " << file_path << "\n";
    }
}

void Interactables::SwitchToAlternate()
{
    if (TextureAlt.getSize().x > 0 && TextureAlt.getSize().y > 0) {
        Sprite.setTexture(TextureAlt, true);
        size = TextureAlt.getSize();
        
        if (size.x > 0 && size.y > 0) {
            Sprite.setOrigin({size.x / 2.f, size.y / 2.f});
            boundRect.setSize({static_cast<float>(size.x), static_cast<float>(size.y)});
            boundRect.setOrigin({size.x / 2.f, size.y / 2.f});
        }
    }
}

void Interactables::ResetToDefault()
{
    if (Texture.getSize().x > 0 && Texture.getSize().y > 0) {
        Sprite.setTexture(Texture, true);
        size = Texture.getSize();
        
        if (size.x > 0 && size.y > 0) {
            Sprite.setOrigin({size.x / 2.f, size.y / 2.f});
            boundRect.setSize({static_cast<float>(size.x), static_cast<float>(size.y)});
            boundRect.setOrigin({size.x / 2.f, size.y / 2.f});
        }
    }
}

void Interactables::Draw(sf::RenderWindow& window)
{
    if (!pickedUp) {
        window.draw(Sprite);
    }
}

void Interactables::Update(float deltaTime)
{
    // Update position if needed
    Sprite.setPosition(position);
    // boundRect.setPosition(position);
    
    // Animation logic
    if (isAnimated && numFrames > 1 && !pickedUp) {
        animTimer += deltaTime;
        if (animTimer >= frameDuration) {
            animTimer = 0.f;
            currentFrame++;
            if (currentFrame >= numFrames) currentFrame = 0;
            
            // Vertical sprite sheet: move the Y coordinate
            Sprite.setTextureRect(sf::IntRect({0, currentFrame * frameSize.y}, {frameSize.x, frameSize.y}));
        }
    }
    
    // Add animation or state changes here
    // Example: bobbing animation, color changes on interaction, etc.
}

void Interactables::MakeAnimated(int frames, float duration, int frameWidth, int frameHeight)
{
    isAnimated = true;
    numFrames = frames;
    frameDuration = duration;
    frameSize = {frameWidth, frameHeight};
    currentFrame = 0;
    animTimer = 0.f;

    Sprite.setTextureRect(sf::IntRect({0, 0}, {frameSize.x, frameSize.y}));
    Sprite.setOrigin({frameSize.x / 2.f, frameSize.y / 2.f});
    boundRect.setSize({static_cast<float>(frameSize.x), static_cast<float>(frameSize.y)});
    boundRect.setOrigin({frameSize.x / 2.f, frameSize.y / 2.f});
}

void Interactables::setPosition(sf::Vector2f pos)
{
    position = pos;
    Sprite.setPosition(position);
    boundRect.setPosition(position);
}

sf::FloatRect Interactables::getHitBox() const
{
    return Sprite.getGlobalBounds();
}

bool Interactables::IsPlayerNearby(sf::Vector2f playerPos, float range)
{
    float distance = std::sqrt(
        (playerPos.x - position.x) * (playerPos.x - position.x) +
        (playerPos.y - position.y) * (playerPos.y - position.y)
    );
    return distance <= range;
}

void Interactables::OnInteract()
{
    isInteracting = !isInteracting;
    
    if (isInteracting) {
        // Change appearance when interacting
        Sprite.setColor(sf::Color::Yellow);
        std::cout << "Table interaction started!\n";
    } else {
        // Reset appearance
        Sprite.setColor(sf::Color::White);
        std::cout << "Table interaction ended!\n";
    }
    
   
}
