#pragma once
#include <string>

#pragma once
#include "Renderable.h"

struct IntLLNode {
    int value;
    IntLLNode* next;
};

enum class CommandType;

class Command {
public:
    virtual void ProcessCommandInfo(char* info) = NULL;

    virtual void RunCommand() = NULL;

    virtual bool IsFulfilled() = NULL;

    virtual ~Command();
};

class EmptyCMD : public Command {
public:
    void ProcessCommandInfo(char* info);

    void RunCommand();

    bool IsFulfilled();

    ~EmptyCMD();
};

class FunctionCall : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~FunctionCall() override;
private:
    char* functionName;
};

class UpdateText : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~UpdateText() override;
private:
    char* textField;
};

class ChangeBGPortrait : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~ChangeBGPortrait() override;
private:

    char* imageFilename;
};

class ChangePortrait : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~ChangePortrait() override;
private:

    char* imageFileName;
};

class PlaySFX : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~PlaySFX() override;
private:
    char* fileName;
};

class LoadMusic : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~LoadMusic() override;
private:
    char* fileName;
};

struct LLCharArrElement {
    char* element; // Created with new
    LLCharArrElement* next;

    // Gets the array at the index specified, looping back over to the first if necessarry
    inline LLCharArrElement* GetElementForward(int fwdAmount);

    ~LLCharArrElement();
};

struct ChoiceData {
public:
    LLCharArrElement* firstOption; // Expected to be declared using new and NOT malloc
    int totalOptions;

    int* choiceButtons;

    bool choiceSelected;

    ChoiceData();

    ChoiceData(LLCharArrElement* _firstOption, int _totalOptions);

    ~ChoiceData();
};

class ChoiceCMD : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~ChoiceCMD() override;
private:
    ChoiceData* choiceData;
};

class WaitSFXFinish : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~WaitSFXFinish() override;
};

class SetFontSize : public Command {
public:
    void ProcessCommandInfo(char* info) override;

    void RunCommand() override;

    bool IsFulfilled() override;

    ~SetFontSize() override;
private:
    int fontSize;
};