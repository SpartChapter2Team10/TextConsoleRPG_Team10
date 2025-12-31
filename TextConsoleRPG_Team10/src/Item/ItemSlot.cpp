#include "../../include/Item/ItemSlot.h"
#include "../../include/Item/IItem.h"

//아이템 슬롯이 비어있는지 확인
bool ItemSlot::IsEmpty() const {
    if (_Item == nullptr) {
        return true;
    }
    return false;
}

//아이템을 슬롯에 넣음
//실제 추가는 Inventory 클래스에서 처리
bool ItemSlot::SetItem(IItem* Item, int amount) {
    if (_Item != nullptr || Item == nullptr || amount <= 0) {
        return false;
    }

    _Item = Item;
    _Amount = amount;
    return true;
}

//슬롯에 든 아이템 개수 증가
void ItemSlot::AddAmount(int amount) {
    if (amount <= 0) {
        return;
    }
    _Amount += amount;
}

//슬롯에 든 아이템 개수 감소
void ItemSlot::RemoveAmount(int amount) {
    if (amount <= 0) {
        return;
    }
    _Amount -= amount;

    if (_Amount < 0) {
        _Amount = 0;
    }
}

//슬롯에 든 아이템 개수 반환
int ItemSlot::GetAmount() const{
    return _Amount;
}

//슬롯에 있는 아이템 반환
IItem* ItemSlot::GetItem() const {
    return _Item;
}

//아이템 슬롯 초기화
void ItemSlot::ClearItem() {
    _Item = nullptr;
    _Amount = 0;
}