#pragma once
#include "IMonster.h"
#include "../../include/Item/MonsterSpawnData.h"
#include "../../include/Skill/ISkill.h"
#include <tuple>
#include <memory>
#include <vector>

class Boss : public IMonster
{
private:
    // Boss 전용 스킬 시스템
    std::vector<std::unique_ptr<ISkill>> _Skills;

    // 턴 카운터 (스킬 쿨타임 관리)
    mutable int _TurnCounter;

    // 페이즈 추적
    mutable bool _IsPhase2;  // HP 50% 미만 여부

    std::string _AttackName;  // CSV에서 로드한 기본 공격명

protected:
    // Boss 스킬 초기화
    void InitializeSkills();

    // 페이즈 체크
    void CheckPhase() const;

public:
    Boss(const MonsterSpawnData& Data);
    virtual int TakeDamage(ICharacter* Target, int amount) override;
    std::tuple<std::string, int> Attack(ICharacter* Target) const override;
    bool IsDead() const override;
    std::tuple<int, int, std::unique_ptr<IItem>> DropReward() override;
    std::string GetAttackNarration() const override;

    // 턴 카운터 증가
    void IncrementTurnCounter() const { _TurnCounter++; }

    // 페이즈 2 여부 반환
    bool IsPhase2() const { return _IsPhase2; }
};
