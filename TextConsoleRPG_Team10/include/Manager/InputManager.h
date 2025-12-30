#pragma once
#include "../Singleton.h"
#include <string>
#include <vector>

using namespace std;

class InputManager : public Singleton<InputManager>
{
public:
    string GetInput(string prompt);
    int GetIntInput(string prompt, int min, int max);
    string GetStringInput(string prompt, vector<string> validOptions);
    char GetCharInput(string prompt, string validChars);
    bool GetYesNoInput(string prompt);
};
