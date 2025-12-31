#include "../../include/Manager/BattleManager.h"
#include "../../include/Unit/NormalMonster.h"
#include "../../include/Unit/Player.h"
#include <iostream>
using namespace std;
bool BattleManager::StartAutoBattle(Player* P)
{
    // Implementation needed
    // 현재: 플레이어 선공, 노멀 몬스터로 고정 (추후 몬스터 지정 필요)
    NormalMonster* NM = new NormalMonster(P->GetLevel());

void BattleManager::StartAutoBattle(Player* p)
{
    // Implementation needed
}

void BattleManager::ProcessTurn(ICharacter* Atk, ICharacter* Def)
{
    // Implementation needed
}

void BattleManager::CalculateReward(Player* P)
{
    // Implementation needed
    // 전투 결과에 따른 보상 계산 및 지급, 플레이어는 경험치 및 골드, 아이템 획득
    // 몬스터 드롭 불러오고, 이거는 게임 매니저에서 호출됨
}
