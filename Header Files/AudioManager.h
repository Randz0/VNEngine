#pragma once
#include <SDL_Mixer.h>

extern Mix_Music* bgMusic;

extern Mix_Chunk* soundFX[5];
extern unsigned char soundFXChannelIds[5];

extern const unsigned char soundFXChannels;

bool LastPlayedChannelFinished();

void ClearMusic();

void QuePlaySFX(const char* path);

void PlayMusic(const char* path);