#pragma once
#include "IItem.h"

class HealPotion : public IItem
{
public:
    void ApplyEffect(Player& p) override;
};
