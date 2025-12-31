#include <iostream>
#include "../../include/Manager/InputManager.h"
#include "../../include/Manager/PrintManager.h"

using namespace std;

string InputManager::GetInput(const string& Prompt)
{
    string str = "";
    PrintManager::GetInstance()->PrintLog(Prompt);
    getline(cin, str);

    return str;
}

int InputManager::GetIntInput(const string& Prompt, int Min, int Max)
{
    int input = 0;
    while (true)
    {
        PrintManager::GetInstance()->PrintLog(Prompt);
        cin >> input;

        if (cin.fail())
        {
            //string Msg = "숫자만 입력 가능합니다.\n";
            string Msg = "Only numbers can be entered.\n";
            PrintManager::GetInstance()->PrintLog(Msg, ELogImportance::WARNING);
            ClearInputBuffer();
        }
        else if (input < Min || input > Max)
        {
            //string Msg = "범위를 벗어났습니다.\n";
            string Msg = "Out of range.\n";
            PrintManager::GetInstance()->PrintLog(Msg, ELogImportance::WARNING);
            ClearInputBuffer();
        }
        else
        {
            ClearInputBuffer();
            return input;
        }
    }
}

string InputManager::GetStringInput(const string& Prompt, const vector<string>& ValidOptions)
{
    string str = "";
    while (true)
    {
        PrintManager::GetInstance()->PrintLog(Prompt);
        getline(cin, str);
        for (const string& option : ValidOptions)
        {
            if (str == option)
            {
                return str;
            }
        }
        //string Msg = "유효하지 않은 입력입니다: ";
        string Msg = "Invalid input: ";
        Msg.append(str);
        PrintManager::GetInstance()->PrintLog(Msg, ELogImportance::WARNING);
    }
}

char InputManager::GetCharInput(const string& Prompt, const string& validChars)
{
    char ch = '\0';
    while (true)
    {
        PrintManager::GetInstance()->PrintLog(Prompt);
        cin >> ch;

        if (cin.fail())
        {
            //string Msg = "유효하지 않은 입력입니다.\n";
            string Msg = "Invalid input: ";
            PrintManager::GetInstance()->PrintLog(Msg, ELogImportance::WARNING);
            ClearInputBuffer();
            continue;
        }

        for (const char& option : validChars)
        {
            if (ch == option)
            {
                return ch;
            }
        }
        //string Msg = "유효하지 않은 입력입니다: ";
        string Msg = "Invalid input: ";
        Msg += ch;
        PrintManager::GetInstance()->PrintLog(Msg, ELogImportance::WARNING);
    }
}

bool InputManager::GetYesNoInput(const string& Prompt)
{
    string input = "";
    while (true)
    {
        PrintManager::GetInstance()->PrintLog(Prompt);
        getline(cin, input);

        if (input == "YES" || input == "Yes" || input == "yes")
        {
            return true;
        }
        else if (input == "NO" || input == "No" || input == "no")
        {
            return false;
        }
        else
        {
            //string Msg = "유효하지 않은 입력입니다: ";
            string Msg = "Invalid input: ";
            Msg.append(input);
            PrintManager::GetInstance()->PrintLog(Msg, ELogImportance::WARNING);
        }
    }
}

void InputManager::ClearInputBuffer()
{
    if (cin.fail() || cin.rdbuf()->in_avail() > 0)
    {
        cin.clear();
        // 개행 문자가 나올 때가지 버퍼 비우기
        cin.ignore(10000000, '\n');
    }
}
