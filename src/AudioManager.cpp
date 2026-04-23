#include "AudioManager.h"

AudioManager::AudioManager() :
    deathSound(deathBuffer),
    doorSlamSound(doorSlamBuffer),
    doorOpenSound(doorOpenBuffer),
    doorCloseSound(doorCloseBuffer),
    interactSound(interactBuffer),
    chestOpenSound(chestOpenBuffer),
    obtainedItemSound(obtainedItemBuffer),
    deniedSound(deniedBuffer),
    walkingSound(walkingBuffer)
{
}

void AudioManager::loadAll()
{
    // --- LOAD MUSIC ---
    if (!bgMusic.openFromFile("Assets/audio/background_audio.mp3")) {
        std::cout << "WARNING: Assets/audio/background_audio.mp3 missing.\n";
    } else {
        bgMusic.setLooping(true);
        bgMusic.setVolume(10.f); 
    }

    if (!chaseMusic.openFromFile("Assets/audio/chasing.mp3")) {
        std::cout << "WARNING: Assets/audio/chasing.mp3 missing.\n";
    } else {
        chaseMusic.setLooping(true);
        chaseMusic.setVolume(40.f);
    }

    // --- LOAD SOUND BUFFERS ---
    if (!deathBuffer.loadFromFile("Assets/audio/death_sound.mp3")) {
        std::cout << "WARNING: Assets/audio/death_sound.mp3 missing.\n";
    } else {
        deathSound.setVolume(100.f);
    }

    if (!doorSlamBuffer.loadFromFile("Assets/audio/door_slam.mp3")) {
        std::cout << "WARNING: Assets/audio/door_slam.mp3 missing.\n";
    } else {
        doorSlamSound.setVolume(70.f);
    }

    if (!doorOpenBuffer.loadFromFile("Assets/audio/doorOpening.mp3")) {
        std::cout << "WARNING: Assets/audio/doorOpening.mp3 missing.\n";
    } else {
        doorOpenSound.setVolume(100.f);
    }

    if (!doorCloseBuffer.loadFromFile("Assets/audio/doorClosing.mp3")) {
        std::cout << "WARNING: Assets/audio/doorClosing.mp3 missing.\n";
    } else {
        doorCloseSound.setVolume(100.f);
    }

    if (!interactBuffer.loadFromFile("Assets/audio/interact.mp3")) {
        std::cout << "WARNING: Assets/audio/interact.mp3 missing.\n";
    } else {
        interactSound.setVolume(100.f);
    }

    if (!chestOpenBuffer.loadFromFile("Assets/audio/chest_open.mp3")) {
        std::cout << "WARNING: Assets/audio/chest_open.mp3 missing.\n";
    } else {
        chestOpenSound.setVolume(100.f);
    }

    if (!obtainedItemBuffer.loadFromFile("Assets/audio/obtained_item.mp3")) {
        std::cout << "WARNING: Assets/audio/obtained_item.mp3 missing.\n";
    } else {
        obtainedItemSound.setVolume(70.f);
    }

    if (!deniedBuffer.loadFromFile("Assets/audio/denied.mp3")) {
        std::cout << "WARNING: Assets/audio/denied.mp3 missing.\n";
    } else {
        deniedSound.setVolume(100.f);
    }

    if (!walkingBuffer.loadFromFile("Assets/audio/walking.mp3")) {
        std::cout << "WARNING: Assets/audio/walking.mp3 missing.\n";
    } else {
        walkingSound.setVolume(70.f);
        walkingSound.setLooping(true);
    }
}

void AudioManager::playDeathSound() { deathSound.play(); }
void AudioManager::playDoorSlamSound() { doorSlamSound.play(); }
void AudioManager::playDoorOpenSound() { doorOpenSound.play(); }
void AudioManager::playDoorCloseSound() { doorCloseSound.play(); }
void AudioManager::playInteractSound() { interactSound.play(); }
void AudioManager::playChestOpenSound() { chestOpenSound.play(); }
void AudioManager::playObtainedItemSound() { obtainedItemSound.play(); }
void AudioManager::playDeniedSound() { deniedSound.play(); }

void AudioManager::toggleWalkingSound(bool isMoving) {
    if (isMoving && walkingSound.getStatus() != sf::SoundSource::Status::Playing) {
        walkingSound.play();
    } else if (!isMoving && walkingSound.getStatus() == sf::SoundSource::Status::Playing) {
        walkingSound.stop();
    }
}

void AudioManager::playBgMusic() {
    if (bgMusic.getStatus() != sf::SoundSource::Status::Playing) {
        bgMusic.play();
    }
}

void AudioManager::stopBgMusic() {
    bgMusic.stop();
}

void AudioManager::playChaseMusic() {
    if (chaseMusic.getStatus() != sf::SoundSource::Status::Playing) {
        chaseMusic.play();
        chaseMusic.setPlayingOffset(sf::seconds(1.0f));
    }
}

void AudioManager::stopChaseMusic() {
    chaseMusic.stop();
}

void AudioManager::pauseAll() {
    if (bgMusic.getStatus() == sf::SoundSource::Status::Playing) bgMusic.pause();
    if (chaseMusic.getStatus() == sf::SoundSource::Status::Playing) chaseMusic.pause();
    if (walkingSound.getStatus() == sf::SoundSource::Status::Playing) walkingSound.pause();
    // Maybe pause other sounds too if you want, but these are the main looping ones.
}

void AudioManager::resumeAll() {
    if (bgMusic.getStatus() == sf::SoundSource::Status::Paused) bgMusic.play();
    if (chaseMusic.getStatus() == sf::SoundSource::Status::Paused) chaseMusic.play();
    if (walkingSound.getStatus() == sf::SoundSource::Status::Paused) walkingSound.play();
}
