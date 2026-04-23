#include "Player.h"
#include "GameData.h"
#include "AudioManager.h"
#include <iostream>
#include <string>

#include <cmath>

void Player::InitDarkness()
{
    if (!darknessRenderTexture.resize({1920, 1080})) {
        std::cout << "Error initializing darkness render texture\n";
    }

    const unsigned int lightRadius = 400; // Radius of the light circle
    sf::Image lightImage;
    lightImage.resize({static_cast<unsigned>(lightRadius * 2), static_cast<unsigned>(lightRadius * 2)}, sf::Color::Transparent);

    for (unsigned int y = 0; y < lightRadius * 2; ++y) {
        for (unsigned int x = 0; x < lightRadius * 2; ++x) {
            float dx = static_cast<float>(x) - static_cast<float>(lightRadius);
            float dy = static_cast<float>(y) - static_cast<float>(lightRadius);
            float distance = std::sqrt(dx * dx + dy * dy);
            
            if (distance < lightRadius) {
                float ratio = distance / static_cast<float>(lightRadius);
                ratio = std::pow(ratio, 1.5f); // soften
                std::uint8_t alpha = static_cast<std::uint8_t>(240.f * ratio);
                lightImage.setPixel({x, y}, sf::Color(0, 0, 0, alpha));
            } else {
                lightImage.setPixel({x, y}, sf::Color(0, 0, 0, 240));
            }
        }
    }

    (void)lightTexture.loadFromImage(lightImage);
    lightSprite.setTexture(lightTexture, true);
    lightSprite.setOrigin({static_cast<float>(lightRadius), static_cast<float>(lightRadius)});

    darknessFinalSprite.setTexture(darknessRenderTexture.getTexture(), true);
}

void Player::Init()
{   
    // Visual Debugging hitbox
    boundRect.setFillColor(sf::Color::Transparent);
    boundRect.setOutlineColor(sf::Color::Red);
    boundRect.setOutlineThickness(1.0f);
    
    // Default scaling
    sprite.setScale({4.5f, 4.5f});
    boundRect.setScale({1.0f, 1.0f});
}

void Player::Load()
{
    // --- LOAD WALK FRAMES ---
    animFrames.resize(4);
    for (int i = 0; i < 4; i++) {
        std::string path = "Assets/Textures/player_walk" + std::to_string(i + 1) + ".png";
        if (!animFrames[i].loadFromFile(path)) {
            std::cout << "ERROR: Could not find " << path << "\n";
        }
    }

    // --- LOAD IDLE FRAMES ---
    int numIdleFrames = 4;
    idleFrames.resize(numIdleFrames); 
    for (int i = 0; i < numIdleFrames; i++) {
        std::string path = "Assets/Textures/player_idle" + std::to_string(i + 1) + ".png";
        if (!idleFrames[i].loadFromFile(path)) {
            std::cout << "ERROR: Could not find " << path << "\n";
        }
    }

    // --- SET INITIAL SPRITE (Using Idle Frame 1) ---
    if (!idleFrames.empty() && idleFrames[0].getSize().x > 0) {
        sprite.setTexture(idleFrames[0], true); 
        sf::Vector2u texSize = idleFrames[0].getSize();
        sprite.setOrigin({texSize.x / 2.f, texSize.y / 2.f});
        sprite.setColor(sf::Color::White); 
    }

    sprite.setPosition({960.f, 600.f});

    // 4. Setup Hitbox
    float hitboxWidth = 50.0f;
    float hitboxHeight = 160.0f;
    boundRect.setSize({hitboxWidth, hitboxHeight});
    boundRect.setOrigin({hitboxWidth / 2.f, hitboxHeight / 2.f});
}

sf::FloatRect Player::getHitBox() const{
    return boundRect.getGlobalBounds();
}

void Player::Update(float deltaTime, AudioManager& audioManager)
{
    float speed = 1.f;
    float gravity = 0.005f;
    float maxFallSpeed = 0.5f;
    sf::Vector2f vPlayerMovement(0.f, 0.f);

    bool isMoving = false; // Flag to check if we should animate walking

    // Movement Controls
    if (!isControlDisabled) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            vPlayerMovement.x -= 1.0f;
            isMoving = true;
            sprite.setScale({-4.5f, 4.5f}); // Flip sprite to face left
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            vPlayerMovement.x += 1.0f;
            isMoving = true;
            sprite.setScale({4.5f, 4.5f}); // Flip sprite to face right
        }
    }

    // Apply Gravity and Movement
    velocity.y += gravity * deltaTime;
    if (velocity.y > maxFallSpeed) velocity.y = maxFallSpeed;
    
    vPlayerMovement.x += velocity.x;
    vPlayerMovement.y = velocity.y;
    
    sprite.move(vPlayerMovement * speed * deltaTime);
    boundRect.setPosition(sprite.getPosition());


    // --- ANIMATION LOGIC ---
    
    // 1. Did we just stop or start moving? If so, reset the animation.
    if (isMoving != wasMoving) {
        currentFrame = 0;
        animTimer = 0.f;
        audioManager.toggleWalkingSound(isMoving);
    }

    // 2. Play the walk cycle if moving
    if (isMoving && !animFrames.empty()) {
        animTimer += deltaTime;
        if (animTimer >= frameDuration) {
            animTimer = 0.f;
            currentFrame++;
            if (currentFrame >= animFrames.size()) currentFrame = 0;
            
            sprite.setTexture(animFrames[currentFrame], true);
        }
    } 
    // 3. Play the idle cycle if standing still
    else if (!isMoving && !idleFrames.empty()) {
        animTimer += deltaTime;
        if (animTimer >= frameDuration) {
            animTimer = 0.f;
            currentFrame++;
            if (currentFrame >= idleFrames.size()) currentFrame = 0; 
            
            sprite.setTexture(idleFrames[currentFrame], true);
        }
    }

    // 4. Save state for next frame
    wasMoving = isMoving; 
}

void Player::Draw(sf::RenderWindow& window){
    window.draw(sprite);
    // You can comment out the next line if you want to hide the red box!
    //window.draw(boundRect); 
}

void Player::DrawDarkness(sf::RenderWindow& window) {
    // Clear darkness texture with fully opaque dark color
    darknessRenderTexture.clear(sf::Color(0, 0, 0, 240)); 
    
    // Position the light source over the player
    sf::Vector2f playerPos = sprite.getPosition();
    lightSprite.setPosition(playerPos);
    
    // Draw the soft light hole using sf::BlendNone to overwrite alpha
    darknessRenderTexture.draw(lightSprite, sf::BlendNone);
    darknessRenderTexture.display();
    
    window.draw(darknessFinalSprite);
}