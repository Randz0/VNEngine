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

extern TTF_Font* globalFont;

void ResizeWindowSurface();

void DrawOverLastFrame();

void DrawAllImages();

void SetFont(const char* _filePath);

void UpdateDialogueText(char* textStruct);

void InitRenderingModule();