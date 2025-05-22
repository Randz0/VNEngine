#pragma once
#include <SDL.h>

#pragma once
#include <SDL_TTF.h>

#pragma once
#include <string>

#pragma once
#include <vector>

#pragma once
#include "Renderable.h"

extern Image bgImage;
extern Image portraitImage;

extern bool isRenderingMainDialougeBox;

extern Image* dialogueBox;
extern Text* dialogueText;

extern TTF_Font* font;

void ResizeWindowSurface();

void DrawOverLastFrame();

void DrawButtons(std::vector<Button>* buttons);

void DrawAllImages();

void SetFont(const char* _filePath);

void UpdateDialogueText(char* textStruct);
