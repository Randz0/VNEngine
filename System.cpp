#pragma once
#include <SDL.h>

extern float choiceScaling = 0.4f;

extern const unsigned char FONTSIZE = 80;
extern const char* DIALOGUE_BOX_FILENAME = "dialogueBox2.bmp";

SDL_Window* gameWindow;
SDL_Surface* gameWindowSurface;

SDL_Renderer* gameRenderer;

bool isWindowOpen = true;

extern const int deltaTimeMS = 20;
extern const float deltaTimeSec = deltaTimeMS * 0.001f;

SDL_Color WHITE;

bool mouseDown;

extern const char* TEXT_IDENTIFIER = "Text";
extern const char* BG_IMAGE_IDENTIFIER = "ChangeBg";
extern const char* PORTRAIT_CHANGE_IDENTIFIER = "ChangePortrait";
extern const char* CHOICE_IDENTIFIER = "Choice";
extern const char* LOAD_MUSIC_IDENTIFIER = "ChangeMusic";
extern const char* PLAY_SFX = "SFX";
extern const char* FUNCTION_CALL = "Call";

extern const char* SCRIPTS_FOLDER = "Assets/Scripts/";
extern const char* AUDIO_FOLDER = "Assets/Audio/";
extern const char* FONTS_FOLDER = "Assets/Fonts/";
extern const char* IMAGES_FOLDER = "Assets/Images/";

void InitializeGlobalConstants() {
    WHITE.r = 255;
    WHITE.g = 255;
    WHITE.b = 255;

    WHITE.a = 255;
}