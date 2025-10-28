#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager()
{
    currentMusic = nullptr;
    initialized = false;
}

SoundManager::~SoundManager()
{
    cleanup();
}

bool SoundManager::init()
{
    if (initialized)
        return true;

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cout << "error SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "error Mix_OpenAudio: " << Mix_GetError() << std::endl;
        return false;
    }

    initialized = true;
    return true;
}

void SoundManager::cleanup()
{
    if (currentMusic != nullptr)
    {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();
    initialized = false;
}

bool SoundManager::loadMusic(const std::string& filename)
{
    if (currentMusic != nullptr)
    {
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }

    currentMusic = Mix_LoadMUS(filename.c_str());
    if (currentMusic == nullptr)
    {
        std::cout << "uploading music error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void SoundManager::playMusic(int loops)
{
    if (currentMusic != nullptr)
    {
        if (Mix_PlayMusic(currentMusic, loops) == -1)
        {
            std::cout << "reproducing music error: " << Mix_GetError() << std::endl;
        }
    }
}

void SoundManager::stopMusic()
{
    Mix_HaltMusic();
}

void SoundManager::pauseMusic()
{
    Mix_PauseMusic();
}

void SoundManager::resumeMusic()
{
    Mix_ResumeMusic();
}

void SoundManager::setMusicVolume(int volume)
{
    Mix_VolumeMusic(volume); // 0-128
}

Mix_Chunk* SoundManager::loadSound(const std::string& filename)
{
    Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
    if (sound == nullptr)
    {
        std::cout << "uploading sound error: " << Mix_GetError() << std::endl;
    }
    return sound;
}

void SoundManager::playSound(Mix_Chunk* sound, int loops)
{
    if (sound != nullptr)
    {
        Mix_PlayChannel(-1, sound, loops);
    }
}

void SoundManager::freeSound(Mix_Chunk* sound)
{
    if (sound != nullptr)
    {
        Mix_FreeChunk(sound);
    }
}