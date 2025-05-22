#pragma once
#include <SDL_Mixer.h>

#pragma once
#include <string>

#pragma once
#include "System.h"

#pragma once
#include "Utilities.h"

#pragma once
#define byte unsigned char

Mix_Music* bgMusic;

Mix_Chunk* soundFX[5];
byte soundFXChannelIds[5];

extern const byte soundFXChannels = 5;

void ClearMusic() {
    if (bgMusic == NULL) {
        return;
    }

    Mix_PauseMusic();
    Mix_RewindMusic();

    Mix_FreeMusic(bgMusic);
    bgMusic = NULL;
}

void QuePlaySFX(const char* path) {
    for (int i = 0; i < soundFXChannels; i++) {
        if (Mix_Playing(soundFXChannelIds[i])) {
            continue;
        }

        Mix_FreeChunk(soundFX[i]);
        soundFX[i] = Mix_LoadWAV(path);
        soundFXChannelIds[i] = Mix_PlayChannel(-1, soundFX[i], 0);

        break;
    }
}

void PlayMusic(const char* fileName) {
    if (strcmp(fileName, "Stop") == 0) {
        return;
    }

    Mix_VolumeMusic(16);

    bgMusic = Mix_LoadMUS(MergePaths(AUDIO_FOLDER, fileName));

    if (bgMusic == NULL) {
        PrintString("Music Cannot be Played (Likely an invalid Path)");
        PrintString(SDL_GetError());

        printf("Given File Name ");
        PrintString(fileName);

        return;
    }

    Mix_PlayMusic(bgMusic, -1);
}
