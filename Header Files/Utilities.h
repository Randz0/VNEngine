#pragma once
#include <SDL.h>

#pragma once
#include <vector>

#pragma once
#include <string>

// Debugging functions

void PrintFloat(float toPrint);

void PrintInteger(int toPrint, bool newLine = true);

void PrintPixelCoords(int x, int y);

void PrintStringVector(std::vector<std::string> _vector);

void PrintBoolean(bool _condition);

void PrintString(const char* toPrint);

void PrintString(std::string toPrint);

// I hate c-strings, so here are some helper functions

char* CreateCStrCopy(const char* toCopy);

char* MergePaths(const char* path1, const char* path2);

size_t GetCStringByteSize(const char* string);

void OnImageLoadFail(const char* _path);

// Screen space conversion --->

// Gives the mouse position and scales it to represent the 1920x1080 screen the engine works with contained in x and y Out.
void GetMouseCoordsAsStandard(int* screenX, int* screenY);

// Assumes that the rect is working in the coords of a 1920 by 1080 screen.
SDL_Rect GetResizedCoords(SDL_Rect* rect);