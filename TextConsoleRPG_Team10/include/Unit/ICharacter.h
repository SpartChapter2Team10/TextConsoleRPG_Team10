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
    virtual void TakeDamage(const int Amount) = 0;
    virtual void Attack(ICharacter* Target) const = 0;
    virtual bool IsDead() const = 0;
    virtual ~ICharacter() {}
};
