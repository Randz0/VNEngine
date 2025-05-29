#pragma once
#include <SDL.h>

extern float choiceScaling;

extern unsigned char FONTSIZE;

extern SDL_Window* gameWindow;
extern SDL_Surface* gameWindowSurface;

extern SDL_Renderer* gameRenderer;

extern bool isWindowOpen;

extern const int deltaTimeMS;
extern const float deltaTimeSec;

extern SDL_Color WHITE;

extern bool mouseDown;

extern const char* TEXT_IDENTIFIER;
extern const char* BG_IMAGE_IDENTIFIER;
extern const char* PORTRAIT_CHANGE_IDENTIFIER;
extern const char* CHOICE_IDENTIFIER;
extern const char* LOAD_MUSIC_IDENTIFIER;
extern const char* PLAY_SFX;
extern const char* FUNCTION_CALL;
extern const char* WAIT_SFX_FINISH;
extern const char* SET_FONTSIZE;

extern const char* IMAGES_FOLDER;
extern const char* FONTS_FOLDER;
extern const char* SCRIPTS_FOLDER;
extern const char* AUDIO_FOLDER;

void InitializeGlobalConstants();
