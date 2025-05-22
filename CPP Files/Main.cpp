#define __STDC_WANT_LIB_EXT1__ 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <Windows.h>

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

void SetupSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("failed to init SDL. Error: ");
        printf(SDL_GetError());
    }

    if (TTF_Init() == -1) {
        printf("failed to init TTF");
    }

    if (Mix_OpenAudio(20000, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("failed to init Mix");
    }

    gameWindow = SDL_CreateWindow("TS PMO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_RESIZABLE);
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
                printf("quit the goddam game");
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
    LoadDefaultFont();

    ConfigureDefaultDialogueBox();

    dialogueText = new Text(50, 800, 1920, 300);
    
    RuntimeInterpreter* interpreter = new RuntimeInterpreter(allFunctions);

    while (interpreter->isWindowOpen && isWindowOpen) {
        PollSDLEvents(&e);

        ResizeWindowSurface();

        if (mouseDown) {
            interpreter->OnClick();
        }

        SDL_RenderClear(gameRenderer);
        
        DrawAllImages();
        interpreter->DrawChoiceButtons();
        
        SDL_RenderPresent(gameRenderer);
        
        Sleep(deltaTimeMS);
    }

    dialogueBox->FreeResources();
    delete dialogueBox;

    dialogueText->FreeResources();
    delete dialogueText;
}

void OnStartUp() {
    InitializeGlobalConstants();

    SetupSDL();

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
