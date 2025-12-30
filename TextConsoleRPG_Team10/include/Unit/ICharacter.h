#pragma once
#include <string>

using namespace std;

class ICharacter
{
protected:
    string _Name;
    int _Level;
    int _CurrentHP;
    int _MaxHP;
    int _Atk;

public:
    virtual void TakeDamage(int amount) = 0;
    virtual void Attack(ICharacter* target) = 0;
    virtual bool IsDead() = 0;
    virtual ~ICharacter() {}
};
