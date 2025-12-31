#include "../../include/Item/Inventory.h"
#include "../../include/Item/ItemSlot.h"
#include "../../include/Item/IItem.h"

//빈 슬롯 인덱스 찾기
int Inventory::FindEmptySlotIndex() const {
    for (int i = 0; i < _Slots.size(); i++) {
        if (_Slots[i].IsEmpty()) {
            return i;
        }
    }
    return -1; //빈 슬롯 없음
}

//특정 아이템이 든 슬롯 인덱스 찾기
int Inventory::FindItemSlotIndex(IItem* item) const {

    if (item == nullptr) {
        return -1; //유효하지 않은 아이템 포인터
    }

    for (int i = 0; i < _Slots.size(); i++) {
        if (_Slots[i].GetItem() == item) {
            return i; //아이템이 든 슬롯 인덱스 반환
        }
    }

    return -1; //아이템이 든 슬롯 없음
}


//특정 아이템의 총 개수 반환
int Inventory::GetItemAmount(IItem* item) const {
    int index = FindItemSlotIndex(item);
    if (index == -1) {
        return 0; //아이템 없음
    }
    return _Slots[index].GetAmount();
}

void Inventory::UseItem(int slotIndex, Player& p) {

}

//아이템 인벤토리에 추가
bool Inventory::AddItem(IItem* item, int amount) {
    if (item == nullptr) {
        return false; //유효하지 않은 아이템 포인터
    }

    int index = FindItemSlotIndex(item);
    int currentAmount = GetItemAmount(item);

    //(1) 최대 보유 가능 개수를 초과한 경우
    if (item->GetMaxCount() < currentAmount + 1) {
        return false;
    }

    if (index != -1) {
        _Slots[index].AddAmount(1);
    }

    //(2) 이미 슬롯에 아이템이 있는 경우
    //(3) 아이템이 슬롯에 없는 경우

}

//아이템 인벤토리에서 제거
void Inventory::RemoveItem(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= _Slots.size()) {
        return; //유효하지 않은 슬롯 인덱스
    }

    _Slots[slotIndex].ClearItem();
}