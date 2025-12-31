#pragma once
#include "../Singleton.h"
#include <vector>

using namespace std;

class IItem;
class Player;

class ShopManager : public Singleton<ShopManager>
{
    friend class Singleton<ShopManager>;
protected:
    ShopManager() {};

private:
    vector<IItem*> _SellList;
    
private:
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
