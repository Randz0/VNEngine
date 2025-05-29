#pragma once
#include <SDL.h>

#pragma once
#include <SDL_TTF.h>

#pragma once
#include "System.h"

#pragma once
#include <string>

#pragma once
#include <vector>

#pragma once
#include "Utilities.h"

#pragma once
#include "Renderable.h"

#pragma once
#include "DataContainers.h"

#pragma once
#include "UI.h"

Image bgImage;
Image portraitImage;

bool isRenderingMainDialougeBox = false;

Image* dialogueBox;
Text* dialogueText;

TTF_Font* globalFont;

void ResizeWindowSurface() {
    SDL_Surface* temp = SDL_GetWindowSurface(gameWindow);

    if (temp == gameWindowSurface) {
        return; // No need to do anything if surfaces are the same
    }

    gameWindowSurface = temp;
}

void DrawOverLastFrame() {
    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
    SDL_Rect screenRect = { 0, 0, gameWindowSurface->w, gameWindowSurface->h };

    SDL_RenderFillRect(gameRenderer, &screenRect);
}

void DrawAllImages() {
    bgImage.BlitTexture();
    portraitImage.BlitTexture();

    if (!isRenderingMainDialougeBox) {
        return;
    }

    dialogueBox->BlitTexture();
    dialogueText->BlitTexture();

    DrawAllButtons();
}

void SetFont(const char* _filePath) {
    TTF_CloseFont(globalFont);
    globalFont = TTF_OpenFont(_filePath, FONTSIZE);
}

void UpdateDialogueText(char* _dialogueText) {
    dialogueText->text = _dialogueText;

    dialogueText->ReloadFont(globalFont, WHITE);
    dialogueText->LoadIntoTexture(gameRenderer);

    isRenderingMainDialougeBox = true;
}

void InitRenderingModule() {
    bgImage = Image(0, 0, 1, 1);
    isRenderingMainDialougeBox = true;
}