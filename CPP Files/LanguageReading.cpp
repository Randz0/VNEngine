#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <SDL.h>
#pragma once
#include <SDL_audio.h>
#pragma once
#include <SDL_ttf.h>
#pragma once
#include <SDL_mixer.h>

#pragma once
#include <Windows.h>

#pragma once
#include <string>

#pragma once
#include <vector>

#pragma once
#include "AudioManager.h"

#pragma once
#include "Rendering.h"

#pragma once 
#include <cstring>

#pragma once
#include "System.h"

#pragma once
#include "LanguageProcessing.h"

enum class CommandType {
    DisplayText = 0,
    BG_Change = 1,
    PortraitChange = 2,
    Choice = 3,
    MoreInformation = 4,
    LoadMusic = 5,
    SFX = 6,
    FunctionCall = 7,
    Error = 8
};

#pragma endregion

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

#pragma region Choice Struct

Choice::Choice() {
    firstOption = NULL;

    dialoguePath = 0;
}

Choice::Choice(LLCharArrElement* _firstOption, int _totalOptions) {
    firstOption = _firstOption;
    totalOptions = _totalOptions;
}

Choice::~Choice() {
    LLCharArrElement* deletingNow = NULL;
    LLCharArrElement* deletingNext = firstOption;

    for (int i = totalOptions; i > 1; i--) {
        deletingNow = deletingNext;
        deletingNext = deletingNow->next;

        delete deletingNow;
    }

    delete deletingNext;
}

#pragma endregion

#pragma region LineReader Functions

const unsigned char IDENTIFIER_LENGTH = 15;
const unsigned char INFORMATION_LENGTH = 100;

Command* LineReader::GetCommandFromLine(std::string currentCommand) {
    char identifier[IDENTIFIER_LENGTH];
    char information[INFORMATION_LENGTH];

    ParseCommandSegments(identifier, information, currentCommand);

    return RetrieveFinalCommand(identifier, information);
}

/// <summary>
/// Responsible for dividing up the command into readable chunks by the compiler. In essence, each line consist of 2 parts, the identifier and information
/// </summary>
/// <param name="identifierOut"></param> The type of command being performed
/// <param name="informationOut"></param> Relevant information to that command
/// <param name="currentCommand"></param> The command being processed
/// <returns></returns>
void LineReader::ParseCommandSegments(char* _identifierOut, char* _informationOut, std::string _currentCommand) {
    char editableCopy[200]; // Getting A Copy To Split Setting buffer to 100

    strcpy_s(editableCopy, sizeof(editableCopy), _currentCommand.c_str());

    char* internalState = NULL;

    char* token = strtok_s(editableCopy, ":", &internalState); // Tokenizing

    //Getting the identifier part
    strcpy_s(_identifierOut, IDENTIFIER_LENGTH, token);

    token = strtok_s(NULL, ":", &internalState);

    //Getting the information
    strcpy_s(_informationOut, INFORMATION_LENGTH, token);
}

CommandType LineReader::GetTypeOfCommand(char* _identifier) {
    if (strcmp(_identifier, TEXT_IDENTIFIER) == 0) {
        return CommandType::DisplayText;
    }

    if (strcmp(_identifier, BG_IMAGE_IDENTIFIER) == 0) {
        return CommandType::BG_Change;
    }

    if (strcmp(_identifier, PORTRAIT_CHANGE_IDENTIFIER) == 0) {
        return CommandType::PortraitChange;
    }

    if (strcmp(_identifier, CHOICE_IDENTIFIER) == 0) {
        return CommandType::Choice;
    }

    if (strcmp(_identifier, LOAD_MUSIC_IDENTIFIER) == 0) {
        return CommandType::LoadMusic;
    }

    if (strcmp(_identifier, PLAY_SFX) == 0) {
        return CommandType::SFX;
    }

    if (strcmp(_identifier, FUNCTION_CALL) == 0) {
        return CommandType::FunctionCall;
    }

    return CommandType::Error;
}

Choice* LineReader::GenerateChoiceInfo(char* _information) {
    char* internalState = NULL;
    char* extractedVal = strtok_s(_information, ";", &internalState);

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

    return new Choice(firstStr, totalOptions);
}

Command* LineReader::RetrieveFinalCommand(char* _identifier, char* _information) {
    Command* finalCommand = new Command(); // Using new to allocate, must use delete to free
    finalCommand->type = GetTypeOfCommand(_identifier);

    if (finalCommand->type == CommandType::Choice) {
        finalCommand->info = GenerateChoiceInfo(_information);

        return finalCommand;
    }

    if (finalCommand->type != CommandType::Error) {
        finalCommand->info = malloc(strlen(_information) + 1);

        strcpy_s((char*)finalCommand->info, strlen(_information) + 1, _information);

        return finalCommand;
    }
}

#pragma endregion

#pragma region Function Object

Function::Function(char* _name) {
    strcpy_s(name, sizeof(name), _name);

    currentLine = 0;
}

Function::Function(std::vector<std::string> _commands, char* _name) {
    commands = _commands;
    strcpy_s(name, sizeof(name), _name);

    currentCommand = new Command();

    currentCommand->type = CommandType::Error;
    currentCommand->info = new char; // Empty Command

    currentLine = 0;
}

/// <summary>
/// Reads the next command within the function, returning true if the function has reached completion
/// </summary>
bool Function::ReadNextCommand() {
    PerformJumpLogic();

    if (currentLine >= commands.size()) {
        return true;
    }

    std::string currentCommandStr = commands[currentLine];

    DeletePreviousCommand();

    currentCommand = LineReader::GetCommandFromLine(currentCommandStr);

    currentLine++;

    return false;
}

// Adds a new Jump command to the top of the Jump stack
void Function::SetupJump(int forwardAmount) {
    if (firstInQue == NULL) {
        IntLLNode* first = (IntLLNode*)malloc(sizeof(IntLLNode));

        first->value = currentLine + forwardAmount;
        first->next = NULL;

        firstInQue = first;
        lastInQue = first;

        return;
    }

    IntLLNode* nextJump = (IntLLNode*)malloc(sizeof(IntLLNode));

    nextJump->value = currentLine + forwardAmount;
    nextJump->next = NULL;

    lastInQue->next = nextJump;
    lastInQue = nextJump;
}

/// <summary>
/// Creates a copy of the function that can be executed from the start of the function
/// </summary>
/// <returns>A Copy Containing the same command lines that can be read from the first line</returns>
Function* Function::CreateCopyForExecution() {
    Function* copy = new Function(commands, name);

    return copy;
}

void Function::PerformJumpLogic() {
    if (firstInQue == NULL) {
        return; // No Jump Commands In Waiting
    }

    currentLine = firstInQue->value;
    IntLLNode* nextInQue = firstInQue->next;

    free(firstInQue);
    firstInQue = nextInQue;
}

#define CommandRequiresFree(type) (type == CommandType::LoadMusic || type == CommandType::SFX)

void Function::DisposeObjectCommandInfo() {
    if (currentCommand->type == CommandType::Choice) {
        Choice* choiceInfo = reinterpret_cast<Choice*>(currentCommand->info);

        delete choiceInfo;
        return;
    }

    delete currentCommand->info;
}

void Function::DeletePreviousCommand() {
    if (CommandRequiresFree(currentCommand->type)) {
        free(currentCommand->info);
    }
    else {
        DisposeObjectCommandInfo();
    }

    delete currentCommand;
}

#pragma endregion

enum AdvancementState {
    cannotProceed = 0, // Do not continue to execute
    proceedForward = 1, // Code Simply Continues processing down the file

    choiceSelected = 2 // Code is taking a path that branches off
};

#pragma region Runtime Interpreter Object

RuntimeInterpreter::RuntimeInterpreter(std::vector<Function*> _programFunctions) {
    programFunctions = _programFunctions;

    FindMainFunc();

    InterpretNextCommand(); // Setting the system up with the first command
}

void RuntimeInterpreter::OnClick() {
    advancementStatus = GetAdvancementStatus();

    switch (advancementStatus) {
    case AdvancementState::proceedForward:
        InterpretNextCommand();
        break;
    case AdvancementState::choiceSelected:
        ChoiceSelected();
        break;
    }
}

void RuntimeInterpreter::FreeResources() {
    TTF_CloseFont(font);
}

void RuntimeInterpreter::DrawChoiceButtons() {
    DrawButtons(&options);
}

void RuntimeInterpreter::FindMainFunc() {
    for (int i = 0; i < programFunctions.size(); i++) {
        if (strcmp(programFunctions.at(i)->name, "Main") == 0) { // Finding the Main Function of the codebase
            functionNest.push_back(programFunctions.at(i)->CreateCopyForExecution());
        }
    }
}

AdvancementState RuntimeInterpreter::GetAdvancementStatus() {
    if (GetCurrentFunction()->currentCommand->type == CommandType::DisplayText) {
        return AdvancementState::proceedForward;
    }

    if (GetCurrentFunction()->currentCommand->type == CommandType::Choice && CheckButtons()) {
        return AdvancementState::choiceSelected;
    }

    return AdvancementState::cannotProceed;
}

/// <summary>
/// Called whenever the program recieves user input and tells the system what to do in the case that a button was pressed
/// </summary>
/// <returns></returns>
bool RuntimeInterpreter::CheckButtons() {
    for (int i = 0; i < options.size(); i++) {
        if (options[i].IsMouseOver()) {
            Choice* info = reinterpret_cast<Choice*>(GetCurrentFunction()->currentCommand->info);
            info->dialoguePath = i;

            return true;
        }
    }

    return false;
}

void RuntimeInterpreter::UpdateBGChange() {
    bgImage = Image(0, 0, 1, 1);

    char* imgPath = reinterpret_cast<char*>(GetCurrentFunction()->currentCommand->info);

    bgImage.LoadImageIntoSurface(imgPath);
    bgImage.LoadIntoTexture(gameRenderer);

    bgImage.SetImageToScreenPercent(1, 1);
}

void RuntimeInterpreter::UpdatePortraitChange() {
    portraitImage = Image(0, 0);

    char* imgPath = reinterpret_cast<char*>(GetCurrentFunction()->currentCommand->info);

    portraitImage.LoadImageIntoSurface(imgPath);
    portraitImage.LoadIntoTexture(gameRenderer);

    portraitImage.xPos = 960 - portraitImage.GetWidth() / 2;
    portraitImage.yPos = 540 - portraitImage.GetHeight() / 2;
}

void RuntimeInterpreter::LoadChoice(Choice* _info) {
    LLCharArrElement* current = _info->firstOption;

    for (float i = 0; i < _info->totalOptions; i++) {
        int yPos = i * choiceScaling * 350 + 300;

        Image* img = new Image(0, yPos, 0.390625f, choiceScaling);
        img->LoadImageIntoSurface(DIALOGUE_BOX_FILENAME);
        img->LoadIntoTexture(true);

        Text* txt = new Text(50 * choiceScaling, yPos + 30 * choiceScaling, 750, 300 * choiceScaling);
        txt->text = current->element;

        txt->ReloadFont(font, WHITE);
        txt->LoadIntoTexture();

        options.push_back(Button(img, txt));

        current = current->next;
    }
}

void RuntimeInterpreter::ChoiceSelected() {
    Choice* info = reinterpret_cast<Choice*>(GetCurrentFunction()->currentCommand->info);

    ClearButtons();

    GetCurrentFunction()->SetupJump(info->dialoguePath);
    GetCurrentFunction()->SetupJump(info->totalOptions);

    InterpretNextCommand(); // Call the split in code
}

void RuntimeInterpreter::UpdateChoice() {
    Choice* info = reinterpret_cast<Choice*>(GetCurrentFunction()->currentCommand->info);

    LoadChoice(info);
}

void RuntimeInterpreter::UpdateMusic() {
    ClearMusic();

    char* path = (char*)(GetCurrentFunction()->currentCommand->info);

    PlayMusic(path);
}

void RuntimeInterpreter::PlaySFX() {
    QuePlaySFX((char*)GetCurrentFunction()->currentCommand->info);
}

void RuntimeInterpreter::AddFunctionToStack() {
    char* functionName = (char*)functionNest.back()->currentCommand->info;

    for (Function* function : programFunctions) {
        if (strcmp(function->name, functionName) != 0) {
            continue;
        }
        
        functionNest.push_back(function->CreateCopyForExecution());
    }
}

// Returns if the next command should be called afterwards
bool RuntimeInterpreter::ProcessCurrentCommand() {
    switch (GetCurrentFunction()->currentCommand->type) {
    case CommandType::DisplayText:
        UpdateDialogueText(reinterpret_cast<char*>(GetCurrentFunction()->currentCommand->info));

        return false;
    case CommandType::BG_Change:
        UpdateBGChange();

        return true;
    case CommandType::PortraitChange:
        UpdatePortraitChange();

        return true;
    case CommandType::Choice:
        UpdateChoice();

        return false;
    case CommandType::LoadMusic:
        UpdateMusic();

        return true;

    case CommandType::SFX:
        PlaySFX();

        return true;
    case CommandType::FunctionCall:
        AddFunctionToStack();
        
        return true;
    }

    return true;
}

void RuntimeInterpreter::PopMostRecentFunction() {
    delete functionNest.back();
    functionNest.pop_back();
    
    if (functionNest.empty()) {
        isWindowOpen = false;

        return; // End of game
    }

    InterpretNextCommand();
}

void RuntimeInterpreter::InterpretNextCommand() {
    if (GetCurrentFunction()->ReadNextCommand()) {
        PopMostRecentFunction();

        return;
    }

    if (ProcessCurrentCommand()) {
        InterpretNextCommand(); // Only called when the interpreter can move safely immediately after something happens
    }
}

void RuntimeInterpreter::RenderStandardText() {
    dialogueBox->BlitTexture();
    dialogueText->BlitTexture();
}

void RuntimeInterpreter::ClearButtons() {
    for (Button butt : options) {
        butt.buttonBg->FreeResources();
        butt.displayText->FreeResources();
    }

    options.clear();
}

Function* RuntimeInterpreter::GetCurrentFunction() {
    if (functionNest.size() <= 0) {
        return NULL;
    }
    
    return functionNest.back();
}

#pragma endregion
