#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <stdio.h>

#pragma once
#include <vector>

#pragma once
#include <string>

#pragma once
#include "System.h"

// Debug functions

void PrintFloat(float toPrint) {
    printf(std::to_string(toPrint).c_str());
}

void PrintInteger(int toPrint, bool newLine = true) {
    printf(std::to_string(toPrint).c_str());

    if (newLine) {
        printf("\n");
    }
}

void PrintPixelCoords(int x, int y) {
    printf("(");
    PrintInteger(x, false);
    printf(", ");
    PrintInteger(y);
    printf(") \n");
}

void PrintStringVector(std::vector<std::string> _vector) {
    PrintInteger(_vector.size());

    for (int i = 0; i < _vector.size(); i++) {
        printf(_vector.at(i).c_str());
        printf("\n");
    }
}

void PrintBoolean(bool _condition) {
    if (_condition) {
        printf("True \n");

        return;
    }

    printf("False \n");
}

void PrintString(const char* toPrint) { // Removes the need for adding a newline character
    printf(toPrint);
    printf("\n");
}

void PrintString(std::string toPrint) { // Overload for normal strings
    printf(toPrint.c_str());
    printf("\n");
}

/// <summary>
/// Includes the null terminator in the c-style string
/// </summary>
/// <returns>The size in bytes of the string</returns>
size_t GetCStringByteSize(const char* string) {
    return strlen(string) + 1;
}

/// <summary>
/// Merges the file-paths of two c-strings, effectively contatenating them.
/// The paths must be null terminated
/// </summary>
char* MergePaths(const char* path1, const char* path2) {
    char* combinedPath = (char*)malloc(GetCStringByteSize(path1) + GetCStringByteSize(path2));
    
    strcpy(combinedPath, path1);
    strcat(combinedPath, path2);

    return combinedPath;
}

void OnImageLoadFail(const char* _path) {
    PrintString("Could Not Load Image Of Path: ");
    PrintString(_path);
    PrintString("Potential Reason: ");
    PrintString(SDL_GetError());
}

// Screen space conversion

// Gives the mouse position and scales it to represent the 1920x1080 screen the engine works with contained in x and y Out.
void GetMouseCoordsAsStandard(int* screenX, int* screenY) {
    int windowWidth;
    int windowHeight;

    SDL_GetWindowSize(gameWindow, &windowWidth, &windowHeight);
    SDL_GetMouseState(screenX, screenY);

    *screenX *= 1920.0f / windowWidth;
    *screenY *= 1080.0f / windowHeight;
}

// Assumes that the rect is working in the coords of a 1920 by 1080 screen.
SDL_Rect GetResizedCoords(SDL_Rect* rect) {
    int x, y;
    SDL_GetWindowSize(gameWindow, &x, &y);

    float scalingConstantX = x / (float)1920;
    float scalingConstantY = y / (float)1080;

    SDL_Rect returnVal =
    { rect->x * scalingConstantX,
      rect->y * scalingConstantY,
      rect->w * scalingConstantX,
      rect->h * scalingConstantY };

    return returnVal;
}
