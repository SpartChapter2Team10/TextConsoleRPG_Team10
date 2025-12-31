#pragma once
#include <vector>
#include "../Config.h"
#include "ItemSlot.h"

using namespace std;

class IItem;
class Player;

//고정 슬롯 인벤토리 클래스
class Inventory
{
private:
    vector<ItemSlot> _Slots; //아이템 슬롯 목록

    //인벤토리 슬롯에 들어갈 아이템들
    HealPotion _HealPotion; //체력 회복 포션 아이템
    AttackUp _AttackUp; //공격력 증가 포션 아이템
        

public:
    //생성자
    Inventory(int MaxSlots) : _Slots(MaxSlots), _MaxSlots(MaxSlots) {}
    
    int GetItemAmount(IItem* item) const; //특정 아이템의 총 개수 반환
    void UseItem(int slotIndex, Player& p); //아이템 사용
    bool AddItem(IItem* item, int amount); //아이템 인벤토리에 추가
    void RemoveItem(int slotIndex); //아이템 인벤토리에서 제거

    template<typename T>
    int FindFirstSlotIndexOfType() const
    {
        EItemType type = T().GetItemType();  // 정적 호출
        for (size_t i = 0; i < _Slots.size(); ++i) {
            if (_Slots[i].GetItem() && _Slots[i].GetItem()->GetItemType() == type) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
};
