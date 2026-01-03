# 에레보스 타워: 뒤틀린 성의 종언 - 프로젝트 가이드라인

## 📋 목차
1. [프로젝트 개요](#-프로젝트-개요)
2. [프로젝트 구조](#-프로젝트-구조)
3. [핵심 시스템 이해](#-핵심-시스템-이해)
4. [개발 워크플로우](#-개발-워크플로우)
5. [코딩 컨벤션](#-코딩-컨벤션)
6. [Manager 시스템 활용](#-manager-시스템-활용)
7. [데이터 관리](#-데이터-관리)
8. [디버깅 가이드](#-디버깅-가이드)
9. [참고 문서](#-참고-문서)

---

## 🎮 프로젝트 개요

### 게임 정보
- **장르**: 텍스트 기반 턴제 RPG
- **플랫폼**: Windows Console
- **기술 스택**: C++17, Windows Console API
- **아키텍처**: Scene 기반 게임 시스템

### 게임 플로우
```
메인 메뉴 → 플레이어 이름 입력 → 직업 선택 → 스테이지 선택
→ 전투/상점/동료 영입 → 결과 → 반복 또는 종료
```

### 프로젝트 목표
- ✅ 깔끔한 Scene 기반 구조
- ✅ 파티 시스템 (메인 플레이어 + 동료)
- ✅ 데이터 주도 개발 (CSV 기반)
- ✅ 확장 가능한 아키텍처

---

## 📁 프로젝트 구조

### 디렉토리 구조
```
TextConsoleRPG_Team10/
├── include/        # 헤더 파일
│   ├── Common/     # 공통 정의
│   │   └── TextColor.h     # 색상 Enum
│   ├── Config.h   # 전역 설정
│   ├── Singleton.h            # 싱글톤 템플릿
│   ├── Manager/    # 매니저 클래스
│   │   ├── GameManager.h    # 게임 전체 관리
│ │   ├── SceneManager.h     # Scene 관리
│   │   ├── BattleManager.h    # 전투 관리
│   │   ├── ShopManager.h      # 상점 관리
│   │   ├── DataManager.h  # 데이터 I/O
│   │   ├── InputManager.h     # 입력 관리
│ │   └── PrintManager.h     # 출력 관리
│   ├── UI/          # UI 시스템
│   │   ├── UIDrawer.h  # UI 렌더링 시스템
│   │   ├── UIScene.h          # Scene 베이스 클래스
│   │   ├── Panel.h            # UI 패널
│   │   ├── ScreenBuffer.h     # 화면 버퍼
│   │   ├── IContentRenderer.h # 렌더러 인터페이스
│   │   ├── TextRenderer.h     # 텍스트 렌더러
│   │   ├── StatRenderer.h     # 스탯 렌더러
│   │   ├── AsciiArtRenderer.h # 아스키 아트 렌더러
│   │   └── Scenes/       # Scene 구현체들
│   │   ├── MainMenuScene.h
│   │       ├── PlayerNameInputScene.h
│   │       ├── CharacterSelectScene.h
│   │       ├── StageSelectScene.h
│   │       ├── BattleScene.h
│   │  ├── ShopScene.h
│   │       ├── CompanionRecruitScene.h
│   │       ├── StoryProgressScene.h
│   │       └── ResultScene.h
│   ├── Unit/     # 캐릭터 관련
│   │   ├── ICharacter.h    # 캐릭터 인터페이스
│   │   ├── Player.h           # 플레이어
│   │   ├── IMonster.h         # 몬스터 인터페이스
│   │   ├── NormalMonster.h    # 일반 몬스터
│   │   └── Boss.h        # 보스
│   ├── Item/         # 아이템 관련
│   │   ├── IItem.h            # 아이템 인터페이스
│   │   ├── Inventory.h    # 인벤토리
│   │   ├── ItemSlot.h      # 아이템 슬롯
│   │   ├── ItemData.h         # 아이템 데이터 구조체
│   │   ├── HealPotion.h       # 회복 포션
│   │   └── AttackUp.h      # 공격력 포션
│   └── Factory/       # 팩토리 패턴
│       └── ItemFactory.h      # 아이템 생성 팩토리
├── src/ # 구현 파일 (.cpp)
│   └── (include와 동일한 구조)
├── Resources/     # 게임 리소스
│   ├── Animations/            # 애니메이션 파일
│   ├── Characters/        # 캐릭터 데이터
│   ├── Maps/        # 맵 데이터
│   ├── Monsters/              # 몬스터 데이터
│   │   └── Monsters.csv     # 몬스터 스폰 데이터
│   ├── UI/ # UI 리소스
│   ├── Items/       # 아이템 데이터
│   │   └── Items.csv          # 아이템 정보
│   └── Sound/         # 사운드 (미구현)
├── Main.cpp           # 프로그램 진입점
├── UIDrawer_Flowchart.md      # UI 시스템 흐름도
└── SCENE_IMPLEMENTATION_GUIDE.md  # Scene 구현 가이드
```

---

## 🏗️ 핵심 시스템 이해

### 1. Scene 시스템

#### Scene 생명주기
```
Scene 등록 → ChangeScene → Exit(이전) → Enter(새) 
→ 게임 루프(Update → Render → HandleInput) → Exit
```

#### UIScene 베이스 클래스
```cpp
class UIScene
{
protected:
    UIDrawer* _Drawer;      // UI 렌더링
    InputManager* _Input;   // 입력 관리
    bool _IsActive;      // 활성화 상태
    std::string _SceneName; // Scene 이름

public:
    // 필수 구현
    virtual void Enter() = 0;      // Scene 진입
  virtual void Exit() = 0;       // Scene 종료
    virtual void Update() = 0;     // 매 프레임 업데이트
    virtual void Render() = 0;     // 화면 렌더링
    
    // 선택적 구현
    virtual void HandleInput() {}  // 입력 처리
};
```

### 2. Manager 시스템

#### GameManager - 게임 전체 관리
```cpp
// 파티 관리
GameManager* gm = GameManager::GetInstance();
gm->SetMainPlayer(std::make_shared<Player>("주인공", true));
gm->AddCompanion(std::make_shared<Player>("동료", false));
auto mainPlayer = gm->GetMainPlayer();
const auto& party = gm->GetParty();
size_t aliveCount = gm->GetAliveCount();
gm->RemoveDeadCompanions();
```

#### SceneManager - Scene 전환
```cpp
SceneManager* sm = SceneManager::GetInstance();
sm->RegisterScene(ESceneType::Battle, std::make_unique<BattleScene>());
sm->ChangeScene(ESceneType::Battle);
Player* player = sm->GetPlayer();
```

#### BattleManager - 전투 관리
```cpp
BattleManager* bm = BattleManager::GetInstance();

// 전투 시작
if (bm->StartBattle(EBattleType::Normal)) {
    // 전투 루프
    while (bm->IsBattleActive()) {
 if (!bm->ProcessBattleTurn()) {
         // 전투 종료
         const BattleResult& result = bm->GetBattleResult();
            if (result.Victory) {
        // 승리 처리
            }
        }
    }
    bm->EndBattle();
}
```

#### ShopManager - 상점 관리
```cpp
ShopManager* sm = ShopManager::GetInstance();

// 상점 열기
sm->ReopenShop("Items.csv");

// 상품 목록
std::vector<ShopItemInfo> items = sm->GetShopItems();

// 구매
auto [success, message, goldChange, itemName] = sm->BuyItem(player, index);

// 판매
auto [success, message, goldChange, itemName] = sm->SellItem(player, slotIndex);
```

#### DataManager - 데이터 I/O
```cpp
DataManager* dm = DataManager::GetInstance();

// CSV 로드
auto csv = dm->LoadCSVFile(dm->GetItemsPath(), "Items.csv");
auto items = dm->LoadItemData("Items.csv");
auto monsters = dm->LoadMonsterSpawnData("Monsters.csv");

// 랜덤 몬스터
auto [stage, monsterName] = dm->GetRandomStageAndMonster();

// 파일 목록
auto files = dm->GetFilesInDirectory(dm->GetAnimationsPath(), ".txt");
```

#### InputManager - 입력 관리
```cpp
InputManager* input = InputManager::GetInstance();

// 블로킹 입력
std::string name = input->GetInput("이름: ");
int choice = input->GetIntInput("선택 (1-3): ", 1, 3);
char yn = input->GetCharInput("[Y/N]: ", "YNyn");
std::string job = input->GetStringInput("직업: ", {"전사", "마법사", "궁수"});
bool confirm = input->GetYesNoInput("계속하시겠습니까? ");

// 논블로킹 입력
if (input->IsKeyPressed()) {
    int keyCode = input->GetKeyCode();
    if (keyCode == 27) { /* ESC 처리 */ }
}
```

#### PrintManager - 출력 관리
```cpp
PrintManager* pm = PrintManager::GetInstance();

// 일반 출력
pm->PrintLog("메시지");
pm->PrintLogLine("메시지");

// 중요도 지정
pm->PrintLogLine("중요!", ELogImportance::DISPLAY);  // 노란색
pm->PrintLogLine("경고!", ELogImportance::WARNING);  // 빨간색

// 타이핑 효과
pm->SetTypingSpeed(ETypingSpeed::Normal);
pm->PrintWithTypingLine("안녕하세요");

// 색상 변경
pm->ChangeTextColor(ETextColor::LIGHT_GREEN);
pm->PrintLogLine("초록색 텍스트");
```

### 3. UIDrawer 시스템

#### 기본 사용 패턴
```cpp
UIDrawer* drawer = UIDrawer::GetInstance();

// 초기화
drawer->Initialize(106, 65);
drawer->Activate();

// 패널 생성
Panel* panel = drawer->CreatePanel("PanelID", x, y, width, height);
panel->SetBorder(true, ETextColor::LIGHT_YELLOW);

// 렌더러 설정
auto textRenderer = std::make_unique<TextRenderer>();
textRenderer->AddLine("텍스트");
panel->SetContentRenderer(std::move(textRenderer));

// 렌더링
drawer->Render();

// 종료
drawer->Deactivate();
```

#### 렌더러 종류

**TextRenderer - 텍스트 출력**
```cpp
auto text = std::make_unique<TextRenderer>();
text->AddLine("일반 텍스트");
text->AddLineWithColor("색상 텍스트", WORD(ETextColor::LIGHT_GREEN));
text->AddLogLine("중요!", ELogImportance::DISPLAY);
text->EnableAutoWrap(true);
text->SetWrapWidth(50);
```

**StatRenderer - 키-값 쌍 표시**
```cpp
auto stats = std::make_unique<StatRenderer>();
stats->SetStat("이름", player->GetName());
stats->SetStat("HP", std::to_string(player->GetCurrentHP()));
stats->SetKeyColor(WORD(ETextColor::LIGHT_CYAN));
stats->SetValueColor(WORD(ETextColor::LIGHT_YELLOW));
```

**AsciiArtRenderer - ASCII 아트**
```cpp
auto art = std::make_unique<AsciiArtRenderer>();

// 파일 로드
art->LoadFromFile(dm->GetMonstersPath(), "Goblin.txt");
art->SetAlignment(ArtAlignment::CENTER);

// 애니메이션
art->LoadAnimationFromFolder(dm->GetAnimationsPath() + "/Attack", 0.3f);
art->StartAnimation();
```

---

## 🔄 개발 워크플로우

### Scene 구현 워크플로우

#### 1. Scene 클래스 생성
```cpp
// include/UI/Scenes/MyScene.h
#pragma once
#include "../UIScene.h"

class MyScene : public UIScene
{
private:
    // Scene 데이터

public:
    MyScene();
    ~MyScene() override;
    
    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
```

#### 2. Scene 구현
```cpp
// src/UI/Scenes/MyScene.cpp
void MyScene::Enter()
{
    // 1. 화면 초기화
    _Drawer->ClearScreen();
    _Drawer->RemoveAllPanels();
    _Drawer->Activate();
    _IsActive = true;
    
    // 2. UI 구성
    Panel* panel = _Drawer->CreatePanel("Main", 0, 0, 106, 40);
    auto text = std::make_unique<TextRenderer>();
 text->AddLine("Hello, Scene!");
    panel->SetContentRenderer(std::move(text));
    
    // 3. 첫 렌더링
    _Drawer->Render();
}

void MyScene::Update()
{
    if (_IsActive) {
        _Drawer->Update();  // deltaTime 계산 + 렌더링
        HandleInput();
    }
}

void MyScene::HandleInput()
{
    // 입력 처리
}
```

#### 3. GameManager에 등록
```cpp
// src/Manager/GameManager.cpp
#include "../../include/UI/Scenes/MyScene.h"

void GameManager::Initialize()
{
    SceneManager* sm = SceneManager::GetInstance();
    sm->RegisterScene(ESceneType::MyScene, std::make_unique<MyScene>());
}
```

### 데이터 추가 워크플로우

#### 1. CSV 파일 생성
```csv
# Resources/Items/Items.csv
ItemType,Name,Price,EffectAmount,MaxCount,Stock
HealPotion,회복 포션,50,50,10,10
AttackUp,공격력 포션,100,10,5,5
```

#### 2. 데이터 로드
```cpp
DataManager* dm = DataManager::GetInstance();
auto items = dm->LoadItemData("Items.csv");

for (const auto& item : items) {
    std::cout << item.Name << " - " << item.Price << "G" << std::endl;
}
```

---

## 📝 코딩 컨벤션

### 네이밍 규칙

```cpp
// 클래스: PascalCase
class GameManager {};
class UIDrawer {};

// 인터페이스: I + PascalCase
class ICharacter {};
class IItem {};

// 변수: _PascalCase (멤버 변수)
private:
    int _PlayerLevel;
  std::string _PlayerName;

// 로컬 변수: camelCase
int playerHealth;
std::string itemName;

// 상수: UPPER_CASE
const int MAX_PARTY_SIZE = 4;
const char* DEFAULT_RESOURCE_PATH = "Resources/";

// 함수: PascalCase
void ProcessTurn();
int GetPlayerLevel();

// Enum: E + PascalCase
enum class ESceneType { MainMenu, Battle };
enum class ETextColor { RED, GREEN };
```

### 파일 구조
```cpp
// 헤더 파일
#pragma once
#include <필요한 헤더>

class ClassName
{
private:
    // private 멤버

protected:
    // protected 멤버

public:
    // public 멤버
};

// 구현 파일
#include "헤더.h"
#include <추가 헤더>

// 구현
```

### 주석 규칙
```cpp
/// <summary>
/// 함수 설명
/// </summary>
/// <param name="player">플레이어 포인터</param>
/// <returns>성공 시 true</returns>
bool ProcessBattle(Player* player);

// 일반 주석
// TODO: 구현 필요
// FIXME: 버그 수정 필요
```

---

## 🛠️ Manager 시스템 활용

### 싱글톤 사용 패턴
```cpp
// 인스턴스 가져오기
GameManager* gm = GameManager::GetInstance();
SceneManager* sm = SceneManager::GetInstance();
DataManager* dm = DataManager::GetInstance();
```

### Manager 간 협력 예시

#### 전투 시작 (BattleScene)
```cpp
void BattleScene::Enter()
{
    // GameManager에서 플레이어 가져오기
    Player* player = GameManager::GetInstance()->GetMainPlayer().get();
    
    // BattleManager로 전투 시작
    BattleManager* bm = BattleManager::GetInstance();
  if (!bm->StartBattle(EBattleType::Normal)) {
        // 실패 시 이전 씬으로
        SceneManager::GetInstance()->ChangeScene(ESceneType::StageSelect);
        return;
    }
    
    // UIDrawer로 UI 구성
UIDrawer* drawer = UIDrawer::GetInstance();
    // ... UI 설정
}
```

#### 상점 구매 (ShopScene)
```cpp
void ShopScene::HandleInput()
{
InputManager* input = InputManager::GetInstance();
    ShopManager* sm = ShopManager::GetInstance();
    Player* player = SceneManager::GetInstance()->GetPlayer();
    
    int choice = input->GetIntInput("구매할 아이템: ", 0, sm->GetSellListSize() - 1);
    
    auto [success, message, goldChange, itemName] = sm->BuyItem(player, choice);
    
    PrintManager::GetInstance()->PrintLogLine(message);
}
```

---

## 💾 데이터 관리

### CSV 파일 형식

#### Items.csv
```csv
ItemType,Name,Price,EffectAmount,MaxCount,Stock
HealPotion,회복 포션,50,50,10,10
AttackUp,공격력 포션,100,10,5,5
```

#### Monsters.csv
```csv
Stage,MonsterName
1층 - 어두운 복도,고블린
2층 - 거미 둥지,거대 거미
3층 - 해골의 방,스켈레톤
보스층,타락한 기사
```

### 데이터 로드 패턴
```cpp
// CSV → 구조체 배열
std::vector<ItemData> items = dm->LoadItemData("Items.csv");

// CSV → 범용 2D 배열
std::vector<std::vector<std::string>> csv = dm->LoadCSVFile(path, "file.csv");

// 파일 저장
dm->SaveTextFile(path, "save.txt", data);
```

### 리소스 경로 관리
```cpp
DataManager* dm = DataManager::GetInstance();

// 경로 가져오기
std::string animPath = dm->GetAnimationsPath();
std::string monstersPath = dm->GetMonstersPath();
std::string itemsPath = dm->GetItemsPath();

// 또는
std::string path = dm->GetResourcePath("Animations");
```

---

## 🐛 디버깅 가이드

### 일반적인 문제 해결

#### 1. Scene이 표시되지 않음
```cpp
// 체크리스트
- [ ] _Drawer->Activate() 호출했는가?
- [ ] _IsActive = true 설정했는가?
- [ ] SetContentRenderer() 호출했는가?
- [ ] _Drawer->Render() 호출했는가?
```

#### 2. 한글이 깨짐
```cpp
// Main.cpp에서 확인
SetConsoleOutputCP(65001);  // UTF-8 설정
```

#### 3. 파일을 찾을 수 없음
```cpp
// 경로 확인
DataManager* dm = DataManager::GetInstance();
if (!dm->FileExists(dm->GetItemsPath(), "Items.csv")) {
    PrintManager::GetInstance()->PrintLogLine(
        "Items.csv 파일을 찾을 수 없습니다!",
        ELogImportance::WARNING
    );
}
```

#### 4. nullptr 오류
```cpp
// dynamic_cast 후 반드시 확인
TextRenderer* text = dynamic_cast<TextRenderer*>(panel->GetContentRenderer());
if (!text) {
    PrintManager::GetInstance()->PrintLogLine(
        "TextRenderer 캐스팅 실패!",
     ELogImportance::WARNING
    );
    return;
}
```

### 디버그 출력
```cpp
// 디버그 패널 생성
Panel* debugPanel = _Drawer->CreatePanel("Debug", 0, 0, 50, 10);
auto debugText = std::make_unique<TextRenderer>();
debugText->AddLine("Debug: value=" + std::to_string(value));
debugPanel->SetContentRenderer(std::move(debugText));

// 또는 PrintManager 사용
PrintManager::GetInstance()->PrintLogLine(
    "Debug: " + std::to_string(value),
    ELogImportance::WARNING
);
```

---

## 📚 참고 문서

### 필수 읽기
1. **[SCENE_IMPLEMENTATION_GUIDE.md](./SCENE_IMPLEMENTATION_GUIDE.md)**
   - Scene 구현 단계별 가이드
   - Manager 활용법
   - 실전 예제

2. **[UIDrawer_Flowchart.md](./UIDrawer_Flowchart.md)**
   - UI 렌더링 파이프라인
   - 시스템 아키텍처
   - 최적화 기법

### 코드 참고
```
완전 구현된 Scene:
- src/UI/Scenes/MainMenuScene.cpp
- src/UI/Scenes/PlayerNameInputScene.cpp

Manager 구현:
- src/Manager/GameManager.cpp
- src/Manager/BattleManager.cpp
- src/Manager/ShopManager.cpp
```

---

## ✅ 시작하기 체크리스트

### 프로젝트 이해
- [ ] 프로젝트 구조 파악
- [ ] Scene 시스템 이해
- [ ] Manager 시스템 이해
- [ ] UIDrawer 기본 사용법 숙지

### 개발 준비
- [ ] 개발 환경 설정 (Visual Studio 2022)
- [ ] 프로젝트 빌드 성공
- [ ] Main.cpp 실행 확인
- [ ] SCENE_IMPLEMENTATION_GUIDE.md 읽기

### Scene 구현
- [ ] 담당 Scene 선택
- [ ] 헤더 파일 작성
- [ ] 구현 파일 작성
- [ ] GameManager에 등록
- [ ] 테스트 및 디버깅

---

## 🎯 추천 학습 경로

### 1단계: 기초 이해 (1-2일)
- Scene 시스템 구조 이해
- MainMenuScene, PlayerNameInputScene 코드 분석
- UIDrawer 기본 사용법 학습

### 2단계: 간단한 Scene 구현 (2-3일)
- CharacterSelectScene 또는 StageSelectScene 구현
- InputManager, PrintManager 활용
- 기본 UI 구성

### 3단계: 복잡한 Scene 구현 (3-5일)
- BattleScene 또는 ShopScene 구현
- BattleManager, ShopManager 연동
- 동적 UI 업데이트

### 4단계: 통합 및 테스트 (2-3일)
- 모든 Scene 연결
- 데이터 검증
- 버그 수정 및 최적화

---

## 🚀 팀 협업 가이드

### Git 워크플로우
```bash
# 1. 최신 코드 가져오기
git pull origin main

# 2. 브랜치 생성
git checkout -b feature/my-scene

# 3. 작업 후 커밋
git add .
git commit -m "feat: BattleScene 구현"

# 4. 푸시 및 PR
git push origin feature/my-scene
```

### 커밋 메시지 규칙
```
feat: 새로운 기능 추가
fix: 버그 수정
docs: 문서 수정
refactor: 코드 리팩토링
test: 테스트 코드
chore: 빌드/설정 변경
```

### 코드 리뷰 체크리스트
- [ ] 코딩 컨벤션 준수
- [ ] nullptr 체크
- [ ] 메모리 누수 없음
- [ ] 적절한 주석
- [ ] 테스트 완료

---

## 💡 유용한 팁

### 빠른 프로토타이핑
```cpp
// Scene 구현 중 임시 UI
Panel* tempPanel = _Drawer->CreatePanel("Temp", 10, 10, 80, 20);
auto tempText = std::make_unique<TextRenderer>();
tempText->AddLine("TODO: 구현 필요");
tempPanel->SetContentRenderer(std::move(tempText));
```

### 입력 검증 활용
```cpp
// InputManager가 자동으로 검증
int choice = input->GetIntInput("선택 (1-5): ", 1, 5);
// 1~5 외 입력 시 자동 재입력
```

### 색상 활용
```cpp
#include "include/Common/TextColor.h"

// 패널 테두리
panel->SetBorder(true, static_cast<WORD>(ETextColor::LIGHT_YELLOW));

// 텍스트 색상
text->SetTextColor(static_cast<WORD>(ETextColor::LIGHT_GREEN));
```

---

**작성일**: 2025-01-28  
**버전**: 1.0  
**작성자**: Development Team  

**질문이나 문제가 있으면 팀 채팅방에서 문의하세요!** 🚀
