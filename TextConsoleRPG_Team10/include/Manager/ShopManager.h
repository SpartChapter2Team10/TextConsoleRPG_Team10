#pragma once
#include "../Singleton.h"
#include <vector>

using namespace std;

class IItem;
class Player;

class ShopManager : public Singleton<ShopManager>
{
private:
    vector<IItem*> _SellList;
    vector<IItem*> _ResellList;

public:
    void PrintShop();
    bool BuyItem(Player* p, int idx);
    int SellItem(Player* p, int slotIdx);
};
