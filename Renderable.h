#pragma once
#include <SDL.h>

#pragma once
#include <SDL_TTF.h>

#pragma once
#include <string>

class Text {
public:
    int xPos;
    int yPos;

    int maxTextWidth;
    int maxTextHeight;

    Text();

    Text(int _xPos, int _yPos);

    Text(int _xPos, int _yPos, int _maxTextWidth, int _maxTextHeight);

    void ReloadFont(TTF_Font* font, SDL_Color color);

    void LoadIntoTexture(bool deleteSurface = false);

    bool BlitToScreen(SDL_Surface* blitTo);

    bool BlitTexture();

    void FreeResources();

    std::string text;
private:
    SDL_Surface* underLyingSurface;
    SDL_Texture* underLyingTexture;
};

class Image {
public:
    int xPos;
    int yPos;

    float xScale;
    float yScale;

    Image();

    Image(int _xPos, int _yPos);

    Image(int _xPos, int _yPos, float _xScale, float _yScale);

    bool LoadImageIntoSurface(std::string _path);

    // Loads the surface into a texture.
    // Only call after loading as a surface
    // Only deletes Surface if the texture was able to be loaded.
    bool LoadIntoTexture(bool deleteSurface = true);

    void SetImageToScreenPercent(float _percentWidth, float _percentHeight);

    bool BlitSurface(SDL_Surface* windowScreen);

    bool BlitTexture();

    void FreeResources();

    void GetBottomRight(int* _xPos, int* _yPos);

    int GetWidth();
    int GetHeight();

private:
    SDL_Surface* underLyingSurface;
    SDL_Texture* underLyingTexture;

    int width;
    int height;
};

class Button {
public:
    Image* buttonBg;
    Text* displayText;

    Button(Image* _buttonBg, Text* _displayText);

    // Renders The Button to the screen
    // useBgTexture, and useTextTexture decide if the button background and text will be blitted as a surface or texture
    // The renderer is used in the case that something is renderered as a texture.
    // Don't specify the renderer if your only using surfaces to render and dont' specify the window surface if your only using Textures
    bool RenderButton(bool useBgTexture = false, bool useTextTexture = false, SDL_Surface* windowSurface = NULL);

    bool IsMouseOver();

private:
    SDL_Surface* underLyingSurface;
    SDL_Texture* underLyingTexture;
};