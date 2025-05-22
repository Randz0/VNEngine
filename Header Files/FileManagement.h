#pragma once
#include <fstream>

#pragma once
#include <stdio.h>

#pragma once
#include <string>

#pragma once
#include "System.h"

#pragma once
#include "LanguageFeatures.h"

#pragma once
#include <vector>

/// <summary>
/// Loads the contents of your saved data into the system
/// </summary>
void LoadData();

Function* ReadInitalFunctionData(std::string _lineData);

void ConfigureDefaultDialogueBox();

void LoadDefaultFont();

std::vector<Function*> GetLoadedFunctions();

/// <summary>
/// Loads In the Functions responsible for the program execution
/// </summary>
/// <param name="actToLoad"></param>
/// <param name="amountOfDialouge"></param>
/// <returns> pointer to array of strings representing all of the commands in the function </returns>
void PerformPreprocessing(std::string scriptToLoad);

// TO-DO make this write to the system global variables that can be saved
void WriteData();
