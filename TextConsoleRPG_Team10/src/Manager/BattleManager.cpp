#include "../../include/Manager/BattleManager.h"
#include "../../include/Manager/PrintManager.h"
#include "../../include/Unit/NormalMonster.h"
#include "../../include/Unit/Boss.h"
#include "../../include/Item/HealPotion.h"
#include "../../include/Item/AttackUp.h"
#include "../../include/Unit/Player.h"
#include "../../include/Manager/GameManager.h"
#include "../../include/Manager/DataManager.h"
#include "../../include/Item/MonsterSpawnData.h"
#include <iostream>
#include <tuple>
#include <memory>
#include <Windows.h>

// ===== Scene 친화적 인터페이스 구현 =====

bool BattleManager::StartBattle(EBattleType type)
{
    // TODO: 팀원 구현 필요
    // 1. 이미 전투 중이면 false 반환
    // 2. GameManager에서 메인 플레이어 가져오기
    // 3. 플레이어 레벨 기반으로 몬스터 생성
    //    - Normal: DataManager::GetRandomStageAndMonster() → NormalMonster 생성
    //    - Boss: Boss 생성
    // 4. _CurrentMonster, _BattleType, _IsBattleActive, _Result 초기화
    // 5. 성공 시 true 반환

    return false;  // TODO: 구현 후 실제 결과 반환
}

void BattleManager::EndBattle()
{
    // TODO: 팀원 구현 필요
    // 1. 전투 중이 아니면 즉시 반환
    // 2. GameManager에서 파티 가져오기
    // 3. 모든 파티원 버프 초기화 (ResetBuffs)
    // 4. 승리 시 CalculateReward 호출
    // 5. GameManager::RemoveDeadCompanions() 호출
    // 6. _CurrentMonster.reset(), _BattleType = None, _IsBattleActive = false
}

bool BattleManager::ProcessBattleTurn()
{
    // TODO: 팀원 구현 필요
    // 1. 전투 중이 아니거나 몬스터가 없으면 false 반환
    // 2. GameManager에서 메인 플레이어 가져오기
    // 3. 플레이어 턴: ProcessTurn(Player, Monster)
    // 4. 몬스터 사망 확인
    // - 사망 시: _Result.Victory = true, IsCompleted = true, 승리 메시지 출력, false 반환
    // 5. 몬스터 턴: ProcessAttack(Monster, Player)
    // 6. 플레이어 사망 확인
    //    - 사망 시: _Result.Victory = false, IsCompleted = true, 패배 메시지 출력, false 반환
    // 7. 라운드 종료: 파티 전체 ProcessRoundEnd()
    // 8. 전투 계속: true 반환

    return false;  // TODO: 구현 후 실제 결과 반환
}

// ===== 내부 로직 (팀원이 구현 예정) =====

void BattleManager::ProcessTurn(ICharacter* Atk, ICharacter* Def)
{
    // TODO: 팀원 구현 필요 (파티 기반 전투, 직업별 우선순위 등)
    // 현재는 레거시 코드 유지 (참고용)

    // 자동 아이템 사용: Atk가 Player일 경우에는 인벤토리 검사
    Player* _Player = dynamic_cast<Player*>(Atk);
    if (_Player)
    {
        if (Def->GetCurrentHP() < _Player->GetTotalAtk())
        {
            ProcessAttack(Atk, Def);
            return;
        }

        // 인벤토리 접근 (bool + 포인터)
        Inventory* inventory = nullptr;
        if (!_Player->TryGetInventory(inventory))
        {
            ProcessAttack(Atk, Def);
            return;
        }

        // 체력이 1/4이하라면 회복 포션 사용 시도
        if (_Player->GetCurrentHP() <= _Player->GetMaxHP() / 4)
        {
            int Slot = inventory->FindFirstSlotIndexOfType<HealPotion>();
            if (Slot != -1) // 회복 포션 탐색 성공
            {
                PrintManager::GetInstance()->PrintLogLine(_Player->GetName() + "이(가) 위험을 감지하고 회복포션을 사용합니다.");
                PrintManager::GetInstance()->PrintLogLine("현재 체력: " + std::to_string(_Player->GetCurrentHP()) + "/" + std::to_string(_Player->GetMaxHP()));
                _Player->UseItem(Slot);
                return;  // 체력 물약은 사용 후 바로 턴 종료
            }
        }

        // 상대 HP가 내 공격력보다 높다면 공격력 포션 사용 시도
        if (Def->GetCurrentHP() > _Player->GetTotalAtk())
        {
            int Slot = inventory->FindFirstSlotIndexOfType<AttackUp>();
            if (Slot != -1) // 공격력 포션 탐색 성공
            {
                PrintManager::GetInstance()->PrintLogLine(_Player->GetName() + "이(가) 전술적으로 공격력 포션을 사용합니다.");
                PrintManager::GetInstance()->PrintLogLine("공격력: " + std::to_string(_Player->GetAtk()));
                _Player->UseItem(Slot);
                ProcessAttack(Atk, Def);
                return;
            }
        }

        // 일반 공격
        ProcessAttack(Atk, Def);
    }
    else
    {
        // Atk가 Player가 아닌 경우(몬스터 등)에는 아이템 사용 없음
        ProcessAttack(Atk, Def);
    }
}

void BattleManager::ProcessAttack(ICharacter* Atk, ICharacter* Def)
{
    // TODO: 팀원 구현 필요 (파티 공격 시스템 등)
    // 현재는 레거시 코드 유지 (참고용)

    // 실제 피해 계산은 Attack 구현체가 담당
    int _BeforeHP = Def->GetCurrentHP();
    Atk->Attack(Def);
    int Damage = _BeforeHP - Def->GetCurrentHP();

    // 연출 출력
    PrintManager::GetInstance()->PrintLogLine(Atk->GetAttackNarration());

    // 버프 정보 포함한 피해 로그
    Player* _Player = dynamic_cast<Player*>(Atk);
    std::string damageInfo = " | 피해: " + std::to_string(Damage);

    // 플레이어가 버프 상태라면 표시
    if (_Player && _Player->GetTotalAtk() > _Player->GetAtk())
    {
        int BuffBonus = _Player->GetTotalAtk() - _Player->GetAtk();
        damageInfo += " (버프 +" + std::to_string(BuffBonus) + ")";
    }

    std::string msg = Atk->GetName() + "의 공격" + damageInfo
        + " | 남은 체력[" + Def->GetName() + "]: " + std::to_string(Def->GetCurrentHP()) + "/" + std::to_string(Def->GetMaxHP());
    PrintManager::GetInstance()->PrintLogLine(msg);
    Sleep(30);
}

void BattleManager::CalculateReward(Player* P, IMonster* M)
{
    // TODO: 팀원 구현 필요
    // 레거시 코드를 참고하여 구현하세요

    if (!P || !M)
        return;

    auto Reward = M->DropReward();
    int Exp = std::get<0>(Reward);
    int Gold = std::get<1>(Reward);
    std::unique_ptr<IItem> DroppedItem = std::move(std::get<2>(Reward));

    // TODO: BattleResult에 저장
    _Result.ExpGained = Exp;
    _Result.GoldGained = Gold;
    if (DroppedItem)
        _Result.ItemName = DroppedItem->GetName();

    // TODO: GameManager에서 메인 플레이어 및 파티 가져오기
    // TODO: 경험치 파티 전체 분배
    // TODO: 골드 메인 플레이어에게만 지급
    // TODO: 아이템 메인 플레이어 인벤토리에 추가

    /* ===== 레거시 코드 (참고용) =====

    GameManager* gm = GameManager::GetInstance();
    std::shared_ptr<Player> mainPlayer = gm->GetMainPlayer();
    const auto& party = gm->GetParty();

    PrintManager::GetInstance()->EndLine();

    // ===== 경험치: 모든 동료가 동일 수치로 획득 =====
    if (Exp > 0)
    {
        if (party.size() > 1)
        {
            PrintManager::GetInstance()->PrintLog("파티원 모두 ");
        }
        else
        {
            PrintManager::GetInstance()->PrintLog(mainPlayer->GetName() + "은(는) ");
        }
        PrintManager::GetInstance()->PrintColorText(std::to_string(Exp), ETextColor::LIGHT_GREEN);
        PrintManager::GetInstance()->PrintLogLine("의 경험치를 획득했습니다.");

        // 파티 전체에 경험치 분배
        for (const auto& member : party)
        {
            if (member)
            {
                member->GainExp(Exp);
                PrintManager::GetInstance()->PrintLogLine(
                    member->GetName() + "의 EXP: " +
                std::to_string(member->GetExp()) + "/" +
                std::to_string(member->GetMaxExp()));
            }
        }
        PrintManager::GetInstance()->EndLine();
    }

    // ===== 골드: 메인 플레이어만 획득 =====
    if (Gold > 0 && mainPlayer)
    {
        PrintManager::GetInstance()->PrintLog(mainPlayer->GetName() + "은(는) ");
        PrintManager::GetInstance()->PrintColorText(std::to_string(Gold), ETextColor::YELLOW);
        PrintManager::GetInstance()->PrintLogLine("G를 획득했습니다.");
        mainPlayer->GainGold(Gold);
        PrintManager::GetInstance()->PrintLog(mainPlayer->GetName() + "의 소지 골드량은 ");
        PrintManager::GetInstance()->PrintColorText(std::to_string(mainPlayer->GetGold()) + " G", ETextColor::YELLOW);
        PrintManager::GetInstance()->PrintLogLine("입니다.");
    }

    // ===== 아이템: 메인 플레이어 인벤토리에만 추가 =====
    if (DroppedItem && mainPlayer)
    {
        std::string ItemName = DroppedItem->GetName();

        // 메인 플레이어 인벤토리 접근
        Inventory* inventory = nullptr;
        if (!mainPlayer->TryGetInventory(inventory))
        {
        PrintManager::GetInstance()->PrintLogLine(
        mainPlayer->GetName() + "은(는) 인벤토리가 없어 아이템을 얻지 못했습니다.",
        ELogImportance::WARNING);
        PrintManager::GetInstance()->PrintLogLine("");
        return;
        }

        int Remain;
        if (inventory->AddItem(std::move(DroppedItem), 1, Remain))
        {
            PrintManager::GetInstance()->PrintLogLine(
                mainPlayer->GetName() + "은(는) " + ItemName + "을 보상으로 얻었습니다.",
                ELogImportance::DISPLAY); 
        }
        else
        {
            PrintManager::GetInstance()->PrintLogLine(
                mainPlayer->GetName() + "은(는) 인벤토리가 가득 차 있어 아이템을 얻지 못했습니다.",
                ELogImportance::WARNING);
        }
        PrintManager::GetInstance()->PrintLogLine("");
    }

    ===== 레거시 코드 끝 ===== */
}

// ===== 레거시 인터페이스 (하위 호환용 - 제거 예정) =====

bool BattleManager::StartAutoBattle(Player* P)
{
    if (!P) return false;

    DataManager* dm = DataManager::GetInstance();
    auto [stage, monsterName] = dm->GetRandomStageAndMonster();
    if (stage.empty() || monsterName.empty()) return false;

    auto NM = std::make_unique<NormalMonster>(P->GetLevel(), stage, monsterName);
    ICharacter* Target = NM.get();
    PrintManager::GetInstance()->PrintLogLine(NM->GetStage());
    PrintManager::GetInstance()->PrintLogLine(Target->GetName() + "이(가) 출현했습니다.. ");

    while (true)
    {
        // === 플레이어 턴 ===
        ProcessTurn(P, Target);
        if (Target->IsDead())
        {
            // 플레이어 승리
            PrintManager::GetInstance()->PrintLogLine(P->GetName() + "이(가) " + Target->GetName() + "를 쓰러뜨렸습니다!!");
            CalculateReward(P, dynamic_cast<IMonster*>(Target));
            P->ResetBuffs();
            return true;
        }

        // === 몬스터 턴 ===
        ProcessAttack(Target, P);
        if (P->IsDead())
        {
            PrintManager::GetInstance()->PrintLogLine(P->GetName() + "이(가) 패배했습니다...");
            P->ResetBuffs();
            return false;
        }

        // === 라운드 종료 ===
        P->ProcessRoundEnd();
    }
}

bool BattleManager::StartBossBattle(Player* P)
{
    if (!P) return false;

    std::unique_ptr<Boss> boss = std::make_unique<Boss>(P->GetLevel());

    ICharacter* Target = boss.get();
    PrintManager::GetInstance()->PrintLogLine("보스 출현");
    PrintManager::GetInstance()->PrintLogLine(boss->GetStage());
    PrintManager::GetInstance()->PrintLogLine(Target->GetName() + "이(가) 출현했습니다.. ");

    while (true)
    {
        // === 플레이어 턴 ===
        ProcessTurn(P, Target);
        if (Target->IsDead())
        {
            // 플레이어 승리
            PrintManager::GetInstance()->PrintLogLine(P->GetName() + "이(가) " + Target->GetName() + "를 쓰러뜨렸습니다!!");
            P->ResetBuffs();
            return true;
        }

        // === 몬스터 턴 ===
        ProcessAttack(Target, P);
        if (P->IsDead())
        {
            PrintManager::GetInstance()->PrintLogLine(P->GetName() + "이(가) 패배했습니다...");
            P->ResetBuffs();
            return false;
        }

        // === 라운드 종료 ===
        P->ProcessRoundEnd();
    }
}