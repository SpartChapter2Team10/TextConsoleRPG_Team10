#include "../../include/Manager/BattleManager.h"
#include "../../include/Manager/PrintManager.h"
#include "../../include/Unit/NormalMonster.h"
#include "../../include/Item/HealPotion.h"
#include "../../include/Item/AttackUp.h"
#include "../../include/Unit/Player.h"
#include <iostream>
#include <tuple>
#include <memory>

using namespace std;

bool BattleManager::StartAutoBattle(Player* P)
{
    // Implementation needed
    // 현재: 플레이어 선공, 노멀 몬스터로 고정 (추후 몬스터 지정 필요)
    
    //NormalMonster* NM = new NormalMonster(P->GetLevel());
    if (!P) return false;

    auto NM = make_unique<NormalMonster>(P->GetLevel());
    ICharacter* Target = NM.get();

    PrintManager::GetInstance()->PrintLogLine(Target->GetName() + "가 출현했습니다.. ");

    while (true)
    {
        ProcessTurn(P, Target);
        if (Target->IsDead())
        {
            //플레이어 승리
            // 전투 결과에 따른 보상 계산 및 지급, 플레이어는 경험치 및 골드를 획득합니다.
            PrintManager::GetInstance()->PrintLogLine(P->GetName() + "이(가) " + Target->GetName() + "를 쓰러뜨렸습니다!!");
            CalculateReward(P, dynamic_cast<IMonster*>(Target));
            return true;
        }
        ProcessAttack(Target, P);
        if (P->IsDead())
        {
            PrintManager::GetInstance()->PrintLogLine(P->GetName() + "이(가) 패배했습니다...");
            return false;
        }
    }

}

void BattleManager::ProcessTurn(ICharacter* Atk, ICharacter* Def)
{
    // TODO: 전투 턴을 진행함
    //아이템 사용 여부 체크 후 사용

    // 자동 아이템 사용: Atk가 Player일 경우에는 인벤토리 검사
    Player* _Player = dynamic_cast<Player*>(Atk);
    if (_Player)
    {
        // 체력이 1/4이하라면 회복 포션 사용 시도
        if (_Player->GetCurrentHP() <= _Player->GetMaxHP() / 4)
        {
            int Slot = _Player->GetInventory().FindFirstSlotIndexOfType<HealPotion>();
            if (Slot != -1) // 회복 포션 탐색 성공
            {
                PrintManager::GetInstance()->PrintLogLine(_Player->GetName() + "이(가) 위험을 감지하고 회복포션을 사용합니다.");
                _Player->UseItem(Slot);
                return;  // 체력 물약은 사용 후 바로 턴 종료
            }
        }

        // 상대 HP가 내 공격력보다 높다면 공격력 포션 사용 시도
        if (Def->GetCurrentHP() > Atk->GetAtk())
        {
            int Slot = _Player->GetInventory().FindFirstSlotIndexOfType<AttackUp>();
            if (Slot != -1) // 공격력 포션 탐색 성공
            {
                PrintManager::GetInstance()->PrintLogLine(_Player->GetName() + "이(가) 전술적으로 공격력 포션을 사용합니다.");
                _Player->UseItem(Slot);

                // 포션 적용후 바로 공격하거나 턴 끝내는 처리는 알아서
                ProcessAttack(Atk, Def);
                return;
            }
        }
        
    }
    else 
    {
        // Atk가 Player가 아닌 경우(몬스터 등)에는 아이템 사용 없음
        ProcessAttack(Atk, Def);
    }
}

void BattleManager::ProcessAttack(ICharacter* Atk, ICharacter* Def)
{
    // 실제 피해 계산은 Attack 구현체가 담당
    int _BeforeHP = Def->GetCurrentHP();
    Atk->Attack(Def);
    int Damage = _BeforeHP - Def->GetCurrentHP();

    // 연출 출력
    PrintManager::GetInstance()->PrintLogLine(Atk->GetAttackNarration());

    // 피해 로그
    std::string msg = Atk->GetName() + "의 공격 | 피해: " + std::to_string(Damage)
        + " | 남은 체력: " + std::to_string(Def->GetCurrentHP()) + "/" + std::to_string(Def->GetMaxHP());
    PrintManager::GetInstance()->PrintLogLine(msg);
}

void BattleManager::CalculateReward(Player* P, IMonster* M)
{
    // Implementation needed
    // 전투 결과에 따른 보상 계산 및 지급, 플레이어는 경험치 및 골드, 아이템 획득

    if (!P || !M) 
        return;

    auto Reward = M->DropReward(); // tuple<int exp, int gold, IItem* item>
    int Exp = get<0>(Reward);
    int Gold = get<1>(Reward);
    IItem* Item = get<2>(Reward);

    if (Exp > 0)
    {
        P->GainExp(Exp);
        PrintManager::GetInstance()->PrintLogLine(P->GetName() + "은(는) " + std::to_string(Exp) + "의 경험치를 획득했습니다.");
    }

    if (Gold > 0)
    {
        P->GainGold(Gold);
        PrintManager::GetInstance()->PrintLogLine(P->GetName() + "은(는) " + std::to_string(Gold) + " 골드를 획득했습니다.");
    }

    if (Item)
    {
        int Remain;
        // Shop과는 달리 Drop의 경우 RawPointer 소유권을 넘기고 unique_ptr로 감싸서 AddItem 호출
        std::unique_ptr<IItem> DroppedItem(Item);
        if (P->GetInventory().AddItem(std::move(DroppedItem), 1, Remain)) // 1은 루트 개수, Remain은 Inven에 못드간 것 개수
        {
            PrintManager::GetInstance()->PrintLogLine(P->GetName() + "은(는) " + Item->GetName() + "을 보상으로 얻었습니다.");
        }
        else
        {
            PrintManager::GetInstance()->PrintLogLine(P->GetName() + "은(는) 인벤토리가 가득 차 있어 아이템을 얻지 못했습니다.");
        }

    }
}