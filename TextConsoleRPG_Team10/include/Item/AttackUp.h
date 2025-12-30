#pragma once
#include "IItem.h"

class AttackUp : public IItem
{
public:
    void ApplyEffect(Player& p) override;
};
