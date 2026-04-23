#include "Enemy.h"
#include <iostream>
#include <string>

void Enemy::Init()
{
    // Visual Debugging hitbox
    boundRect.setFillColor(sf::Color::Transparent);
    boundRect.setOutlineColor(sf::Color::Blue);
    boundRect.setOutlineThickness(1.0f);

    // Default scaling (2x larger than player)
    sprite.setScale({9.0f, 9.0f});
    boundRect.setScale({1.0f, 1.0f});
}

void Enemy::Load()
{
    // --- LOAD IDLE FRAMES ---
    idleFrames.resize(3);
    for (int i = 0; i < 3; i++) {
        std::string path = "Assets/Textures/enemy_idle" + std::to_string(i + 1) + ".png";
        if (!idleFrames[i].loadFromFile(path)) {
            std::cout << "ERROR: Could not find " << path << "\n";
        }
    }

    // --- LOAD RUNNING FRAMES ---
    runFrames.resize(3);
    for (int i = 0; i < 3; i++) {
        std::string path = "Assets/Textures/enemy_running" + std::to_string(i + 1) + ".png";
        if (!runFrames[i].loadFromFile(path)) {
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

    // Default starting position
    sprite.setPosition({800.f, 700.f});

    // 4. Setup Hitbox (2x larger: 100x320)
    float hitboxWidth = 100.0f;
    float hitboxHeight = 320.0f;
    boundRect.setSize({hitboxWidth, hitboxHeight});
    boundRect.setOrigin({hitboxWidth / 2.f, hitboxHeight / 2.f});
}

sf::FloatRect Enemy::getHitBox() const
{
    return boundRect.getGlobalBounds();
}

void Enemy::Update(float deltaTime)
{
    float speed = 0.5f; // Enemy moves slower than player
    float gravity = 0.005f;
    float maxFallSpeed = 0.5f;
    sf::Vector2f vEnemyMovement(0.f, 0.f);

    // Apply Gravity
    velocity.y += gravity * deltaTime;
    if (velocity.y > maxFallSpeed) velocity.y = maxFallSpeed;

    vEnemyMovement.x = velocity.x;
    vEnemyMovement.y = velocity.y;

    sprite.move(vEnemyMovement * speed * deltaTime);
    boundRect.setPosition(sprite.getPosition());

    // --- ANIMATION LOGIC ---

    // 1. Update animation timer
    animTimer += deltaTime;
    if (animTimer >= frameDuration) {
        animTimer = 0.f;
        currentFrame++;
    }

    // 2. Play the running cycle if moving
    if (isRunning && !runFrames.empty()) {
        if (currentFrame >= runFrames.size()) currentFrame = 0;
        sprite.setTexture(runFrames[currentFrame], true);
    }
    // 3. Play the idle cycle if standing still
    else if (!isRunning && !idleFrames.empty()) {
        if (currentFrame >= idleFrames.size()) currentFrame = 0;
        sprite.setTexture(idleFrames[currentFrame], true);
    }
}

void Enemy::Draw(sf::RenderWindow& window)
{
    if (isDead) return;
    window.draw(sprite);
    // Uncomment the next line to see the hitbox
    //window.draw(boundRect);
}

void Enemy::handleFloorCollision(const sf::FloatRect& floorBounds)
{
    if (boundRect.getGlobalBounds().findIntersection(floorBounds)) {
        sf::FloatRect enemyBounds = boundRect.getGlobalBounds();
        
        float floorTop = floorBounds.position.y;
        float enemyBottom = enemyBounds.position.y + enemyBounds.size.y;
        
        float overlapDistance = enemyBottom - floorTop;
        sprite.setPosition({sprite.getPosition().x, sprite.getPosition().y - overlapDistance});
        boundRect.setPosition(sprite.getPosition());
        setVelocityY(0.f);
    }
}

void Enemy::handleLeftWallCollision(const sf::FloatRect& wallBounds)
{
    if (boundRect.getGlobalBounds().findIntersection(wallBounds)) {
        sf::FloatRect enemyBounds = boundRect.getGlobalBounds();
        sprite.setPosition({wallBounds.position.x + wallBounds.size.x + enemyBounds.size.x / 2.f, sprite.getPosition().y});
        boundRect.setPosition(sprite.getPosition());
    }
}

void Enemy::handleRightWallCollision(const sf::FloatRect& wallBounds)
{
    if (boundRect.getGlobalBounds().findIntersection(wallBounds)) {
        sf::FloatRect enemyBounds = boundRect.getGlobalBounds();
        sprite.setPosition({wallBounds.position.x - enemyBounds.size.x / 2.f, sprite.getPosition().y});
        boundRect.setPosition(sprite.getPosition());
    }
}

void Enemy::handleTopWallCollision(const sf::FloatRect& wallBounds)
{
    if (boundRect.getGlobalBounds().findIntersection(wallBounds)) {
        sf::FloatRect enemyBounds = boundRect.getGlobalBounds();
        sprite.setPosition({sprite.getPosition().x, wallBounds.position.y + wallBounds.size.y + enemyBounds.size.y / 2.f});
        boundRect.setPosition(sprite.getPosition());
    }
}
