# 아이템 예약 시스템 사용 가이드

## 📋 개요

BattleManager 담당자를 위한 **아이템 예약 시스템** 사용 가이드입니다.  
전투 로직에 집중하고, 아이템 관련 처리는 제공된 인터페이스만 호출하면 됩니다.

---

## 🎯 핵심 개념

### 예약 시스템 흐름

```
1. 전투 시작 → _CurrentRound = 0 초기화
2. 플레이어 아이템 예약 → ReserveItemUse() 호출
3. 매 라운드마다 → ProcessReservedItems() 자동 호출
4. 조건 만족 시 → 자동 사용 + 로그 출력
5. 전투 종료 → 예약 목록 자동 정리
```

---

## 🛠️ 제공된 인터페이스

### 1. **ReserveItemUse** - 아이템 예약 등록

```cpp
// 사용법
bool BattleManager::ReserveItemUse(Player* player, int slotIndex);
```

**파라미터:**
- `player`: 사용자 (메인 플레이어 또는 파티원)
- `slotIndex`: 인벤토리 슬롯 인덱스 (0부터 시작)

**반환값:**
- `true`: 예약 성공
- `false`: 예약 실패 (인벤토리 없음, 슬롯 비어있음, 이미 예약됨 등)

**자동 처리 사항:**
- ✅ 인벤토리 유효성 검증
- ✅ 슬롯 아이템 존재 확인
- ✅ 중복 예약 방지
- ✅ 아이템에 현재 라운드 기록 (`item->Reserve(_CurrentRound)`)
- ✅ 예약 성공/실패 로그 출력

**사용 예시:**
```cpp
// BattleScene에서 호출
if (playerChoice == 2) {  // 아이템 사용 선택
    int slotIndex = input->GetIntInput("슬롯 번호: ", 0, 9);
    
BattleManager* bm = BattleManager::GetInstance();
    if (bm->ReserveItemUse(player, slotIndex)) {
        // 예약 성공 - 자동으로 로그 출력됨
    } else {
        // 예약 실패 - 실패 이유가 자동으로 출력됨
    }
}
```

---

### 2. **CancelItemReservation** - 아이템 예약 취소

```cpp
// 사용법
bool BattleManager::CancelItemReservation(Player* player, int slotIndex);
```

**파라미터:**
- `player`: 사용자
- `slotIndex`: 취소할 슬롯 인덱스

**반환값:**
- `true`: 취소 성공
- `false`: 취소 실패 (예약 없음)

**자동 처리 사항:**
- ✅ 예약 목록에서 제거
- ✅ 아이템 예약 상태 초기화 (`item->CancelReservation()`)
- ✅ 취소 로그 출력

**사용 예시:**
```cpp
// 플레이어가 예약 취소 선택 시
if (playerChoice == 3) {
    int slotIndex = input->GetIntInput("취소할 슬롯: ", 0, 9);
    
    BattleManager* bm = BattleManager::GetInstance();
    bm->CancelItemReservation(player, slotIndex);  // 자동으로 결과 출력
}
```

---

### 3. **ProcessReservedItems** - 예약된 아이템 자동 처리 (핵심!)

```cpp
// 사용법
void BattleManager::ProcessReservedItems();
```

**자동 처리 사항:**
- ✅ 모든 예약된 아이템의 조건 체크 (`item->CanUse(player, _CurrentRound)`)
- ✅ 조건 만족 시 자동 사용 (`item->ApplyEffect(player)`)
- ✅ 사용 후 인벤토리에서 제거 (`inventory->RemoveItem()`)
- ✅ 예약 취소 (`item->CancelReservation()`)
- ✅ 조건 미달 시 예약 유지 (다음 턴 재시도)
- ✅ 사용된 아이템 예약 목록에서 제거
- ✅ 모든 처리 과정 로그 출력

**⚠️ 중요: ProcessBattleTurn에서 호출해야 합니다!**

```cpp
bool BattleManager::ProcessBattleTurn()
{
    // 라운드 시작
    _CurrentRound++;
    
    // ===== 1. 예약된 아이템 먼저 처리 (필수!) =====
    ProcessReservedItems();
 
    // ===== 2. 플레이어 턴 =====
    ProcessTurn(player, monster);
  
  // ===== 3. 몬스터 턴 =====
    ProcessAttack(monster, player);
    
    // ===== 4. 라운드 종료 =====
    player->ProcessRoundEnd();
    
    return !isGameOver;
}
```

---

### 4. **GetActiveReservations** - 예약 목록 조회 (UI용)

```cpp
// 사용법
const std::vector<ItemReservation>& BattleManager::GetActiveReservations() const;
```

**반환값:**
- 현재 활성화된 예약 목록

**사용 예시:**
```cpp
// UI에 예약된 아이템 표시
BattleManager* bm = BattleManager::GetInstance();
const auto& reservations = bm->GetActiveReservations();

for (const auto& res : reservations) {
    Inventory* inv = nullptr;
  if (res.User->TryGetInventory(inv)) {
        IItem* item = inv->GetItemAtSlot(res.SlotIndex);
        if (item) {
 std::cout << "[예약] " << item->GetName() 
        << " (조건: " << item->GetUseConditionDescription() << ")\n";
        }
 }
}
```

---

### 5. **GetCurrentRound** - 현재 라운드 조회

```cpp
// 사용법
int BattleManager::GetCurrentRound() const;
```

**반환값:**
- 현재 라운드 (0부터 시작)

**사용 예시:**
```cpp
int round = bm->GetCurrentRound();
std::cout << "현재 라운드: " << round << "\n";
```

---

## 📊 ItemReservation 구조체

```cpp
struct ItemReservation
{
int SlotIndex;      // 인벤토리 슬롯 인덱스
    Player* User;       // 사용자
    bool IsActive;      // 예약 활성화 여부
};
```

---

## 🎮 실전 사용 예시

### BattleScene에서 사용

```cpp
void BattleScene::HandleInput()
{
    InputManager* input = InputManager::GetInstance();
    BattleManager* bm = BattleManager::GetInstance();
    Player* player = SceneManager::GetInstance()->GetPlayer();
    
    // 플레이어 선택지 표시
    int choice = input->GetIntInput(
  "[1] 공격 [2] 아이템 예약 [3] 예약 취소: ", 1, 3
    );
    
    if (choice == 1) {
        // 공격 (기존 로직)
  // ...
    }
    else if (choice == 2) {
        // 아이템 예약
    Inventory* inventory = nullptr;
        if (!player->TryGetInventory(inventory)) {
      PrintManager::GetInstance()->PrintLogLine("인벤토리가 없습니다.");
     return;
  }
 
      // 인벤토리 표시 (생략)
     
 int slotIndex = input->GetIntInput("예약할 슬롯 번호 (0-9): ", 0, 9);
        
        // ===== 인터페이스 호출 (모든 검증과 로그는 자동!) =====
        bm->ReserveItemUse(player, slotIndex);
    }
    else if (choice == 3) {
        // 예약 취소
 int slotIndex = input->GetIntInput("취소할 슬롯 번호 (0-9): ", 0, 9);
     
        // ===== 인터페이스 호출 (자동으로 결과 출력) =====
        bm->CancelItemReservation(player, slotIndex);
    }
}
```

### ProcessBattleTurn 구현 예시

```cpp
bool BattleManager::ProcessBattleTurn()
{
    if (!_IsBattleActive || !_CurrentMonster) {
 return false;
    }
    
    GameManager* gm = GameManager::GetInstance();
    std::shared_ptr<Player> mainPlayer = gm->GetMainPlayer();
    if (!mainPlayer) return false;
    
    // ===== 라운드 시작 =====
    _CurrentRound++;
    PrintManager::GetInstance()->EndLine();
    PrintManager::GetInstance()->PrintLogLine(
     "========== 라운드 " + std::to_string(_CurrentRound) + " ==========",
      ELogImportance::DISPLAY
    );
    
    // ===== 1. 예약된 아이템 처리 (자동!) =====
    ProcessReservedItems();
    
    // ===== 2. 플레이어 턴 =====
    ProcessTurn(mainPlayer.get(), _CurrentMonster.get());
    
    // ===== 3. 몬스터 사망 체크 =====
    if (_CurrentMonster->IsDead()) {
        _Result.Victory = true;
     _Result.IsCompleted = true;
        
        PrintManager::GetInstance()->PrintLogLine(
            mainPlayer->GetName() + "이(가) 승리했습니다!",
      ELogImportance::DISPLAY
        );
        
        return false;  // 전투 종료
    }
    
    // ===== 4. 몬스터 턴 =====
    ProcessAttack(_CurrentMonster.get(), mainPlayer.get());
    
  // ===== 5. 플레이어 사망 체크 =====
    if (mainPlayer->IsDead()) {
        _Result.Victory = false;
        _Result.IsCompleted = true;
        
     PrintManager::GetInstance()->PrintLogLine(
            mainPlayer->GetName() + "이(가) 패배했습니다...",
   ELogImportance::WARNING
        );
        
        return false;  // 전투 종료
}
    
    // ===== 6. 라운드 종료 처리 =====
    const auto& party = gm->GetParty();
    for (const auto& member : party) {
        if (member) {
     member->ProcessRoundEnd();  // 버프 턴 감소
        }
    }
  
 return true;  // 전투 계속
}
```

---

## ✅ 체크리스트

### BattleManager 구현 시 확인사항

- [ ] `_CurrentRound` 변수가 선언되어 있는가? (헤더에 이미 추가됨)
- [ ] `_ItemReservations` 벡터가 선언되어 있는가? (헤더에 이미 추가됨)
- [ ] `StartBattle`에서 `_CurrentRound = 0` 초기화하는가?
- [ ] `ProcessBattleTurn` 시작 시 `_CurrentRound++` 증가하는가?
- [ ] `ProcessReservedItems()`를 **턴 시작 직후** 호출하는가?
- [ ] `EndBattle`에서 `_ItemReservations.clear()` 호출하는가?

### Scene 구현 시 확인사항

- [ ] 아이템 사용 UI에 예약 옵션 추가했는가?
- [ ] `ReserveItemUse()` 호출 후 반환값 확인하는가?
- [ ] 예약 취소 기능을 제공하는가?
- [ ] 현재 예약된 아이템 목록을 표시하는가? (선택사항)

---

## 🎨 로그 출력 예시

### 예약 성공 시
```
체력 회복 포션 사용 예약 완료! (조건: HP 30% 이하)
```

### 조건 미달 시 (매 라운드)
```
=== 예약된 아이템 처리 중 ===
체력 회복 포션 - 조건 미달 (HP 30% 이하) → 대기 중
```

### 조건 만족 시 (자동 사용)
```
=== 예약된 아이템 처리 중 ===
>>> 주인공의 체력 회복 포션 자동 사용! (HP 30% 이하 만족)
```

### 예약 취소 시
```
체력 회복 포션 예약이 취소되었습니다.
```

---

## 🚨 주의사항

### 1. ProcessReservedItems 호출 위치

**❌ 잘못된 예 (턴 종료 후)**
```cpp
ProcessTurn(player, monster);
ProcessAttack(monster, player);
player->ProcessRoundEnd();
ProcessReservedItems();  // 너무 늦음!
```

**✅ 올바른 예 (턴 시작 직후)**
```cpp
_CurrentRound++;
ProcessReservedItems();  // 먼저!
ProcessTurn(player, monster);
ProcessAttack(monster, player);
player->ProcessRoundEnd();
```

### 2. 라운드 초기화

전투 시작 시 반드시 `_CurrentRound = 0`으로 초기화하세요:

```cpp
bool BattleManager::StartBattle(EBattleType type)
{
    _CurrentRound = 0;// 필수!
    _ItemReservations.clear();  // 이전 전투 예약 제거
    // ...
}
```

### 3. 전투 종료 시 정리

```cpp
void BattleManager::EndBattle()
{
    // 모든 예약 취소
    for (auto& res : _ItemReservations) {
        if (res.IsActive) {
            Inventory* inv = nullptr;
          if (res.User->TryGetInventory(inv)) {
        IItem* item = inv->GetItemAtSlot(res.SlotIndex);
  if (item) {
    item->CancelReservation();
       }
   }
        }
    }
    
    _ItemReservations.clear();
    _CurrentRound = 0;
    // ...
}
```

---

## 💡 FAQ

### Q1. 아이템 조건은 어떻게 정의하나요?
**A:** 아이템 클래스의 `CanUse` 메서드에 이미 구현되어 있습니다. BattleManager는 신경 쓸 필요 없습니다.

### Q2. 여러 아이템을 동시에 예약할 수 있나요?
**A:** 네! `_ItemReservations` 벡터가 여러 예약을 관리합니다.

### Q3. 한 턴에 여러 아이템이 조건을 만족하면?
**A:** 모두 자동으로 사용됩니다. `ProcessReservedItems`가 순차적으로 처리합니다.

### Q4. 예약된 아이템을 수동으로 사용할 수 있나요?
**A:** 아니요. 예약되면 조건 만족 시까지 대기하거나, 명시적으로 예약 취소해야 합니다.

### Q5. 전투 중 인벤토리에서 아이템이 사라지면?
**A:** `ProcessReservedItems`가 자동으로 감지하고 예약을 취소합니다.

---

## 📞 추가 지원

- **전체 가이드**: `SCENE_IMPLEMENTATION_GUIDE.md` 참고
- **아이템 조건 예시**: 가이드 문서의 "아이템 조건 예시 테이블" 섹션
- **질문**: 아이템 담당자(본인)에게 문의

---

**작성일**: 2025-01-28  
**작성자**: 아이템 시스템 담당  
**버전**: 1.0

**BattleManager 담당자는 이 인터페이스만 호출하면 모든 아이템 로직이 자동으로 처리됩니다!** 🚀
