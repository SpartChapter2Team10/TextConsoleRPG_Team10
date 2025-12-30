#pragma once
#include "../Singleton.h"
#include "../TypingSpeed.h"
#include <string>

using namespace std;

class PrintManager : public Singleton<PrintManager>
{
private:
    TypingSpeed _CurrentSpeed;
    float _SpaceInterval;
    int _LineLimit;

public:
    void PrintLog(string msg);
    void PrintWithTyping(string msg);
    void ChangeTextColor(int colorIdx);
    void SetLineLimit(int limit);
    int GetLineLimit();
};
