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
#include <stdio.h>

#pragma once
#include <string>
#pragma once
#include <fstream>

#pragma once
#include <vector>

#pragma once
#include "AudioManager.h"

#pragma once
#include "Rendering.h"

#pragma once
#include "LanguageFeatures.h"

#pragma once
#include "Renderable.h"

class LineReader { // Responsible for parsing individual lines in the script to actual commands
public:

    static Command* GetCommandFromLine(std::string currentCommand);

private:

    /// <summary>
    /// Responsible for dividing up the command into readable chunks by the compiler. In essence, each line consist of 2 parts, the identifier and information
    /// </summary>
    /// <param name="identifierOut"></param> The type of command being performed
    /// <param name="informationOut"></param> Relevant information to that command
    /// <param name="currentCommand"></param> The command being processed
    /// <returns></returns>
    static void ParseCommandSegments(char* _identifierOut, char* _informationOut, std::string _currentCommand);

    static CommandType GetTypeOfCommand(char* _identifier);

    static Choice* GenerateChoiceInfo(char* _information);

    static Command* RetrieveFinalCommand(char* _identifier, char* _information);
};
