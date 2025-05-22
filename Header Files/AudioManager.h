#pragma once
#include <SDL_Mixer.h>

#pragma once
#define byte unsigned char

extern Mix_Music* bgMusic;

extern Mix_Chunk* soundFX[5];
extern byte soundFXChannelIds[5];

extern const byte soundFXChannels;

void ClearMusic();

void QuePlaySFX(const char* path);

void PlayMusic(const char* path);
