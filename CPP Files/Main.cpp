#define __STDC_WANT_LIB_EXT1__ 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#pragma once
#include <thread>
#pragma once
#include <chrono>

#include <stdio.h>

#include <string>
#include <fstream>

#include <vector>

#include "System.h"

#include "Utilities.h"
#include "AudioManager.h"

#include "Rendering.h"

#include "LanguageFeatures.h"
#include "LanguageProcessing.h"

#include "FileManagement.h"
#include "UI.h"

void SetupSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("failed to init SDL. Error: ");
        printf(SDL_GetError());
    }

    if (TTF_Init() == -1) {
        printf("failed to init TTF");
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("failed to init Mix");
    }

    gameWindow = SDL_CreateWindow("VN Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_RESIZABLE);
    gameWindowSurface = SDL_GetWindowSurface(gameWindow);

    gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
}

void CloseSDL() {
    SDL_DestroyRenderer(gameRenderer);
    gameRenderer = NULL;

    SDL_DestroyWindow(gameWindow);
    gameWindow = NULL;

    gameWindowSurface = NULL;
    
    Mix_FreeMusic(bgMusic);
    bgMusic = NULL;

    for (int i = 0; i < soundFXChannels; i++) {
        Mix_FreeChunk(soundFX[i]);

        soundFX[i] = NULL;
    }

    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void PollSDLEvents(SDL_Event* e) {
    mouseDown = false;

    while (SDL_PollEvent(e)) {
        switch (e->type) {
            case SDL_QUIT:
                isWindowOpen = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseDown = true;
                break;
        }
    }
}

void PerformGameLoop() {
    SDL_Event e;

    PerformPreprocessing("Main.txt");
    
    std::vector<Function*> allFunctions = GetLoadedFunctions();
    LoadFont();

    ConfigureDefaultDialogueBox();

    dialogueText = new Text(0, 0);
    CenterTextOnImage(dialogueText, dialogueBox);

    globalInterpreter = RuntimeInterpreter(allFunctions);

    std::chrono::duration<int, std::ratio<1, 1000>> sleepTime = std::chrono::milliseconds(deltaTimeMS);

    while (globalInterpreter.isWindowOpen && isWindowOpen) {
        PollSDLEvents(&e);

        ResizeWindowSurface();

        ListenForButtonClicks(); // Order matters because otherwise, script may auto-fire a button input
        globalInterpreter.Update();

        SDL_RenderClear(gameRenderer);
        
        DrawAllImages();

        SDL_RenderPresent(gameRenderer);
        
        std::this_thread::sleep_for(sleepTime);
    }

    dialogueBox->FreeResources();
    delete dialogueBox;

    dialogueText->FreeResources();
    delete dialogueText;
}

void OnStartUp() {
    InitializeGlobalConstants();
    
    SetupSDL();

    InitRenderingModule();

    LoadData();
}

void OnQuit() {
    WriteData();

    CloseSDL();
}

int main(int argc, char* args[]) {
    OnStartUp();

    PerformGameLoop();

    OnQuit();

    return 0;
}