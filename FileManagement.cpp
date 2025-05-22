#pragma once
#include "System.h"

#pragma once
#include "Utilities.h"

#pragma once
#include "LanguageFeatures.h"

#pragma once
#include "Rendering.h";

#pragma once
#include <vector>

#pragma once
#include <fstream>

char* defaultDialogueBoxPath;
char* defaultFontName;

void PerformPreprocessing(std::string scriptFilename);

void ConfigureDefaultDialogueBox() {
    Image* loadedDialogueBox = new Image(0, 780, 1, 1);
    loadedDialogueBox->LoadImageIntoSurface(defaultDialogueBoxPath);
    loadedDialogueBox->LoadIntoTexture(true);

    dialogueBox = loadedDialogueBox;
}

void LoadDefaultFont() {
    char* path = MergePaths(FONTS_FOLDER, defaultFontName);
    
    SetFont(path);

    free(path);
}

/// <summary>
/// Loads the contents of your saved data into the system
/// </summary>
void LoadData() {
    std::fstream* stream = new std::fstream();

    stream->open("SaveData.txt", std::ios::in);

    std::string readData;

    // Lalala we are not doing this rn

    stream->close();

    delete stream;
}

Function* ReadInitalFunctionData(std::string _lineData) {
    char copiedFunctionName[40];
    strcpy_s(copiedFunctionName, _lineData.substr(6).c_str());

    return new Function(copiedFunctionName);
}

void HandleDirective(std::string directive) {
    if (directive.find("font=") == 1) {
        defaultFontName = (char*)malloc(40);

        strcpy_s(defaultFontName, directive.length() - 4 /*Including the null character */, directive.substr(6).c_str());
        return;
    }

    if (directive.find("box=") == 1) {
        defaultDialogueBoxPath = (char*)malloc(40);

        strcpy_s(defaultDialogueBoxPath, directive.length() - 3 /*Including the null character */, directive.substr(5).c_str());
        return;
    }

    if (directive.find("include=") == 1) {
        std::string includedFilePath = directive.substr(9);
        
        PerformPreprocessing(includedFilePath);

        return;
    }
}

/// <summary>
/// Takes a non-command line and tries to parse it's information. This can be ignoring a comment for instance, 
/// or also setting some preprocessor information such as the font
/// </summary>
/// <returns> If the line should disrupt the scannig of any current function </returns>
bool ParseNonCommand(std::string line) {
    if (line.find("//") != std::string::npos) {
        return false;
    }

    if (line.at(0) == '#') {
        HandleDirective(line);

        return false; 
    }

    return true; // The line is effectively gibberish and should signal the end of a function block
}

#define IsNotCommand(line) (line.at(0) != ':')

std::vector<std::string> scriptsLoaded;

bool ScriptAlreadyLoaded(std::string scriptFileName) {
    for (std::string potentialMatch : scriptsLoaded) {
        if (potentialMatch == scriptFileName) {
            return true;
        }
    }

    return false;
}

std::vector<Function*> allFoundFunctions;

std::vector<Function*> GetLoadedFunctions() {
    return allFoundFunctions;
}

/// <summary>
/// Performs the preprocessing stage of the compilation process, starting from a base file, (typically your main file) and loading all the necesarry details
/// </summary>
/// <returns> An Array of function pointers that contain the function data needed at runtime </returns>
void PerformPreprocessing(std::string scriptFilename) {
    if (ScriptAlreadyLoaded(scriptFilename)) {
        return;
    }
    
    scriptsLoaded.push_back(scriptFilename);

    std::fstream* stream = new std::fstream;
    stream->open(MergePaths(SCRIPTS_FOLDER, scriptFilename.c_str()), std::ios::in);

    std::string currentLine;

    bool searchForNextFunction = true;

    while (std::getline(*stream, currentLine)) /* Returns 0 (false) if not finished */ {
        if (currentLine.empty()) {
            searchForNextFunction = true;

            continue;
        }

        if (IsNotCommand(currentLine)) {
            if (ParseNonCommand(currentLine)) {
                searchForNextFunction = true;
            }

            continue;
        }

        if (!searchForNextFunction) {
            allFoundFunctions.back()->commands.push_back(currentLine);

            continue;
        }

        if (currentLine.find(":Func:") != std::string::npos) {
            Function* functionData = ReadInitalFunctionData(currentLine);
            allFoundFunctions.push_back(functionData);

            searchForNextFunction = false;

            continue; // Function should not include its own definition
        }
    }

    stream->close();
    delete stream;
}

// TO-DO make this write to the system global variables that can be saved
void WriteData() {
    std::fstream* stream = new std::fstream;
    std::string toWrite;

    stream->open("SaveData.txt", std::ios::trunc);

    // Again not dealing with this as of now

    stream->close();
    delete stream;
}
