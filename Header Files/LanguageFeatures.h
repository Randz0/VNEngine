#pragma once
#include <SDL.h>

#pragma once
#include <vector>

#pragma once
#include "DataContainers.h"

#pragma once
#include "Renderable.h"


class Function { // represents a function in the scripting language
public:
    char name[30];

    std::vector<std::string> commands; // The script written commands
    int currentLine; // The current line (from the start if the function) of the function

    Command* currentCommand;

    IntLLNode* firstInQue; // If the program needs to jump between lines, its handled by a stack stored thru a linked list managing front and back
    IntLLNode* lastInQue;

    Function(char* _name);

    Function(std::vector<std::string> _commands, char* _name);

    /// <summary>
    /// Reads the next command within the function, returning true if the function has reached completion
    /// </summary>
    Command* ReadNextCommand();

    // Adds a new Jump command to the top of the Jump stack
    void SetupJump(int forwardAmount);

    /// <summary>
    /// Creates a copy of the function that can be executed from the start of the function
    /// </summary>
    /// <returns>A Copy Containing the same command lines that can be read from the first line</returns>
    Function* CreateCopyForExecution();

private:
    void PerformJumpLogic();

    void DisposeObjectCommandInfo();

    void DeletePreviousCommand();
};

class RuntimeInterpreter {
public:
    std::vector<Function*> programFunctions; // All of the possible functions that can be ran
    std::vector<Function*> functionNest; // Contains all of the functions in the order they are nested in

    bool isWindowOpen = true;

    // The variables below are involved when handling control logic WITHIN a function, they are temporary and should not be used to handle logic
    // That is responsible for inter-function responsibilties

    RuntimeInterpreter();

    RuntimeInterpreter(std::vector<Function*> _programFunctions);

    void Update();

    void FreeResources();

    Function* GetCurrentFunction();

    void AddFunctionToStackByName(const char* functionName);

private:
    void PopMostRecentFunction();
};