#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

class SoundManager
{
public:
    static SoundManager& instance()
    {
        static SoundManager SM;
        return SM;
    }

    bool init();
    void cleanup();

    // Música de fondo
    bool loadMusic(const std::string& filename);
    void playMusic(int loops = -1); // -1 = loop infinito
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(int volume); // 0-128

    // Efectos de sonido
    Mix_Chunk* loadSound(const std::string& filename);
    void playSound(Mix_Chunk* sound, int loops = 0);
    void freeSound(Mix_Chunk* sound);

private:
    SoundManager();
    ~SoundManager();

    Mix_Music* currentMusic;
    bool initialized;
};

#endif#pragma once