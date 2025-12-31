#pragma once
#include "../Singleton.h"
#include <string>
#include <vector>

using namespace std;

class InputManager : public Singleton<InputManager>
{
public:
    string GetInput(const string& Prompt);
    int GetIntInput(const string& Prompt, int Min, int Max);
    string GetStringInput(const string& Prompt, const vector<string>& ValidOptions);
    char GetCharInput(const string& Prompt, const string& validChars);
    bool GetYesNoInput(const string& Prompt);

private:
    void ClearInputBuffer();
};