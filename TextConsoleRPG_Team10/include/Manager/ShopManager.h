#pragma once
#include "../Singleton.h"
#include <vector>

using namespace std;

class IItem;
class Player;

struct ItemStock {
    IItem* prototype;  // 프로토타입 포인터 (ShopManager 소유)
    int count;         // 재고 개수
};

class ShopManager : public Singleton<ShopManager>
{
private:
    vector<ItemStock> _SellList;  // 판매 목록 (프로토타입 + 개수)
    
protected:
    ShopManager() = default;
    friend class Singleton<ShopManager>;

    ShopManager(const ShopManager&) = delete;
    ShopManager& operator=(const ShopManager&) = delete;

public:
    // 판매 리스트 초기화
    void ReopenShop();

    // 판매 리스트 출력
    void PrintShop();
    bool BuyItem(Player* Player, int Idx);
    int SellItem(Player* Player, int SlotIdx);
};
