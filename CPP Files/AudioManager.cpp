#pragma once
#include <SDL_Mixer.h>

#pragma once
#include <string>

#pragma once
#include "System.h"

#pragma once
#include "Utilities.h"

Mix_Music* bgMusic;

Mix_Chunk* soundFX[5];
extern const unsigned char soundFXChannels = 5;

int lastPlayedChannel = 0;

void ClearMusic() {
    if (bgMusic == NULL) {
        return;
    }

    Mix_PauseMusic();
    Mix_RewindMusic();

    Mix_FreeMusic(bgMusic);
    bgMusic = NULL;
}

bool LastPlayedChannelFinished() {
    return !Mix_Playing(lastPlayedChannel);
}

void QuePlaySFX(const char* fileName) {
    for (int i = 0; i < soundFXChannels; i++) {
        if (Mix_Playing(i)) {
            continue;
        }

        Mix_FreeChunk(soundFX[i]);
        soundFX[i] = Mix_LoadWAV(MergePaths(AUDIO_FOLDER, fileName));

        Mix_PlayChannel(i, soundFX[i], 0);
        lastPlayedChannel = i;

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