# 빠른 참조 가이드 (Quick Reference)

## 📌 자주 사용하는 코드 패턴

### Scene 기본 템플릿
```cpp
void MyScene::Enter()
{
    _Drawer->ClearScreen();
    _Drawer->RemoveAllPanels();
    _Drawer->Activate();
    _IsActive = true;
    
    // UI 구성
    Panel* panel = _Drawer->CreatePanel("Main", 0, 0, 106, 40);
    panel->SetBorder(true, static_cast<WORD>(ETextColor::LIGHT_CYAN));
    
    auto text = std::make_unique<TextRenderer>();
    text->AddLine("내용");
    panel->SetContentRenderer(std::move(text));
    
    _Drawer->Render();
}

void MyScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _IsActive = false;
}

void MyScene::Update()
{
    if (_IsActive) {
        _Drawer->Update();
        HandleInput();
    }
}
```

---

## 🎨 UI 렌더러 사용법

### TextRenderer
```cpp
auto text = std::make_unique<TextRenderer>();
text->AddLine("일반 텍스트");
text->AddLineWithColor("색상 텍스트", WORD(ETextColor::LIGHT_GREEN));
text->AddLogLine("중요!", ELogImportance::DISPLAY);
text->SetTextColor(WORD(ETextColor::WHITE));
panel->SetContentRenderer(std::move(text));
```

### StatRenderer
```cpp
auto stats = std::make_unique<StatRenderer>();
stats->SetStat("이름", "플레이어");
stats->SetStat("HP", "100/100");
stats->SetKeyColor(WORD(ETextColor::LIGHT_CYAN));
stats->SetValueColor(WORD(ETextColor::LIGHT_YELLOW));
panel->SetContentRenderer(std::move(stats));
```

### AsciiArtRenderer
```cpp
auto art = std::make_unique<AsciiArtRenderer>();
art->LoadFromFile(DataManager::GetInstance()->GetMonstersPath(), "Goblin.txt");
art->SetAlignment(ArtAlignment::CENTER);
art->SetColor(WORD(ETextColor::LIGHT_RED));
panel->SetContentRenderer(std::move(art));
```

---

## 🎮 입력 처리

### 블로킹 입력
```cpp
InputManager* input = InputManager::GetInstance();

std::string name = input->GetInput("이름: ");
int choice = input->GetIntInput("선택 (1-3): ", 1, 3);
char yn = input->GetCharInput("[Y/N]: ", "YNyn");
std::string job = input->GetStringInput("직업: ", {"전사", "마법사", "궁수"});
bool confirm = input->GetYesNoInput("계속하시겠습니까? ");
```

### 논블로킹 입력
```cpp
if (input->IsKeyPressed()) {
    int keyCode = input->GetKeyCode();
    if (keyCode == 27) { /* ESC */ }
}

if (input->IsKeyDown(32)) { /* SPACE */ }
if (input->IsCharPressed('a')) { /* 'a' 키 */ }
```

---

## 🎯 Manager 사용법

### GameManager
```cpp
GameManager* gm = GameManager::GetInstance();

// 파티 설정
gm->SetMainPlayer(std::make_shared<Player>("주인공", true));
gm->AddCompanion(std::make_shared<Player>("동료", false));

// 파티 조회
auto mainPlayer = gm->GetMainPlayer();
const auto& party = gm->GetParty();
size_t aliveCount = gm->GetAliveCount();

// 파티 관리
gm->RemoveDeadCompanions();
```

### SceneManager
```cpp
SceneManager* sm = SceneManager::GetInstance();

sm->ChangeScene(ESceneType::Battle);
Player* player = sm->GetPlayer();
```

### BattleManager
```cpp
BattleManager* bm = BattleManager::GetInstance();

// 전투 시작
if (bm->StartBattle(EBattleType::Normal)) {
    while (bm->IsBattleActive()) {
        if (!bm->ProcessBattleTurn()) {
    // 전투 종료
            const BattleResult& result = bm->GetBattleResult();
 }
    }
    bm->EndBattle();
}
```

### ShopManager
```cpp
ShopManager* sm = ShopManager::GetInstance();

sm->ReopenShop("Items.csv");
auto items = sm->GetShopItems();

auto [success, message, goldChange, itemName] = sm->BuyItem(player, index);
auto [success, message, goldChange, itemName] = sm->SellItem(player, slotIndex);
```

### DataManager
```cpp
DataManager* dm = DataManager::GetInstance();

auto items = dm->LoadItemData("Items.csv");
auto monsters = dm->LoadMonsterSpawnData("Monsters.csv");
auto [stage, monsterName] = dm->GetRandomStageAndMonster();

std::string path = dm->GetItemsPath();
auto files = dm->GetFilesInDirectory(dm->GetAnimationsPath(), ".txt");
```

### PrintManager
```cpp
PrintManager* pm = PrintManager::GetInstance();

pm->PrintLog("메시지");
pm->PrintLogLine("메시지");
pm->PrintLogLine("중요!", ELogImportance::DISPLAY);
pm->PrintLogLine("경고!", ELogImportance::WARNING);

pm->ChangeTextColor(ETextColor::LIGHT_GREEN);
pm->SetTypingSpeed(ETypingSpeed::Normal);
pm->PrintWithTypingLine("타이핑 효과");
```

---

## 🎨 색상 코드

```cpp
// 사용 방법
panel->SetBorder(true, static_cast<WORD>(ETextColor::LIGHT_YELLOW));
text->SetTextColor(static_cast<WORD>(ETextColor::LIGHT_GREEN));

// 색상 목록
ETextColor::BLACK    // 0 - 검정
ETextColor::BLUE         // 1 - 파랑
ETextColor::GREEN           // 2 - 초록
ETextColor::CYAN       // 3 - 청록
ETextColor::RED     // 4 - 빨강
ETextColor::MAGENTA     // 5 - 자홍
ETextColor::YELLOW      // 6 - 노랑
ETextColor::LIGHT_GRAY      // 7 - 밝은 회색
ETextColor::DARK_GRAY       // 8 - 어두운 회색
ETextColor::LIGHT_BLUE   // 9 - 밝은 파랑
ETextColor::LIGHT_GREEN     // 10 - 밝은 초록
ETextColor::LIGHT_CYAN      // 11 - 밝은 청록
ETextColor::LIGHT_RED       // 12 - 밝은 빨강
ETextColor::LIGHT_MAGENTA   // 13 - 밝은 자홍
ETextColor::LIGHT_YELLOW    // 14 - 밝은 노랑
ETextColor::WHITE           // 15 - 흰색
```

---

## 📐 좌표 시스템

```
(0,0) ──────────────────────── (106,0)
  │
  │   Panel(10, 5, 30, 20)
  │   ┌─────────────────┐
  │ │ │
  │   │  Content Area   │
  │   │      │
  │ └─────────────────┘
  │
(0,65)──────────────────────── (106,65)
```

- X축: 0 ~ 106 (좌 → 우)
- Y축: 0 ~ 65 (상 → 하)
- 한글: 자동으로 2칸 차지

---

## 🐛 자주 발생하는 오류

### Scene이 표시되지 않음
```cpp
// 체크 항목
✓ _Drawer->Activate() 호출
✓ _IsActive = true 설정
✓ SetContentRenderer() 호출
✓ _Drawer->Render() 호출
```

### 한글 깨짐
```cpp
// Main.cpp 확인
SetConsoleOutputCP(65001);  // UTF-8
```

### nullptr 오류
```cpp
// dynamic_cast 후 반드시 nullptr 체크
TextRenderer* text = dynamic_cast<TextRenderer*>(panel->GetContentRenderer());
if (!text) {
 PrintManager::GetInstance()->PrintLogLine("캐스팅 실패!", ELogImportance::WARNING);
    return;
}
```

### 파일 경로 오류
```cpp
DataManager* dm = DataManager::GetInstance();
if (!dm->FileExists(dm->GetItemsPath(), "Items.csv")) {
    PrintManager::GetInstance()->PrintLogLine("파일 없음!", ELogImportance::WARNING);
}
```

---

## 📁 주요 경로

```cpp
DataManager* dm = DataManager::GetInstance();

dm->GetAnimationsPath()   // Resources/Animations/
dm->GetCharactersPath()   // Resources/Characters/
dm->GetMapsPath()         // Resources/Maps/
dm->GetMonstersPath()     // Resources/Monsters/
dm->GetUIPath()   // Resources/UI/
dm->GetItemsPath()      // Resources/Items/
dm->GetSoundPath()        // Resources/Sound/

// 또는
dm->GetResourcePath("Animations")
```

---

## 🔧 유틸리티 함수

### Player 관련
```cpp
Player* player = SceneManager::GetInstance()->GetPlayer();

// 스탯 조회
player->GetName();
player->GetLevel();
player->GetCurrentHP();
player->GetMaxHP();
player->GetAtk();
player->GetGold();
player->GetExp();

// 스탯 수정
player->ModifyHP(50);        // HP +50
player->ModifyGold(-100); // 골드 -100
player->GainExp(100);           // 경험치 +100
player->CheckLevelUp();         // 레벨업 체크

// 인벤토리
Inventory* inventory = nullptr;
if (player->TryGetInventory(inventory)) {
    player->UseItem(slotIndex);
}
```

### Inventory 관련
```cpp
Inventory* inv = nullptr;
if (player->TryGetInventory(inv)) {
  // 아이템 수량
int amount = inv->GetItemAmount(item);
    int slotAmount = inv->GetSlotAmount(slotIndex);
    
    // 아이템 이름
    std::string name = inv->GetSlotItemTypeName(slotIndex);
    
    // 아이템 사용
    inv->UseItem(slotIndex, player);
    
    // 아이템 추가
    int remain;
    inv->AddItem(std::move(item), 1, remain);
    
    // 아이템 제거
    inv->RemoveItem(slotIndex, 1);
}
```

---

## 📊 데이터 구조

### ItemData
```cpp
struct ItemData {
    std::string ItemType;    // "HealPotion"
 std::string Name;    // "회복 포션"
    int Price;           // 50
    int EffectAmount;        // 50
    int MaxCount;            // 10
    int Stock;    // 10
};
```

### MonsterSpawnData
```cpp
struct MonsterSpawnData {
    std::string Stage;       // "1층 - 어두운 복도"
    std::string MonsterName; // "고블린"
};
```

### BattleResult
```cpp
struct BattleResult {
    bool Victory;           // 승리 여부
    bool IsCompleted;  // 전투 종료 여부
    int ExpGained;     // 획득 경험치
    int GoldGained;      // 획득 골드
    std::string ItemName;   // 획득 아이템 이름
};
```

### ShopItemInfo
```cpp
struct ShopItemInfo {
    std::string name;  // 아이템 이름
    int price;   // 가격
    int stock;        // 재고
};
```

---

## 🎯 Scene 전환 흐름

```
MainMenu → PlayerNameInput → CharacterSelect → StageSelect
    ↓        ↓
  종료  ← CompanionRecruit ← Shop ← Battle
           ↓   ↓       ↓
          Result ──────────┴───────┘
```

---

## ⚡ 빠른 디버깅

### 콘솔 출력
```cpp
PrintManager::GetInstance()->PrintLogLine(
    "Debug: value=" + std::to_string(value),
    ELogImportance::WARNING
);
```

### 디버그 패널
```cpp
Panel* debugPanel = _Drawer->CreatePanel("Debug", 0, 0, 50, 10);
auto debugText = std::make_unique<TextRenderer>();
debugText->AddLine("Debug Info:");
debugText->AddLine("Value: " + std::to_string(value));
debugPanel->SetContentRenderer(std::move(debugText));
```

### 변수 출력
```cpp
std::cout << "Debug: " << variableName << std::endl;
```

---

**작성일**: 2025-01-28  
**버전**: 1.0  
