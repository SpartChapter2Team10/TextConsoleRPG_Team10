#pragma once
#include <string>

using namespace std;

class Player;

class IItem
{
protected:
    string _ItemName;
    int _Price;
    int _EffectAmount;
    int _MaxStack;

public:
    virtual void ApplyEffect(Player& p) = 0;
    virtual int GetMaxStack();
    virtual ~IItem() {}
};
