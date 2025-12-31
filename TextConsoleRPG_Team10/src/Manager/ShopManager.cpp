#include "../../include/Manager/ShopManager.h"
#include "../../include/Manager/PrintManager.h"
#include "../../include/Item/HealPotion.h"
#include "../../include/Item/AttackUp.h"
#include "../../include/Unit/Player.h"
#include <memory>

using namespace std;

void ShopManager::ReopenShop()
{
    // 기존 프로토타입 해제
    for (auto& stock : _SellList)
    {
        delete stock.prototype;
    }
    _SellList.clear();

    // 새 프로토타입 생성 및 재고 설정
    _SellList.push_back({ new HealPotion(), 10 });  // HealPotion 10개
    _SellList.push_back({ new AttackUp(), 5 });     // AttackUp 5개
}

void ShopManager::PrintShop()
{
    PrintManager::GetInstance()->PrintLogLine("===== 상점에 오신 것을 환영합니다! =====");
    PrintManager::GetInstance()->PrintLogLine("오늘의 판매 목록을 안내해드릴게요.");

    for (size_t i = 0; i < _SellList.size(); ++i)
    {
        const ItemStock& stock = _SellList[i];
        if (!stock.prototype) continue;

        // 아이템 이름과 가격, 재고를 출력
        std::string itemName = "알 수 없음";
        int price = 0;
        if (auto* item = stock.prototype)
        {
            itemName = item->GetName();
            price = item->GetPrice();
        }

        std::string msg = std::to_string(i) + ". " + itemName +
            " | 가격: " + std::to_string(price) +
            "골드 | 재고: " + std::to_string(stock.count) + "개";
        PrintManager::GetInstance()->PrintLogLine(msg);
    }

    PrintManager::GetInstance()->PrintLogLine("필요한 물건이 있으신가요? 선택해 주세요!");
}

bool ShopManager::BuyItem(Player* Player, int idx)
{
    if (!Player || idx < 0 || idx >= _SellList.size())
    {
        PrintManager::GetInstance()->PrintLogLine("구매할 수 없는 상품입니다.", ELogImportance::WARNING);
        return false;
    }

    ItemStock& stock = _SellList[idx];
    if (stock.count <= 0)
    {
        PrintManager::GetInstance()->PrintLogLine("죄송합니다. 해당 상품은 품절입니다.");
        return false;  // 재고 없음
    }

    IItem* Selected = stock.prototype;
    int Price = Selected ? Selected->GetPrice() : 0;
    std::string ItemName = Selected ? Selected->GetName() : "알 수 없음";


    // 플레이어의 골드 확인
    if (Player->GetGold() < Price)
    {
        PrintManager::GetInstance()->PrintLogLine("소지금이 부족합니다. (" + std::to_string(Price) + "골드 필요)");
        return false;
    }

    // 지정 타입의 포션 생성 추후 csv로 처리함
    // 런타임에 원하는 타입의 포션을 생성하려면
    // 미리 타입을 매핑해놓고 생성하는 Factory Method 디자인 패턴 형태로 구현되어야 합니다.
    // 어차피 필수 기능이니까 이까지만 합니다 전
    std::unique_ptr<IItem> Item;
    if (dynamic_cast<HealPotion*>(Selected))
    {
        Item = std::make_unique<HealPotion>();
    }
    else if (dynamic_cast<AttackUp*>(Selected))
    {
        Item = std::make_unique<AttackUp>();
    }
    else {
        return false;  // 알 수 없는 타입
    }

    // Inventory에 추가
    int remain;
    bool success = Player->GetInventory().AddItem(std::move(Item), 1, remain);
    if (success && remain == 0)
    {
        stock.count--;  // 재고 감소
        Player->GainGold(-Price); // 골드 차감
        PrintManager::GetInstance()->PrintLogLine(_SellList[idx].prototype->GetName() + "을(를) 구매하셨습니다! 남은 골드: " + std::to_string(Player->GetGold()));
        return true;
    }
    else
    {
        PrintManager::GetInstance()->PrintLogLine("인벤토리에 공간이 부족합니다.");
        return false;
    }
    return false;
}

int ShopManager::SellItem(Player* Player, int slotIdx)
{
    // Implementation needed (판매 로직, 골드 추가 등)
    if (!Player) {
        return 0;
    }
    return 0;
}
