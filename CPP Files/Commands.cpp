#pragma once
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include "DataContainers.h"

#pragma once
#include "System.h"

#pragma once
#include "FileManagement.h"

#pragma once
#include "AudioManager.h"

#pragma once
#include "Rendering.h"

#pragma once
#include "LanguageProcessing.h"

#pragma once
#include "UI.h"

#pragma region Linked List Char Array Element Struct

// Gets the array at the index specified, looping back over to the first if necessarry
LLCharArrElement* LLCharArrElement::GetElementForward(int fwdAmount) {
    LLCharArrElement* current = this;

    for (int i = 0; i < fwdAmount; i++) {
        current = current->next;
    }

    return current;
}

LLCharArrElement::~LLCharArrElement() {
    delete[] element;
}

#pragma endregion

Command::~Command() {
    // does nothing, base class has nothing to clean up
}

void EmptyCMD::ProcessCommandInfo(char* info) {
    free(info);
}

void EmptyCMD::RunCommand() {
    return;
}

bool EmptyCMD::IsFulfilled() {
    return true;
}

EmptyCMD::~EmptyCMD() {
    return;
}

void FunctionCall::ProcessCommandInfo(char* info) {
    functionName = info;
}

void FunctionCall::RunCommand() {
    globalInterpreter.AddFunctionToStackByName(functionName);

    globalInterpreter.functionNest.back()->ReadNextCommand();
    globalInterpreter.functionNest.back()->currentCommand->RunCommand();
}

bool FunctionCall::IsFulfilled() {
    return true;
}

FunctionCall::~FunctionCall() {
    free(functionName);
}

void UpdateText::ProcessCommandInfo(char* info) {
	textField = (char*)info;
}

void UpdateText::RunCommand() {
	UpdateDialogueText(textField);
}

bool UpdateText::IsFulfilled() {
    return mouseDown;
}

UpdateText::~UpdateText() {
	free(textField);
}

void ChangeBGPortrait::ProcessCommandInfo(char* info) {
	imageFilename = info;
}

void ChangeBGPortrait::RunCommand() {
	bgImage.LoadImageIntoSurface(imageFilename);
    bgImage.SetImageToScreenPercent(1, 1);

    bgImage.LoadIntoTexture(true);
}

bool ChangeBGPortrait::IsFulfilled() {
    return true;
}

ChangeBGPortrait::~ChangeBGPortrait() {
	free(imageFilename);
}

void ChangePortrait::ProcessCommandInfo(char* info) {
	imageFileName = info;
}

void ChangePortrait::RunCommand() {
	portraitImage.LoadImageIntoSurface(imageFileName);
    portraitImage.LoadIntoTexture(true);

    portraitImage.xPos = 960 - portraitImage.GetWidth() * portraitImage.xScale * 0.5f;
    portraitImage.yPos = 540 - portraitImage.GetHeight() * portraitImage.yScale * 0.5f;
}

bool ChangePortrait::IsFulfilled() {
    return true;
}

ChangePortrait::~ChangePortrait() {
	free(imageFileName);
}

void PlaySFX::ProcessCommandInfo(char* info) {
	fileName = info;
}

void PlaySFX::RunCommand() {
	QuePlaySFX(fileName);
}

bool PlaySFX::IsFulfilled() {
    return true;
}

PlaySFX::~PlaySFX() {
    free(fileName);
}

void LoadMusic::ProcessCommandInfo(char* info) {
    fileName = info;
}

void LoadMusic::RunCommand() {
    PlayMusic(fileName);
}

bool LoadMusic::IsFulfilled() {
    return true;
}

LoadMusic::~LoadMusic() {
	free(fileName);
}

#pragma region ChoiceData Struct

ChoiceData::ChoiceData() {
    firstOption = NULL;
    choiceSelected = false;
}

ChoiceData::ChoiceData(LLCharArrElement* _firstOption, int _totalOptions) {
    firstOption = _firstOption;
    totalOptions = _totalOptions;

    choiceSelected = false;
}

ChoiceData::~ChoiceData() {
    LLCharArrElement* deletingNow = NULL;
    LLCharArrElement* deletingNext = firstOption;

    for (int i = totalOptions; i > 1; i--) {
        deletingNow = deletingNext;
        deletingNext = deletingNow->next;

        delete deletingNow;
    }

    delete deletingNext;

    for (int i = 0; i < totalOptions; i++) {
        RemoveButton(choiceButtons[i]);
    }

    delete[] choiceButtons;
}

#pragma endregion

class ChoiceButtonHandler : public ButtonHandler {
public:
    ChoiceButtonHandler(int _dialoguePath, int _totalOptions, bool* choiceSelectedRef) {
        dialoguePath = _dialoguePath;
        totalOptions = _totalOptions;

        choiceSelected = choiceSelectedRef;
    }

    bool OnButtonClick() override {
        globalInterpreter.GetCurrentFunction()->SetupJump(dialoguePath);
        globalInterpreter.GetCurrentFunction()->SetupJump(totalOptions);

        *choiceSelected = true;

        return true;
    };

private:
    int dialoguePath;
    int totalOptions;

    bool* choiceSelected;
};

void ChoiceCMD::ProcessCommandInfo(char* info) {
    char* internalState = NULL;
    char* extractedVal = strtok_s(info, ";", &internalState);

    int totalOptions = 0;
    LLCharArrElement* currentStr = new LLCharArrElement();
    LLCharArrElement* firstStr = currentStr;

    while (extractedVal != NULL) {
        totalOptions += 1;

        currentStr->element = new char[strlen(extractedVal) + 1]; // Allocating the str
        strcpy(currentStr->element, extractedVal);

        currentStr->next = new LLCharArrElement();
        currentStr = currentStr->next; // Getting to the next element in the linked list

        extractedVal = strtok_s(NULL, ";", &internalState);
    }

    firstStr->GetElementForward(totalOptions - 1)->next = firstStr; // So the pointers all loop back to each other
    free(info);

    choiceData = new ChoiceData(firstStr, totalOptions);
}

void ChoiceCMD::RunCommand() {
    LLCharArrElement* current = choiceData->firstOption;

    choiceData->choiceButtons = new int[choiceData->totalOptions];

    for (int i = 0; i < choiceData->totalOptions; i++) {
        int yPos = i * choiceScaling * 400 + 100;

        choiceData->choiceButtons[i] = AddButton(0, yPos, 0.390625f, 0.5f, current->element);
        AppendHandlerToBackButton(new ChoiceButtonHandler(i, choiceData->totalOptions, &(choiceData->choiceSelected)) );

        current = current->next;
    }
}

bool ChoiceCMD::IsFulfilled() {
    return choiceData->choiceSelected;
}

ChoiceCMD::~ChoiceCMD() {
    delete choiceData;
}

void WaitSFXFinish::ProcessCommandInfo(char* info) {
    free(info);
}

void WaitSFXFinish::RunCommand() {
    return;
}

bool WaitSFXFinish::IsFulfilled() {
    return LastPlayedChannelFinished();
}

WaitSFXFinish::~WaitSFXFinish() {
    return;
}

void SetFontSize::ProcessCommandInfo(char* info) {
    try {
        fontSize = std::stoi(info);
    }
    catch (std::invalid_argument excep) {
        fontSize = FONTSIZE;
    }
}

void SetFontSize::RunCommand() {
    FONTSIZE = fontSize;
    LoadFont();
}

bool SetFontSize::IsFulfilled() {
    return true;
}

SetFontSize::~SetFontSize() {
    return;
}