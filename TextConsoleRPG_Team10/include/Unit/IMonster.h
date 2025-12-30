#pragma once
#include "ICharacter.h"

class IMonster : public ICharacter
{
public:
    virtual void DropReward() = 0;
    virtual ~IMonster() {}
};
