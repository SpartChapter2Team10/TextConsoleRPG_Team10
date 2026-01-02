# UIDrawer 시스템 사용 가이드

## 📖 개요

UIDrawer는 기존 텍스트 기반 출력 시스템과 **완벽하게 호환**되면서, 복잡한 UI 화면을 구성할 수 있는 새로운 렌더링 시스템입니다.

### 주요 특징
- ✅ **기존 코드 수정 불필요**: PrintManager와 독립적으로 동작
- ✅ **패널 기반 레이아웃**: 절대 좌표로 자유롭게 배치
- ✅ **다양한 콘텐츠**: 텍스트, 스탯, ASCII 아트, 애니메이션
- ✅ **UTF-8 한글 지원**: 한글 2칸 처리 자동화
- ✅ **더블 버퍼링**: 깜빡임 없는 부드러운 렌더링

---

## 🚀 빠른 시작

### 1. 초기화
```cpp
#include "include/UI/UIDrawer.h"

UIDrawer* drawer = UIDrawer::GetInstance();
drawer->Initialize(106, 65);  // 콘솔 크기
drawer->Activate();           // UI 모드 활성화
```

### 2. 패널 생성
```cpp
#include "include/UI/Panel.h"

// CreatePanel(ID, X, Y, Width, Height)
Panel* panel = drawer->CreatePanel("MyPanel", 10, 10, 50, 20);
panel->SetBorder(true, 14);  // 노란색 테두리
```

### 3. 콘텐츠 설정
```cpp
#include "include/UI/TextRenderer.h"

auto textRenderer = std::make_unique<TextRenderer>();
textRenderer->AddLine("Hello, World!");
textRenderer->SetTextColor(14);  // 노란색
panel->SetContentRenderer(std::move(textRenderer));
```

### 4. 렌더링
```cpp
drawer->Render();  // 화면에 출력
```

### 5. 종료
```cpp
drawer->Deactivate();  // 레거시 모드로 복귀
```

---

## 📐 좌표 시스템

```
(0,0) ────────────────────────────────────── (106,0)
  ││
  │   Panel1: (10, 5, 30, 10)     │
  │   ┌────────────────┐   │
  │   │     │   │
  │   │  Content Area  │ │
  │   │        │    │
  │   └────────────────┘             │
  │       │
  │           Panel2: (50, 20, 40, 15)│
  │    ┌──────────────────────┐│
  │          │          ││
  │           │  Another Panel    ││
  │  │       ││
  │          └──────────────────────┘│
 (0,65)────────────────────────────────────(106,65)
```

- **X축**: 0 ~ 106 (좌 → 우)
- **Y축**: 0 ~ 65 (상 → 하)
- **한글**: 자동으로 2칸 차지

---

## 🎨 콘텐츠 렌더러 종류

### 1. TextRenderer (텍스트 로그)
```cpp
auto textRenderer = std::make_unique<TextRenderer>();
textRenderer->AddLine("첫 번째 줄");
textRenderer->AddLine("두 번째 줄");
textRenderer->SetTextColor(10);  // 초록색
textRenderer->SetAutoScroll(true);  // 자동 스크롤
```

**사용 사례**: 전투 로그, 시스템 메시지

### 2. StatRenderer (키-값 쌍)
```cpp
auto statRenderer = std::make_unique<StatRenderer>();
statRenderer->SetStat("이름", player->GetName());
statRenderer->SetStat("HP", player->GetCurrentHP());
statRenderer->SetStat("레벨", player->GetLevel());
statRenderer->SetKeyColor(11);   // 하늘색
statRenderer->SetValueColor(14); // 노란색
```

**사용 사례**: 플레이어 정보, 몬스터 스탯

### 3. AsciiArtRenderer (아스키 아트)
```cpp
auto artRenderer = std::make_unique<AsciiArtRenderer>();

// 파일에서 로드
std::string monstersPath = DataManager::GetInstance()->GetResourcePath("Monsters");
artRenderer->LoadFromFile(monstersPath, "Goblin.txt");

// 정렬 설정
artRenderer->SetAlignment(ArtAlignment::CENTER);
artRenderer->SetColor(12);  // 빨간색
```

**사용 사례**: 몬스터 이미지, 타이틀 화면

### 4. AsciiArtRenderer (애니메이션)
```cpp
// 방법 1: 파일명 직접 지정
auto artRenderer = std::make_unique<AsciiArtRenderer>();

std::vector<std::string> frames = {
    "frame_001.txt",
    "frame_002.txt",
    "frame_003.txt"
};

std::string animPath = DataManager::GetInstance()->GetAnimationsPath();
artRenderer->LoadAnimationFromFiles(animPath + "/MonsterAttack", frames, 0.3f);  // 0.3초마다 전환
artRenderer->StartAnimation();

// 방법 2: 폴더 자동 로드 (권장)
auto artRenderer = std::make_unique<AsciiArtRenderer>();

std::string animPath = DataManager::GetInstance()->GetAnimationsPath();
artRenderer->LoadAnimationFromFolder(animPath + "/MonsterAttack", 0.3f);  // 폴더 내 모든 .txt 파일 자동 로드
artRenderer->StartAnimation();
```

**파일 구조 예시:**
```
Resources/
└── Animations/
  └── MonsterAttack/
        ├── frame_001.txt
        ├── frame_002.txt
        ├── frame_003.txt
        └── frame_004.txt
```

**중요**: 파일명은 사전순으로 정렬되므로, **제로 패딩**(001, 002, ...)을 사용하세요.

**사용 사례**: 공격 모션, 스킬 이펙트, 인트로 애니메이션

---

## 🎬 애니메이션 시스템 상세

### DataManager 애니메이션 지원

#### 1. 폴더 내 파일 목록 가져오기
```cpp
// 특정 폴더 내 모든 파일 목록 가져오기 (확장자 필터 가능)
std::vector<std::string> DataManager::GetFilesInDirectory(
    const std::string& folderPath, 
    const std::string& extension = ""
);
```

**예시:**
```cpp
// 모든 파일
std::vector<std::string> allFiles = 
    DataManager::GetInstance()->GetFilesInDirectory("Resources/Animations/Attack");

// .txt 파일만
std::vector<std::string> txtFiles = 
    DataManager::GetInstance()->GetFilesInDirectory("Resources/Animations/Attack", ".txt");
```

#### 2. 애니메이션 폴더 로드
```cpp
// AsciiArtRenderer에서 폴더 기반 애니메이션 로드
bool LoadAnimationFromFolder(
    const std::string& animationFolderPath,
    float frameDuration = 0.5f,
    const std::string& extension = ".txt"
);
```

**장점:**
- ✅ 파일 추가/삭제 시 코드 수정 불필요
- ✅ 자동 정렬 (파일명 순서대로)
- ✅ 확장자 필터링 가능

**예시:**
```cpp
auto anim = std::make_unique<AsciiArtRenderer>();

// 기본 사용 (.txt 파일, 0.5초 간격)
anim->LoadAnimationFromFolder("Resources/Animations/MonsterAttack");

// 커스터마이징
anim->LoadAnimationFromFolder(
    "Resources/Animations/Skill",
    0.2f,    // 0.2초마다 프레임 전환
    ".txt"   // .txt 파일만 로드
);

anim->StartAnimation();
```

### 애니메이션 파일명 규칙

**올바른 예시 (제로 패딩):**
```
frame_001.txt
frame_002.txt
...
frame_010.txt
frame_099.txt
```

**잘못된 예시 (정렬 문제):**
```
frame_1.txt   → frame_1.txt
frame_10.txt  → frame_10.txt
frame_2.txt   → frame_2.txt  (2가 10 뒤로 감)
```

### 애니메이션 제어
```cpp
artRenderer->StartAnimation();            // 재생 시작
artRenderer->StopAnimation();       // 재생 중지
artRenderer->SetFrameDuration(0.2f);   // 프레임 간격 변경 (0.2초)
```

### 애니메이션 흐름
1. **DataManager**가 폴더에서 파일 목록 가져오기
2. **파일명 정렬** (사전순)
3. 각 파일을 **프레임으로 로드** (LoadTextFile)
4. **시간 기반 프레임 전환** (Update)
5. **현재 프레임 렌더링** (Render)

---

## 🔄 동적 업데이트

### 스탯 업데이트 예시
```cpp
// 1. 패널 가져오기
Panel* statsPanel = drawer->GetPanel("PlayerStats");

// 2. 렌더러 가져오기
StatRenderer* statsContent = dynamic_cast<StatRenderer*>(statsPanel->GetContentRenderer());

// 3. 값 변경
if (statsContent) {
  statsContent->SetStat("HP", std::to_string(player->GetCurrentHP()));
}

// 4. 재렌더링
statsPanel->Redraw();  // 해당 패널만
// 또는
drawer->RedrawAll();   // 전체 화면
```

### 로그 추가 예시
```cpp
Panel* logPanel = drawer->GetPanel("BattleLog");
TextRenderer* logContent = dynamic_cast<TextRenderer*>(logPanel->GetContentRenderer());

if (logContent) {
    logContent->AddLine("플레이어의 공격!");
    logContent->AddLine("몬스터에게 50 데미지!");
}

logPanel->Redraw();
drawer->Render();
```

---

## ⚙️ 고급 기능

### 1. 애니메이션 루프
```cpp
while (gameRunning) {
    drawer->Update();  // deltaTime 계산 + 애니메이션 업데이트 + 렌더링
    // 자동으로 30 FPS 유지
}
```

### 2. FPS 변경
```cpp
drawer->SetTargetFPS(60.0f);  // 60 FPS로 변경
```

### 3. 전체 화면 강제 재렌더링
```cpp
drawer->RedrawAll();  // 모든 패널 dirty 플래그 설정 + 렌더링
```

### 4. 패널 제거
```cpp
drawer->RemovePanel("PanelID");    // 특정 패널 제거
drawer->RemoveAllPanels();         // 모든 패널 제거
```

---

## 🎮 실전 예제: 전투 화면

```cpp
void ShowBattleScreen(Player* player, IMonster* monster)
{
UIDrawer* drawer = UIDrawer::GetInstance();
    drawer->Initialize();
    drawer->Activate();

    // 상단 헤더
    Panel* header = drawer->CreatePanel("Header", 0, 0, 106, 3);
    header->SetBorder(true, 11);
    auto headerText = std::make_unique<TextRenderer>();
    headerText->AddLine("[전투] " + monster->GetStage());
    header->SetContentRenderer(std::move(headerText));

    // 플레이어 스탯 (좌측)
    Panel* playerPanel = drawer->CreatePanel("PlayerStats", 0, 3, 30, 15);
    playerPanel->SetBorder(true, 10);
    auto playerStats = std::make_unique<StatRenderer>();
    playerStats->SetStat("이름", player->GetName());
    playerStats->SetStat("HP", std::to_string(player->GetCurrentHP()) + "/" + std::to_string(player->GetMaxHP()));
    playerPanel->SetContentRenderer(std::move(playerStats));

    // 몬스터 아트 (중앙)
    Panel* monsterArt = drawer->CreatePanel("MonsterArt", 30, 3, 46, 30);
    monsterArt->SetBorder(true, 12);
    auto art = std::make_unique<AsciiArtRenderer>();
    art->LoadFromFile(DataManager::GetInstance()->GetResourcePath("Monsters"), 
             monster->GetName() + ".txt");
    art->SetAlignment(ArtAlignment::CENTER);
    monsterArt->SetContentRenderer(std::move(art));

    // 몬스터 스탯 (우측)
    Panel* monsterPanel = drawer->CreatePanel("MonsterStats", 76, 3, 30, 15);
    monsterPanel->SetBorder(true, 12);
    auto monsterStats = std::make_unique<StatRenderer>();
    monsterStats->SetStat("이름", monster->GetName());
    monsterStats->SetStat("HP", std::to_string(monster->GetCurrentHP()) + "/" + std::to_string(monster->GetMaxHP()));
    monsterPanel->SetContentRenderer(std::move(monsterStats));

    // 전투 로그 (하단)
    Panel* logPanel = drawer->CreatePanel("BattleLog", 0, 40, 106, 25);
    logPanel->SetBorder(true, 7);
    auto log = std::make_unique<TextRenderer>();
    log->AddLine("전투 시작!");
    logPanel->SetContentRenderer(std::move(log));

    // 첫 렌더링
drawer->Render();

    // 전투 루프
    while (!player->IsDead() && !monster->IsDead()) {
        // 공격 로직...
        
        // 스탯 업데이트
      StatRenderer* pStats = dynamic_cast<StatRenderer*>(playerPanel->GetContentRenderer());
        pStats->SetStat("HP", std::to_string(player->GetCurrentHP()) + "/" + std::to_string(player->GetMaxHP()));
 
        // 로그 추가
 TextRenderer* logContent = dynamic_cast<TextRenderer*>(logPanel->GetContentRenderer());
        logContent->AddLine("플레이어의 공격!");
        
    // 화면 갱신
     drawer->Update();
      Sleep(1000);
    }

    drawer->Deactivate();
}
```

---

## 🔧 레거시 코드와 병행 사용

### 방법 1: 화면별 분리
```cpp
// 상점 화면 (레거시)
void ShowShop() {
    PrintManager::GetInstance()->PrintLogLine("상점에 오신 것을 환영합니다!");
    // 기존 코드...
}

// 전투 화면 (UIDrawer)
void ShowBattle() {
    UIDrawer::GetInstance()->Activate();
    // UI Drawer 코드...
    UIDrawer::GetInstance()->Deactivate();
}
```

### 방법 2: 모드 전환
```cpp
// 레거시 모드
PrintManager::GetInstance()->PrintLogLine("일반 출력");

// UI Drawer 모드로 전환
UIDrawer::GetInstance()->Activate();
// ... UI 구성 ...
UIDrawer::GetInstance()->Deactivate();

// 다시 레거시 모드
PrintManager::GetInstance()->PrintLogLine("다시 일반 출력");
```

---

## 📁 파일 구조

```
include/UI/
├── UIDrawer.h              # 메인 매니저
├── Panel.h       # 패널 클래스
├── ScreenBuffer.h   # 화면 버퍼
├── IContentRenderer.h      # 렌더러 인터페이스
├── TextRenderer.h        # 텍스트 렌더러
├── StatRenderer.h    # 스탯 렌더러
└── AsciiArtRenderer.h      # 아스키 아트 렌더러

src/UI/
├── UIDrawer.cpp
├── Panel.cpp
├── ScreenBuffer.cpp
├── TextRenderer.cpp
├── StatRenderer.cpp
└── AsciiArtRenderer.cpp

Example_UIDrawer.cpp    # 사용 예제
```

---

## 🎨 색상 코드 (Windows Console)

| 값 | 색상 | 값 | 색상 |
|----|------|----|------|
| 0  | BLACK | 8  | DARK_GRAY |
| 1  | BLUE | 9  | LIGHT_BLUE |
| 2  | GREEN | 10 | LIGHT_GREEN |
| 3  | CYAN | 11 | LIGHT_CYAN |
| 4  | RED | 12 | LIGHT_RED |
| 5  | MAGENTA | 13 | LIGHT_MAGENTA |
| 6  | YELLOW | 14 | LIGHT_YELLOW |
| 7  | LIGHT_GRAY | 15 | WHITE |

---

## ⚠️ 주의사항

1. **초기화 필수**: `UIDrawer::Initialize()` 호출 후 사용
2. **Activate 후 사용**: UI 모드 활성화 필수
3. **Deactivate 필수**: 종료 시 반드시 호출 (커서 복원)
4. **한글 처리**: 자동으로 2칸 처리, 패널 경계 고려
5. **동적 캐스팅**: 렌더러 접근 시 `dynamic_cast` 사용
6. **메모리 관리**: `unique_ptr` 사용으로 자동 해제

---

## 🐛 문제 해결

### Q: 화면이 깜빡입니다.
A: `drawer->Update()` 대신 `drawer->Render()`만 사용하면 FPS 제한이 없어 깜빡일 수 있습니다. `Update()`를 사용하세요.

### Q: 한글이 깨집니다.
A: `Main.cpp`에서 `SetConsoleOutputCP(65001)` 확인하세요.

### Q: 패널이 표시되지 않습니다.
A: `SetContentRenderer()` 호출 확인 + `Render()` 호출 확인

### Q: 애니메이션이 작동하지 않습니다.
A: `StartAnimation()` 호출 + `drawer->Update()` 사용 (Render() 대신)

---

## 📞 지원

문제가 발생하면 `Example_UIDrawer.cpp`의 예제를 참고하세요.
각 예제는 독립적으로 작동하도록 설계되었습니다.
