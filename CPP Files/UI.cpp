#pragma once
#include "System.h"

#pragma once
#include "FileManagement.h"

#pragma once
#include "Renderable.h"

#pragma once
#include "Rendering.h"

#pragma once
#include "vector"

std::vector<Button> allButtons;

const int textVerticalBuffer = 15;
const int textHorizontalBuffer = 30;

int maxID = 0;

void CenterTextOnImage(Text* text, Image* img) {
	text->xPos = img->xPos + textHorizontalBuffer;
	text->maxTextWidth = img->GetWidth() - textHorizontalBuffer * 2;

	text->yPos = img->yPos + textVerticalBuffer;
	text->maxTextHeight = img->GetHeight() - textVerticalBuffer * 2;
}

// x, and y are the coords relative to a 1920 x 1080 display
// Returns the button id
int AddButton(int x, int y, float xScale, float yScale, const char* text) {
	Image* bgButtonImg = new Image(x, y, xScale, yScale);
	bgButtonImg->LoadImageIntoSurface(defaultDialogueBoxPath);
	bgButtonImg->LoadIntoTexture(true);

	Text* buttonText = new Text(0,0);
	CenterTextOnImage(buttonText, bgButtonImg);

	buttonText->text = text;

	buttonText->ReloadFont(globalFont, WHITE);
	buttonText->LoadIntoTexture();

	maxID++;

	allButtons.push_back(Button(bgButtonImg, buttonText, maxID));
	return maxID;
}

void AppendHandlerToBackButton(ButtonHandler* handler) {
	allButtons.back().buttonHandler = handler;
}

// Returns if the button is clicked
void CheckButtonClicked(Button* butt) {
	if (butt->IsMouseOver() && mouseDown) {
		if (butt->buttonHandler == NULL) {
			return;
		}

		butt->buttonHandler->OnButtonClick();
	}
}

// Returns a pointer to a button if it was clicked. Otherwise, returns NULL
void ListenForButtonClicks() {
	for (Button butt : allButtons) {
		CheckButtonClicked(&butt);
	}
}

void RemoveAllButtons() {
	for (Button butt : allButtons) {
		delete butt.buttonBg;
		delete butt.displayText;
	}

	allButtons.clear();
}

void RemoveButton(int buttId) {
	for (int i = 0; i < allButtons.size(); i++) {
		if (!allButtons.at(i).MatchesId(buttId)) {
			return;
		}

		allButtons.at(i).FreeResources();
		allButtons.erase(allButtons.begin() + i);

		return;
	}
}

void DrawAllButtons() {
	for (int i = 0; i < allButtons.size(); i++)
	{
		allButtons.at(i).RenderButton(true, true, NULL);
	}
}