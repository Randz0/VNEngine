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
#define min(x,y) (x < y ? x : y)

#pragma once
#include "Renderable.h"

#pragma once
#include "DataContainers.h"

Image bgImage;
Image portraitImage;

bool isRenderingMainDialougeBox = false;

Image* dialogueBox;
Text* dialogueText;

TTF_Font* font;

#pragma region Text Object Definitions

Text::Text() {
    xPos = 0;
    yPos = 0;

    maxTextWidth = 50;
    maxTextHeight = 50;

    underLyingSurface = NULL;
    underLyingTexture = NULL;
}

Text::Text(int _xPos, int _yPos) {
    xPos = _xPos;
    yPos = _yPos;

    maxTextWidth = 50;
    maxTextHeight = 50;

    underLyingSurface = NULL;
    underLyingTexture = NULL;
}

Text::Text(int _xPos, int _yPos, int _maxTextWidth, int _maxTextHeight) {
    xPos = _xPos;
    yPos = _yPos;

    maxTextWidth = _maxTextWidth;
    maxTextHeight = _maxTextHeight;

    underLyingSurface = NULL;
    underLyingTexture = NULL;
}

void Text::ReloadFont(TTF_Font* font, SDL_Color color) {
    SDL_FreeSurface(underLyingSurface);

    underLyingSurface = TTF_RenderText_Solid_Wrapped(font, text.c_str(), color, maxTextWidth);
}

void Text::LoadIntoTexture(bool deleteSurface) {
    SDL_DestroyTexture(underLyingTexture);
    underLyingTexture = SDL_CreateTextureFromSurface(gameRenderer, underLyingSurface);

    if (!deleteSurface) {
        return;
    }

    SDL_FreeSurface(underLyingSurface);
    underLyingSurface = NULL;
}

bool Text::BlitToScreen(SDL_Surface* blitTo) {
    if (underLyingSurface->format != gameWindowSurface->format) {
        underLyingSurface = SDL_ConvertSurface(underLyingSurface, gameWindowSurface->format, 0);
    }

    SDL_Rect rect = { xPos, yPos, underLyingSurface->w , min(underLyingSurface->h, maxTextHeight) };
    SDL_Rect scaledRect = GetResizedCoords(&rect);

    return SDL_BlitScaled(underLyingSurface, NULL, blitTo, &scaledRect) < 0;
}

bool Text::BlitTexture() {
    SDL_Rect normalRect = { xPos, yPos, 0, 0 };
    SDL_QueryTexture(underLyingTexture, NULL, NULL, &normalRect.w, &normalRect.h);

    SDL_Rect scaledRect = GetResizedCoords(&normalRect);

    return SDL_RenderCopy(gameRenderer, underLyingTexture, NULL, &scaledRect) < 0;
}

void Text::FreeResources() {
    SDL_FreeSurface(underLyingSurface);
    underLyingSurface = NULL;

    SDL_DestroyTexture(underLyingTexture);
    underLyingTexture = NULL;
}

#pragma endregion

#pragma region Image Object Definitions

Image::Image() {
    xPos = 0;
    yPos = 0;

    xScale = 1;
    yScale = 1;

    width = NULL;
    height = NULL;

    underLyingSurface = NULL;
    underLyingTexture = NULL;
}

Image::Image(int _xPos, int _yPos) {
    xPos = _xPos;
    yPos = _yPos;

    xScale = 1;
    yScale = 1;

    width = NULL;
    height = NULL;

    underLyingSurface = NULL;
    underLyingTexture = NULL;
}

Image::Image(int _xPos, int _yPos, float _xScale, float _yScale) {
    xPos = _xPos;
    yPos = _yPos;

    xScale = _xScale;
    yScale = _yScale;


    width = NULL;
    height = NULL;

    underLyingSurface = NULL;
    underLyingTexture = NULL;
}

bool Image::LoadImageIntoSurface(std::string fileName) {
    underLyingSurface = SDL_LoadBMP(MergePaths(IMAGES_FOLDER, fileName.c_str()));

    if (underLyingSurface == NULL) {
        OnImageLoadFail(fileName.c_str());

        return false;
    }

    width = underLyingSurface->w;
    height = underLyingSurface->h;

    return true;
}

// Loads the surface into a texture.
// Only call after loading as a surface
// Only deletes Surface if the texture was able to be loaded.
bool Image::LoadIntoTexture(bool deleteSurface) {
    if (underLyingSurface == NULL) {
        return false;
    }

    SDL_DestroyTexture(underLyingTexture);
    underLyingTexture = SDL_CreateTextureFromSurface(gameRenderer, underLyingSurface);

    if (underLyingTexture == NULL) {
        return false;
    }

    if (deleteSurface) {
        SDL_FreeSurface(underLyingSurface);
        underLyingSurface = NULL;
    }

    return true;
}

void Image::SetImageToScreenPercent(float _percentWidth, float _percentHeight) {
    int x;
    int y;

    xScale = 1920 * _percentWidth / width;
    yScale = 1080 * _percentHeight / height;
}

bool Image::BlitSurface(SDL_Surface* windowScreen) {
    SDL_Rect dstRect = { xPos, yPos, width * xScale, height * yScale };
    SDL_Rect scaledRect = GetResizedCoords(&dstRect);

    if (underLyingSurface->format != windowScreen->format) {
        underLyingSurface = SDL_ConvertSurface(underLyingSurface, windowScreen->format, 0);
    }

    return SDL_BlitScaled(underLyingSurface, NULL, windowScreen, &scaledRect) < 0;
}

bool Image::BlitTexture() {
    SDL_Rect dstRect = { xPos, yPos, width * xScale, height * yScale };
    SDL_Rect scaledRect = GetResizedCoords(&dstRect);

    return SDL_RenderCopy(gameRenderer, underLyingTexture, NULL, &scaledRect) < 0;
}

void Image::FreeResources() {
    SDL_FreeSurface(underLyingSurface); // Safe to pass null

    if (underLyingTexture != NULL) {
        SDL_DestroyTexture(underLyingTexture);

        underLyingTexture = NULL;
    }
}

void Image::GetBottomRight(int* _xPos, int* _yPos) {
    *_xPos = xPos + width * xScale;
    *_yPos = yPos + height * yScale;
}

int Image::GetWidth() {
    return width;
}

int Image::GetHeight() {
    return height;
}

#pragma endregion

#pragma region Button Definitions

Button::Button(Image* _buttonBg, Text* _displayText) {
    buttonBg = _buttonBg;
    displayText = _displayText;
}

// Renders The Button to the screen
// useBgTexture, and useTextTexture decide if the button background and text will be blitted as a surface or texture
// The renderer is used in the case that something is renderered as a texture.
// Don't specify the renderer if your only using surfaces to render and dont' specify the window surface if your only using Textures
bool Button::RenderButton(bool useBgTexture, bool useTextTexture, SDL_Surface* windowSurface) {
    bool bgSuccess;
    bool textSuccess;

    if (useBgTexture) {
        bgSuccess = buttonBg->BlitTexture();
    }
    else {
        bgSuccess = buttonBg->BlitSurface(windowSurface);
    }

    if (useTextTexture) {
        textSuccess = displayText->BlitTexture();
    }
    else {
        textSuccess = displayText->BlitToScreen(windowSurface);
    }

    return bgSuccess && textSuccess;
}

bool Button::IsMouseOver() {
    int mouseX;
    int mouseY;

    GetMouseCoordsAsStandard(&mouseX, &mouseY);

    int boxRight;
    int boxBottom;

    buttonBg->GetBottomRight(&boxRight, &boxBottom);

    return (mouseX <= boxRight && mouseX >= buttonBg->xPos) && (mouseY <= boxBottom && mouseY >= buttonBg->yPos);
}

#pragma endregion

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

void DrawButtons(std::vector<Button>* buttons) {
    for (int i = 0; i < buttons->size(); i++)
    {
        (*buttons)[i].RenderButton(true, true, NULL);
    }
}

void DrawAllImages() {
    bgImage.BlitTexture();
    portraitImage.BlitTexture();

    if (!isRenderingMainDialougeBox) {
        return;
    }

    dialogueBox->BlitTexture();
    dialogueText->BlitTexture();
}

void SetFont(const char* _filePath) {
    font = TTF_OpenFont(_filePath, FONTSIZE);
}

void UpdateDialogueText(char* _dialogueText) {
    dialogueText->text = _dialogueText;

    dialogueText->ReloadFont(font, WHITE);
    dialogueText->LoadIntoTexture(gameRenderer);

    isRenderingMainDialougeBox = true;
}