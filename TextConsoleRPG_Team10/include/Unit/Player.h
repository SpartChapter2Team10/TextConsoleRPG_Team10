#pragma once
#include "ICharacter.h"
#include "../Item/Inventory.h"
#include <string>

using namespace std;

class Player : public ICharacter
{
private:
    int _CurrentExp;
    int _MaxExp;
    int _Gold;
    Inventory _Inventory;

public:
    Player(const string& Name);
    void TakeDamage(const int Amount) override;
    void Attack(ICharacter* Target) const override;
    bool IsDead() const override;
    
    void CheckLevelUp();
    void ProcessLevelUp();
    void GainExp(const int Amount);
    void GainGold(const int Amount);
    void UseItem(const int SlotIndex);

    inline int GetGold() const { return _Gold; }
    inline int GetExp() const { return _CurrentExp; }

	Inventory& GetInventory() { return _Inventory; }

	void AddAttack(const int Amount);
    void ResetAttack();
    void Heal(const int Amount);

    // 플레이어 전용 공격 연출
    std::string GetAttackNarration() const override;
};
