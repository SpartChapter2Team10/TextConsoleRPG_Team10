#include "../../include/Manager/ShopManager.h"
#include "../../include/Manager/PrintManager.h"
#include "../../include/Item/HealPotion.h"
#include "../../include/Item/AttackUp.h"

void ShopManager::ReopenShop()
{
    // 기존 판매 물품 정리
    for (IItem* it : _SellList)
    {
        delete it;
    }
    _SellList.clear();
    
    _SellList.push_back(new HealPotion());
    _SellList.push_back(new AttackUp());
}

void ShopManager::PrintShop()
{
    // Implementation needed
}

bool ShopManager::BuyItem(Player* p, int idx)
{
    // Implementation needed
    return false;
}

int ShopManager::SellItem(Player* p, int slotIdx)
{
    // Implementation needed
    return 0;
}
