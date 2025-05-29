#pragma once
#include "Renderable.h"

// x, and y are the coords relative to a 1920 x 1080 display
int AddButton(int x, int y, float xScale, float yScale, const char* text);

void AppendHandlerToBackButton(ButtonHandler* handler);

void RemoveAllButtons();

void ListenForButtonClicks();

void RemoveButton(int buttId);

void DrawAllButtons();

void CenterTextOnImage(Text* text, Image* img);