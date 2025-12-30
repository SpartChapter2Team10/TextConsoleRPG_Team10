#pragma once
#include <vector>

using namespace std;

class IItem;
class ItemSlot;
class Player;

class Inventory
{
private:
    vector<ItemSlot*> _Slots;
    int _MaxSlots;

public:
  void UseItem(int slotIndex, Player& p);
    bool AddItem(IItem* item, int quantity);
    void RemoveItem(int slotIndex, int quantity);
    int FindItemSlot(IItem* item);
    int GetEmptySlot();
};
