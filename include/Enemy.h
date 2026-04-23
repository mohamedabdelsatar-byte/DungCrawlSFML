#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

class Enemy
{
    private:
    sf::Texture texture;
    std::vector<sf::Texture> runFrames;
    std::vector<sf::Texture> idleFrames;
    
    public:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    sf::RectangleShape boundRect;
    sf::FloatRect getHitBox() const;
    sf::Vector2i size;
    Enemy() : sprite(texture), velocity(0.f, 0.f) {}

    bool isRunning = false;
    bool isDead = false;
    int currentFrame = 0;
    float animTimer = 0.f;
    float frameDuration = 124.f; // Duration of each frame in milliseconds

    public:
    void Init();
    void Load();
    void Draw(sf::RenderWindow& window);
    void Update(float deltaTime);
    void setPosition(sf::Vector2f pos) { sprite.setPosition(pos); boundRect.setPosition(pos); }
    void setVelocity(sf::Vector2f v) { velocity = v; }
    void setVelocityY(float y) { velocity.y = y; }
    sf::Vector2f getVelocity() const { return velocity; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    
    // Collision detection methods
    void handleFloorCollision(const sf::FloatRect& floorBounds);
    void handleLeftWallCollision(const sf::FloatRect& wallBounds);
    void handleRightWallCollision(const sf::FloatRect& wallBounds);
    void handleTopWallCollision(const sf::FloatRect& wallBounds);
};
