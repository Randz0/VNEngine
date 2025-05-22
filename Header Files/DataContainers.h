#pragma once
#include <string>

struct IntLLNode {
    int value;
    IntLLNode* next;
};

enum class CommandType;

struct Command {
    CommandType type;
    void* info; // Points to the memory address containing the relevant struct
};

struct LLCharArrElement {
    char* element; // Created with new
    LLCharArrElement* next;

    // Gets the array at the index specified, looping back over to the first if necessarry
    inline LLCharArrElement* GetElementForward(int fwdAmount);

    ~LLCharArrElement();
};

struct Choice {
public:
    LLCharArrElement* firstOption; // Expected to be declared using new and NOT malloc
    int totalOptions;

    int dialoguePath;

    Choice();

    Choice(LLCharArrElement* _firstOption, int _totalOptions);

    ~Choice();
};
