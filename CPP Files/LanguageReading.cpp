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
#include "Utilities.h"

#pragma once
#include "LanguageProcessing.h"

enum class CommandType {
    DisplayText = 0,
    BG_Change = 1,
    PortraitChange = 2,
    ChoiceData = 3,
    MoreInformation = 4,
    LoadMusic = 5,
    SFX = 6,
    FunctionCall = 7,
    WaitSFXFinish = 8,
    SetFontSize = 9,
    Error = 10
};

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

    if (token == NULL) {
        _informationOut[0] = NULL;
        
        return;
    }

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
        return CommandType::ChoiceData;
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

    if (strcmp(_identifier, WAIT_SFX_FINISH) == 0) {
        return CommandType::WaitSFXFinish;
    }

    if (strcmp(_identifier, SET_FONTSIZE) == 0) {
        return CommandType::SetFontSize;
    }

    return CommandType::Error;
}

Command* LineReader::RetrieveFinalCommand(char* _identifier, char* _information) {
    Command* finalCommand;
    CommandType type = GetTypeOfCommand(_identifier);

    switch (type) {
    case CommandType::ChoiceData:
        finalCommand = new ChoiceCMD();
        break;
    case CommandType::DisplayText:
        finalCommand = new UpdateText();
        break;
    case CommandType::BG_Change:
        finalCommand = new ChangeBGPortrait();
        break;
    case CommandType::PortraitChange:
        finalCommand = new ChangePortrait();
        break;
    case CommandType::SFX:
        finalCommand = new PlaySFX();
        break;
    case CommandType::LoadMusic:
        finalCommand = new LoadMusic();
        break;
    case CommandType::FunctionCall:
        finalCommand = new FunctionCall();
        break;
    case CommandType::WaitSFXFinish:
        finalCommand = new WaitSFXFinish();
        break;
    case CommandType::SetFontSize:
        finalCommand = new SetFontSize();
        break;
    default:
        finalCommand = new EmptyCMD();
        break;
    }
    
    char* infoCopy = CreateCStrCopy(_information);
    finalCommand->ProcessCommandInfo(infoCopy);

    return finalCommand;
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

    currentCommand = NULL;

    currentLine = 0;
}

/// <summary>
/// Reads the next command within the function, returning NULL if the function has reached completion
/// </summary>
Command* Function::ReadNextCommand() {
    PerformJumpLogic();

    if (currentLine >= commands.size()) {
        return NULL;
    }

    std::string currentCommandStr = commands[currentLine];

    DeletePreviousCommand();

    currentCommand = LineReader::GetCommandFromLine(currentCommandStr);

    currentLine++;

    return currentCommand;
}

// Adds a new Jump command to the top of the Jump stack, relative to the current line
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

void Function::DeletePreviousCommand() {
    delete currentCommand;
}

#pragma endregion

#pragma region Runtime Interpreter Object

RuntimeInterpreter::RuntimeInterpreter() { /*Do Nothing*/ }

RuntimeInterpreter::RuntimeInterpreter(std::vector<Function*> _programFunctions) {
    programFunctions = _programFunctions;

    AddFunctionToStackByName("Main");
    
    Function* currentFunc = GetCurrentFunction();

    currentFunc->commands.insert(currentFunc->commands.begin(), ":Empty:NoInfo");

    currentFunc->ReadNextCommand();
    currentFunc->currentCommand->RunCommand();
}

void RuntimeInterpreter::Update() {
    Function* currentFunc = GetCurrentFunction();
    
    if (!currentFunc->currentCommand->IsFulfilled()) {
        return;
    }

    Command* currentCommand = currentFunc->ReadNextCommand();

    if (currentCommand == NULL) {
        PopMostRecentFunction();

        return;
    }

    currentFunc->currentCommand->RunCommand();
}

void RuntimeInterpreter::AddFunctionToStackByName(const char* functionName) {
    for (Function* function : programFunctions) {
        if (strcmp(function->name, functionName) != 0) {
            continue;
        }

        functionNest.push_back(function->CreateCopyForExecution());
    }
}

void RuntimeInterpreter::PopMostRecentFunction() {
    delete functionNest.back();
    functionNest.pop_back();
    
    if (functionNest.empty()) {
        isWindowOpen = false;

        return; // End of game
    }
}

Function* RuntimeInterpreter::GetCurrentFunction() {
    if (functionNest.size() <= 0) {
        return NULL;
    }
    
    return functionNest.back();
}

#pragma endregion

RuntimeInterpreter globalInterpreter;