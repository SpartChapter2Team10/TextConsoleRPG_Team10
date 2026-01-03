#pragma once
#include "ICharacter.h"
#include "../Item/Inventory.h"
#include <string>
#include <memory>

class Player : public ICharacter
{
private:
    int _CurrentExp;
    int _MaxExp;
    int _Gold;
    
    // 인벤토리 (메인 플레이어만 활성화)
    // nullptr인 경우 인벤토리 비활성화 (동료 캐릭터)
    std::unique_ptr<Inventory> _Inventory;
  
    // 임시 버프 관리
    int _TempAtkBonus;
    int _BuffRoundsRemaining;

public:
    // 생성자
    // enableInventory: true = 인벤토리 활성화 (메인 플레이어)
    //       false = 인벤토리 비활성화 (동료 캐릭터)
    Player(const std::string& Name, bool enableInventory = false);
    
    void TakeDamage(const int Amount) override;
    void Attack(ICharacter* Target) const override;
    bool IsDead() const override;
    
    void CheckLevelUp();
    void ProcessLevelUp();
    void GainExp(const int Amount);
    void GainGold(const int Amount);
    
    // 아이템 사용 (안전)
    // 반환: 성공 시 true, 실패 시 false (인벤토리 없음 포함)
    bool UseItem(const int SlotIndex);

    inline int GetGold() const { return _Gold; }
    inline int GetMaxGold() const { return _MaxExp; }
    inline int GetExp() const { return _CurrentExp; }
    inline int GetMaxExp() const { return _MaxExp; }

    // 인벤토리 접근 (안전)
    // outInventory: 인벤토리 포인터 저장 (출력 매개변수)
    // 반환: 인벤토리가 있으면 true + outInventory에 포인터 저장
    //       없으면 false + outInventory는 nullptr
    bool TryGetInventory(Inventory*& outInventory);

    // 범용 스탯 수정 메서드 - 아이템이나 버프/디버프 등에서 사용
    void ModifyHP(int Amount);   // HP 증감 (최대치 제한)
    void ModifyMaxHP(int Amount);   // 최대 HP 증감
    void ModifyAtk(int Amount);  // 공격력 증감
    void ModifyGold(int Amount);     // 골드 증감
    
    // 버프 관리 메서드
    void ApplyTempAtkBuff(int Amount, int Rounds);
    void ProcessRoundEnd();
    void ResetBuffs();
    int GetTotalAtk() const;

    // 플레이어 전용 공격 연출
    std::string GetAttackNarration() const override;
};
