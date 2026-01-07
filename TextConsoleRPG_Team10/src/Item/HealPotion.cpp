#include "../../include/Item/HealPotion.h"
#include "../../include/Unit/Player.h"

void HealPotion::ApplyEffect(Player& P)
{
    P.ModifyHP(_EffectAmount);
}

std::unique_ptr<IItem> HealPotion::Clone() const
{
    // 새로운 HealPotion 인스턴스 생성하여 반환
    // 각 슬롯이 독립적인 아이템 객체를 소유하도록 함
    return std::make_unique<HealPotion>();
}

bool HealPotion::CanUse(const Player& player, int currentRound) const
{
    // 예약되지 않았으면 사용 불가
    if (!IsReserved()) {
        return false;
    }
    
    // HP 30% 이하일 때만 사용 가능 (턴 무관)
    return player.GetCurrentHP() <= player.GetMaxHP() * 0.3;
}

std::string HealPotion::GetUseConditionDescription() const
{
    return "HP 30% 이하";
}
