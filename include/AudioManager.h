#pragma once

#include <SFML/Audio.hpp>
#include <iostream>

class AudioManager {
public:
    AudioManager();

    void loadAll();

    // Background and Music
    sf::Music bgMusic;
    sf::Music chaseMusic;

    // Sounds
    sf::SoundBuffer deathBuffer;
    sf::Sound deathSound;

    sf::SoundBuffer doorSlamBuffer;
    sf::Sound doorSlamSound;

    sf::SoundBuffer doorOpenBuffer;
    sf::Sound doorOpenSound;

    sf::SoundBuffer doorCloseBuffer;
    sf::Sound doorCloseSound;

    sf::SoundBuffer interactBuffer;
    sf::Sound interactSound;

    sf::SoundBuffer chestOpenBuffer;
    sf::Sound chestOpenSound;

    sf::SoundBuffer obtainedItemBuffer;
    sf::Sound obtainedItemSound;

    sf::SoundBuffer deniedBuffer;
    sf::Sound deniedSound;

    sf::SoundBuffer walkingBuffer;
    sf::Sound walkingSound;

    void playDeathSound();
    void playDoorSlamSound();
    void playDoorOpenSound();
    void playDoorCloseSound();
    void playInteractSound();
    void playChestOpenSound();
    void playObtainedItemSound();
    void playDeniedSound();
    
    void toggleWalkingSound(bool isMoving);
    
    void playBgMusic();
    void stopBgMusic();
    
    void playChaseMusic();
    void stopChaseMusic();

    void pauseAll();
    void resumeAll();
};
