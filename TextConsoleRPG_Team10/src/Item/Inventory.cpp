#include "../../include/Item/Inventory.h"
#include "../../include/Item/ItemSlot.h"
#include "../../include/Item/IItem.h"
#include "../../include/Config.h"
#include <string>
#include <typeinfo>

using namespace std;

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
int Inventory::FindItemSlotIndex(IItem* item) const 
{
    if (item == nullptr) 
    {
        return -1; //유효하지 않은 아이템 포인터
    }

    for (int i = 0; i < _Slots.size(); i++) 
    {
        IItem* slotItem = _Slots[i].GetItem();
        if (slotItem && typeid(*slotItem) == typeid(*item)) {
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

// 특정 슬롯의 아이템 개수 반환
int Inventory::GetSlotAmount(int SlotIndex) const
{
    if (SlotIndex < 0 || SlotIndex >= _Slots.size())
        return -1; // 유효하지 않은 인덱스

    return _Slots[SlotIndex].GetAmount();
}

std::string Inventory::GetSlotItemTypeName(int SlotIndex) const
{
    if (SlotIndex < 0 || SlotIndex >= _Slots.size())
        return "ERR_INDEX"; // 유효하지 않은 인덱스

    const ItemSlot& slot = _Slots[SlotIndex];
    IItem* item = slot.GetItem();

    if (!item)
        return "ERR_NULL"; // 빈 슬롯

    return typeid(*item).name(); // 컴파일러별로 가독성이 다를 수 있음
}

// 아이템 인벤토리에 추가
// return: 모두 추가 성공 시 true, 일부만 추가되면 false
// remain: 인벤토리에 다 못 넣은 남은 개수(ref out)
bool Inventory::AddItem(std::unique_ptr<IItem> Item, int Amount, int& Remain) {
    Remain = Amount;
    if (!Item || Amount <= 0)
        return false; // 유효하지 않은 입력

    int MaxStack = Item->GetMaxCount();

    // 1. 동일 타입(같은 typeid) 슬롯에 포개기 (여러 슬롯에 분산 가능)
    for (auto& slot : _Slots) {
        IItem* slotItem = slot.GetItem();
        if (slotItem && typeid(*slotItem) == typeid(*Item)) {
            int canAdd = MaxStack - slot.GetAmount();
            if (canAdd > 0) {
                int toAdd = std::min(canAdd, Remain); // 해당 슬롯에는 넣을 수 있을 만큼만 추가됨
                slot.AddAmount(toAdd);
                Remain -= toAdd; // 남은것 빼기
                if (Remain == 0)
                    return true; // 모두 추가됨
            }
        }
    }

    // 2. 빈 슬롯에 새로 추가 (여러 슬롯에 분산 가능)
    for (auto& slot : _Slots) {
        if (slot.IsEmpty()) {
            int toAdd = std::min(MaxStack, Remain);
            slot.SetItem(std::move(Item), toAdd);  // 소유권 이동
            Remain -= toAdd;
            if (Remain == 0)
                return true; // 모두 추가됨
        }
    }

    // 3. 인벤토리에 다 못 넣은 경우 false, 남은 개수는 remain에 저장
    return false;
}

// 아이템 사용
// return: 사용 성공 시 true, 실패 시 false
bool Inventory::UseItem(int SlotIndex, Player& P) {
    // 1. 인덱스 유효성 검사
    if (SlotIndex < 0 || SlotIndex >= _Slots.size())
        return false; // 유효하지 않은 슬롯

    ItemSlot& slot = _Slots[SlotIndex];

    // 2. 슬롯이 비어있는지 확인
    if (slot.IsEmpty())
        return false; // 빈 슬롯

    IItem* item = slot.GetItem();

    // 3. 아이템이 실제로 존재하는지 확인
    if (!item)
        return false; // 아이템 없음

    // 4. 아이템 효과 적용 및 개수 감소
    item->ApplyEffect(P);
    slot.RemoveAmount(1);

    return true; // 정상적으로 사용됨
}

// 해당 아이템 슬롯의 아이템을 ItemCount 만큼 제거
bool Inventory::RemoveItem(int SlotIndex, int ItemCount)
{
    if (SlotIndex < 0 || SlotIndex >= _Slots.size() || ItemCount <= 0)
        return false; // 유효하지 않은 인덱스 또는 개수

    ItemSlot& Slot = _Slots[SlotIndex];

    if (Slot.IsEmpty())
        return false; // 빈 슬롯

    int CurrentAmount = Slot.GetAmount();
    if (CurrentAmount < ItemCount)
        return false; // 제거할 개수가 현재 개수보다 많음

    Slot.RemoveAmount(ItemCount); // 아이템 개수 감소
    return true;
}

bool Inventory::RemoveItemAtSlot(int SlotIndex)
{
    if (SlotIndex < 0 || SlotIndex >= _Slots.size()) {
        return false; // 유효하지 않은 슬롯
    }

    ItemSlot& Slot = _Slots[SlotIndex];

    if (Slot.IsEmpty()) {
        return false; // 빈 슬롯
    }

    Slot.ClearItem(); // 슬롯 전체 비우기
    return true;
}
