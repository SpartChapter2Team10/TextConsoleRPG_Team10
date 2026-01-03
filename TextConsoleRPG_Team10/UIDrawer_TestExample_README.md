# UIDrawer 테스트 예제 사용 가이드

## 📖 목차
1. [개요](#-개요)
2. [빠른 시작](#-빠른-시작)
3. [테스트 예제 상세](#-테스트-예제-상세)
4. [InputBridge 사용법](#-inputbridge-사용법)
5. [GameScene 제작 가이드](#-gamescene-제작-가이드)
6. [GameManager 통합](#-gamemanager-통합)
7. [참고 자료](#-참고-자료)
8. [문제 해결](#-문제-해결)

---

## 📌 개요

이 파일은 **UIDrawer 시스템의 모든 핵심 기능**을 시연하는 종합 테스트 예제입니다.  
팀원들이 각자 GameScene을 만들 때 참고할 수 있도록 작성되었습니다.

### 현재 상태
- ✅ Main.cpp에서 GameManager 로직이 **비활성화**되어 있습니다
- ✅ UIDrawer 테스트 모드가 **활성화**되어 있습니다
- ✅ 프로그램 실행 시 UIDrawer 테스트 메뉴가 표시됩니다

---

## 🚀 빠른 시작

### 1단계: 프로젝트 빌드 및 실행
```
F5 (디버그 모드) 또는 Ctrl+F5 (디버그 없이 실행)
```

### 2단계: 메인 메뉴 탐색
콘솔 창에 다음과 같은 메뉴가 표시됩니다:
```
========================================
    UIDrawer 종합 테스트 예제
    Comprehensive Test Example
========================================

테스트할 예제를 선택하세요:

[1] 기본 패널 및 텍스트 렌더링
[2] StatRenderer 및 동적 업데이트
[3] 다양한 색상
[4] ASCII Art 렌더링
[5] 복합 레이아웃 (실전 예제)
[6] InputBridge 입력 처리
[ESC] 종료
```

### 3단계: 예제 실행
- **숫자 키(1-6)** : 원하는 테스트 실행
- **ESC** : 메인 메뉴로 복귀 또는 종료

---

## 📚 테스트 예제 상세

### 🎯 예제 1: 기본 패널 및 텍스트 렌더링
**학습 목표**: 패널 생성, 배치, 텍스트 렌더링

**사용 클래스**:
- `UIDrawer` - 메인 매니저
- `Panel` - 패널 생성 및 관리
- `TextRenderer` - 텍스트 출력

**핵심 코드**:
```cpp
// 패널 생성 (ID, X, Y, Width, Height)
Panel* panel = drawer->CreatePanel("MyPanel", 10, 10, 50, 20);
panel->SetBorder(true, 14); // 테두리 + 색상(노란색)

// 텍스트 렌더러 생성
auto textRenderer = std::make_unique<TextRenderer>();
textRenderer->AddLine("Hello, World!");
textRenderer->SetTextColor(14); // 노란색

// 패널에 렌더러 설정
panel->SetContentRenderer(std::move(textRenderer));

// 화면에 출력
drawer->Render();
```

**배울 내용**:
- 패널 좌표 시스템 (X, Y, Width, Height)
- 텍스트 줄 추가 (`AddLine`)
- 색상 설정 (`SetTextColor`)
- 테두리 설정 (`SetBorder`)

---

### 🎯 예제 2: StatRenderer 및 동적 업데이트
**학습 목표**: 스탯 표시, 실시간 데이터 업데이트

**사용 클래스**:
- `StatRenderer` - 키-값 쌍 표시
- `TextRenderer` - 로그 표시

**핵심 코드**:
```cpp
// StatRenderer 생성
auto statRenderer = std::make_unique<StatRenderer>();
statRenderer->SetStat("이름", "플레이어");
statRenderer->SetStat("HP", "100/100");
statRenderer->SetKeyColor(11);   // 하늘색
statRenderer->SetValueColor(14); // 노란색

// 실시간 업데이트
Panel* statsPanel = drawer->GetPanel("PlayerStats");
StatRenderer* stats = dynamic_cast<StatRenderer*>(statsPanel->GetContentRenderer());
if (stats)
{
    stats->SetStat("HP", std::to_string(newHP) + "/100");
    statsPanel->Redraw(); // 재렌더링
}
drawer->Render();
```

**배울 내용**:
- 키-값 쌍 설정 (`SetStat`)
- 색상 분리 (키 색상, 값 색상)
- `dynamic_cast`를 통한 렌더러 접근
- 패널 재렌더링 (`Redraw`)

**인터랙션**:
- **스페이스 바**: 전투 진행 (최대 5턴)
- **ESC**: 메인 메뉴로 복귀

---

### 🎯 예제 3: 다양한 색상
**학습 목표**: 색상 조합, 시각적 구성

**사용 클래스**:
- `Panel`, `TextRenderer`, `StatRenderer`

**색상 코드 참고**:
| 코드 | 색상 | 코드 | 색상 |
|------|------|------|------|
| 0 | BLACK | 8 | DARK_GRAY |
| 1 | BLUE | 9 | LIGHT_BLUE |
| 2 | GREEN | 10 | LIGHT_GREEN |
| 3 | CYAN | 11 | LIGHT_CYAN |
| 4 | RED | 12 | LIGHT_RED |
| 5 | MAGENTA | 13 | LIGHT_MAGENTA |
| 6 | YELLOW | 14 | LIGHT_YELLOW |
| 7 | LIGHT_GRAY | 15 | WHITE |

**배울 내용**:
- 패널 테두리 색상과 텍스트 색상 조합
- 시각적 계층 구조 표현

---

### 🎯 예제 4: ASCII Art 렌더링
**학습 목표**: 파일에서 아트 로드, 표시

**사용 클래스**:
- `AsciiArtRenderer` - 아스키 아트 렌더링
- `DataManager` - 파일 로드

**핵심 코드**:
```cpp
auto artRenderer = std::make_unique<AsciiArtRenderer>();

// 파일에서 로드
std::string monstersPath = DataManager::GetInstance()->GetResourcePath("Monsters");
artRenderer->LoadFromFile(monstersPath, "Goblin.txt");
artRenderer->SetColor(12); // 빨간색

panel->SetContentRenderer(std::move(artRenderer));
```

**배울 내용**:
- 파일 존재 확인 (`FileExists`)
- 아트 파일 로드 (`LoadFromFile`)
- 아트 색상 설정

**참고**: `Resources/Monsters/` 폴더에 .txt 파일이 없으면 안내 메시지가 표시됩니다.

---

### 🎯 예제 5: 복합 레이아웃 (실전 예제)
**학습 목표**: 실제 게임 화면 구성

**화면 구성**:
```
┌─────────────────────────────────────────────────┐
│ 헤더 (스테이지 정보)  │
├────────┬────────────────────────┬────────────────┤
│ 플레이어│     전투 영역          │  몬스터 정보   │
│ 정보│  (몬스터 아트)      │        │
├────────┤        ├────────────────┤
│인벤토리│              │  상태 정보     │
├────────┴────────────────────────┴────────────────┤
│ 전투 로그 (스크롤 가능)            │
├───────────────────────────────────────────────────┤
│ 명령 입력  │
└───────────────────────────────────────────────────┘
```

**배울 내용**:
- 다중 패널 배치 및 조합
- 레이아웃 설계 원칙
- 실전 게임 UI 구성

---

### 🎯 예제 6: InputBridge 입력 처리 ⭐ NEW!
**학습 목표**: UIDrawer와 InputManager 연동

**사용 클래스**:
- `InputBridge` - UIDrawer ↔ InputManager 브릿지
- `InputManager` - 사용자 입력 처리

**핵심 코드**:
```cpp
InputManager* inputMgr = InputManager::GetInstance();

// 1. 문자열 입력
std::string name = inputMgr->GetInput("이름을 입력하세요: ");

// 2. 정수 입력 (범위 지정)
int level = inputMgr->GetIntInput("레벨을 선택하세요 (1-50): ", 1, 50);

// 3. 문자 입력 (유효한 문자 지정)
char choice = inputMgr->GetCharInput("선택: [Y/N] ", "YNyn");
```

**배울 내용**:
- InputManager 메서드 사용법
  - `GetInput()` - 문자열 입력
  - `GetIntInput()` - 정수 입력 (범위 검증)
  - `GetCharInput()` - 문자 입력 (유효성 검증)
- UI 업데이트와 입력 처리 순서
- 입력 로그 표시

**인터랙션**:
이 예제는 순차적으로 세 가지 입력을 받습니다:
1. 캐릭터 이름 (문자열)
2. 레벨 선택 (1-50)
3. 직업 선택 (W/M/A)

---

## 💡 InputBridge 사용법

### InputBridge란?
`InputBridge`는 UIDrawer와 InputManager를 연결하는 브릿지 클래스입니다.  
UI 화면을 유지하면서 사용자 입력을 받을 수 있게 해줍니다.

### 기본 사용 패턴

#### 1. InputManager 직접 사용 (권장)
```cpp
#include "include/Manager/InputManager.h"

InputManager* inputMgr = InputManager::GetInstance();

// 문자열 입력
std::string playerName = inputMgr->GetInput("이름: ");

// 정수 입력 (최소값, 최대값)
int age = inputMgr->GetIntInput("나이 (1-99): ", 1, 99);

// 문자 입력 (유효한 문자들)
char choice = inputMgr->GetCharInput("[Y/N]: ", "YNyn");
```

#### 2. InputBridge 사용 (고급)
```cpp
#include "include/UI/InputBridge.h"

InputBridge* bridge = InputBridge::GetInstance();

// 입력 패널 설정 (선택사항)
Panel* inputPanel = drawer->CreatePanel("Input", 0, 50, 106, 10);
auto inputText = std::make_unique<TextRenderer>();
inputPanel->SetContentRenderer(std::move(inputText));
bridge->SetInputPanel(inputPanel);

// 입력 요청
std::string result = bridge->RequestInput("프롬프트:");
```

### InputManager 메서드 상세

#### GetInput(prompt)
- **용도**: 자유 문자열 입력
- **반환**: `std::string`
- **예시**: 이름, 메시지 입력

#### GetIntInput(prompt, min, max)
- **용도**: 정수 입력 (범위 제한)
- **반환**: `int`
- **검증**: 
  - 숫자가 아닌 입력 → 재입력 요청
  - 범위 초과 → 재입력 요청
- **예시**: 메뉴 선택, 레벨 설정

#### GetCharInput(prompt, validChars)
- **용도**: 단일 문자 입력 (유효성 검증)
- **반환**: `char`
- **검증**:
  - 빈 입력 → 재입력 요청
  - 2글자 이상 → 재입력 요청
  - validChars에 없는 문자 → 재입력 요청
- **예시**: Y/N 선택, 방향 입력(WASD)

#### GetStringInput(prompt, validOptions)
- **용도**: 문자열 입력 (옵션 검증)
- **반환**: `std::string`
- **검증**: validOptions에 있는 문자열만 허용
- **예시**: 직업 선택("전사", "마법사", "궁수")

#### GetYesNoInput(prompt)
- **용도**: Yes/No 입력
- **반환**: `bool`
- **허용**: "yes", "Yes", "YES", "no", "No", "NO"

### 실전 예제: 캐릭터 생성 화면
```cpp
void CreateCharacterScene()
{
    UIDrawer* drawer = UIDrawer::GetInstance();
    InputManager* input = InputManager::GetInstance();
    
    // UI 구성
    Panel* titlePanel = drawer->CreatePanel("Title", 0, 0, 106, 5);
  // ...패널 설정...
    
    drawer->Render();
    
    // 입력 받기
    std::string name = input->GetInput("캐릭터 이름: ");
    
    Panel* logPanel = drawer->GetPanel("Log");
    TextRenderer* log = dynamic_cast<TextRenderer*>(logPanel->GetContentRenderer());
    log->AddLine("이름: " + name);
    logPanel->Redraw();
    drawer->Render();
    
    int level = input->GetIntInput("시작 레벨 (1-10): ", 1, 10);
    log->AddLine("레벨: " + std::to_string(level));
    logPanel->Redraw();
    drawer->Render();
    
    char classChoice = input->GetCharInput("직업 [W]전사 [M]마법사: ", "WMwm");
    log->AddLine("직업: " + std::string(1, classChoice));
    logPanel->Redraw();
    drawer->Render();
}
```

---

## 🛠️ GameScene 제작 가이드

### 단계별 가이드

#### 1단계: UIScene 상속 클래스 생성
```cpp
// MyGameScene.h
#pragma once
#include "include/UI/UIScene.h"

class MyGameScene : public UIScene
{
private:
    // 멤버 변수 (필요한 데이터)
int _PlayerHP;
    int _EnemyHP;
    
public:
    MyGameScene();
    ~MyGameScene() override;
    
    // Scene 생명주기 (필수 구현)
    void Enter() override;   // Scene 진입 시
    void Exit() override;    // Scene 종료 시
    void Update() override;  // 매 프레임
    void Render() override;  // 화면 출력
    
    // 입력 처리 (선택적)
    void HandleInput() override;
};
```

#### 2단계: Enter() 구현 - 패널 구성
```cpp
// MyGameScene.cpp
void MyGameScene::Enter()
{
    // 예제 5 (복합 레이아웃)을 참고
    
    // 타이틀 패널
    Panel* titlePanel = _Drawer->CreatePanel("Title", 0, 0, 106, 5);
    titlePanel->SetBorder(true, 14);
    auto titleText = std::make_unique<TextRenderer>();
    titleText->AddLine("=== My Game Scene ===");
    titleText->SetTextColor(14);
    titlePanel->SetContentRenderer(std::move(titleText));
    
    // 플레이어 스탯 패널
    Panel* playerPanel = _Drawer->CreatePanel("PlayerStats", 0, 5, 30, 15);
    playerPanel->SetBorder(true, 10);
    auto playerStats = std::make_unique<StatRenderer>();
    playerStats->SetStat("HP", std::to_string(_PlayerHP));
    playerStats->SetKeyColor(10);
    playerStats->SetValueColor(14);
    playerPanel->SetContentRenderer(std::move(playerStats));
    
    // 첫 렌더링
    _Drawer->Render();
}
```

#### 3단계: Update() 구현 - 데이터 업데이트
```cpp
void MyGameScene::Update()
{
  // 예제 2 (동적 업데이트)를 참고
    
    // 스탯 업데이트
    Panel* playerPanel = _Drawer->GetPanel("PlayerStats");
    StatRenderer* stats = dynamic_cast<StatRenderer*>(playerPanel->GetContentRenderer());
    if (stats)
    {
        stats->SetStat("HP", std::to_string(_PlayerHP));
        playerPanel->Redraw();
    }
    
    _Drawer->Render();
}
```

#### 4단계: HandleInput() 구현 - 입력 처리
```cpp
void MyGameScene::HandleInput()
{
    // 예제 6 (InputBridge)을 참고
    
    InputManager* input = InputManager::GetInstance();
    
    char choice = input->GetCharInput("행동 [A]공격 [D]방어: ", "ADad");
    
    if (tolower(choice) == 'a')
    {
      // 공격 로직
_EnemyHP -= 10;
    }
    else if (tolower(choice) == 'd')
    {
        // 방어 로직
    }
    
    Update(); // 화면 갱신
}
```

#### 5단계: Exit() 구현 - 정리
```cpp
void MyGameScene::Exit()
{
    // 패널 제거 (필요시)
    _Drawer->RemoveAllPanels();
}
```

### 참고할 예제
- **레이아웃 설계** → 예제 5 (복합 레이아웃)
- **실시간 업데이트** → 예제 2 (동적 업데이트)
- **입력 처리** → 예제 6 (InputBridge)
- **색상 조합** → 예제 3 (다양한 색상)

---

## 🔄 GameManager 통합

작업이 완료되어 GameManager와 통합하려면:

### Main.cpp 수정
```cpp
// 1. UIDrawer 테스트 호출 주석 처리
// RunUIDrawerTest();

// 2. GameManager 호출 주석 해제
GameManager::GetInstance()->StartGame();
```

### 통합 시 주의사항
1. **초기화 순서**: DataManager → UIDrawer → GameManager
2. **Scene 전환**: UIDrawer 모드 활성화/비활성화 관리
3. **메모리 관리**: Scene 전환 시 패널 정리

---

## 📖 참고 자료

### 주요 파일
| 파일 | 설명 |
|------|------|
| `UIDrawer_TestExample.cpp` | 이 테스트 예제 파일 |
| `UIDrawer_README.md` | UIDrawer 시스템 전체 가이드 |
| `include/UI/UIScene.h` | Scene 베이스 클래스 |
| `include/UI/UIDrawer.h` | UIDrawer 메인 매니저 |
| `include/UI/InputBridge.h` | 입력 브릿지 |
| `include/Manager/InputManager.h` | 입력 관리자 |

### 클래스 다이어그램
```
UIDrawer (메인 매니저)
  ├── Panel (패널 관리)
  │     └── IContentRenderer (인터페이스)
  │        ├── TextRenderer (텍스트)
  │           ├── StatRenderer (스탯)
  │     └── AsciiArtRenderer (아트)
  │
  └── ScreenBuffer (화면 버퍼)

InputBridge (입력 브릿지)
  └── InputManager (입력 처리)
```

### 좌표 시스템
```
(0,0) ────────────────────────── (106,0)
  │           │
  │   Panel(10, 5, 30, 20)          │
  │   ┌─────────────────┐           │
  │   │        │           │
  │   │   Content Area  │        │
  │   │         │           │
  │   └─────────────────┘      │
  │    │
(0,65)───────────────────────── (106,65)
```
- **X축**: 0 ~ 106 (좌 → 우)
- **Y축**: 0 ~ 65 (상 → 하)
- **한글**: 자동으로 2칸 차지

---

## ⚠️ 주의사항

### 필수 확인 사항
1. **콘솔 크기**: 106x65로 설정 (변경 가능)
2. **한글 인코딩**: Main.cpp에서 `SetConsoleOutputCP(65001)` 필수
3. **DataManager 초기화**: UIDrawer 사용 전 필수
4. **Activate/Deactivate**: UI 모드 전환 시 반드시 호출
5. **메모리 관리**: `unique_ptr` 사용으로 자동 해제

### 코딩 규칙
- **dynamic_cast 사용 시**: 항상 nullptr 체크
- **패널 ID**: 고유한 문자열 사용
- **입력 처리**: InputManager 메서드의 유효성 검증 활용

---

## 🐛 문제 해결

### Q: 화면이 깜빡입니다
**A**: `Render()` 대신 `Update()`를 사용하세요 (FPS 제한 포함)
```cpp
// ❌ 깜빡임
while (running) {
    drawer->Render();
}

// ✅ 부드러움
while (running) {
    drawer->Update(); // deltaTime 계산 + 30 FPS 제한
}
```

### Q: 패널이 표시되지 않습니다
**A**: 다음을 확인하세요
1. `SetContentRenderer()` 호출 확인
2. `Render()` 또는 `Update()` 호출 확인
3. 패널 좌표가 화면 범위 내인지 확인 (0-106, 0-65)
4. 패널 크기가 0보다 큰지 확인

### Q: 한글이 깨집니다
**A**: Main.cpp 확인
```cpp
int main()
{
    SetConsoleOutputCP(65001); // 이 줄 필수!
 // ...
}
```

### Q: 입력이 받아지지 않습니다
**A**: 
1. InputManager가 초기화되었는지 확인
2. 콘솔 창이 활성화되어 있는지 확인
3. `GetIntInput`은 숫자만, `GetCharInput`은 단일 문자만 허용

### Q: 색상이 제대로 표시되지 않습니다
**A**:
1. 색상 코드 범위 확인 (0-15)
2. Windows 터미널 설정 확인 (일부 터미널은 색상 미지원)

### Q: 빌드 오류가 발생합니다
**A**:
1. `UIDrawer_TestExample.cpp`가 프로젝트에 포함되어 있는지 확인
2. 모든 헤더 파일 경로가 올바른지 확인
3. C++17 사용 확인

---

## 💡 유용한 팁

### 1. 빠른 프로토타이핑
예제 5 (복합 레이아웃)를 복사하여 시작하세요. 대부분의 게임 화면 구조가 포함되어 있습니다.

### 2. 디버깅
```cpp
// 로그 패널을 만들어 디버그 정보 출력
Panel* debugPanel = drawer->CreatePanel("Debug", 0, 0, 50, 10);
auto debugText = std::make_unique<TextRenderer>();
debugText->AddLine("Debug: HP=" + std::to_string(hp));
debugPanel->SetContentRenderer(std::move(debugText));
```

### 3. 입력 유효성 검증
InputManager 메서드를 활용하면 자동으로 유효성 검증이 이루어집니다.
```cpp
// 자동 검증 - 1~10 범위 외 입력 시 재입력 요청
int choice = input->GetIntInput("선택 (1-10): ", 1, 10);
```

### 4. 재사용 가능한 UI 컴포넌트
자주 사용하는 패널 구성을 함수로 만드세요.
```cpp
Panel* CreatePlayerStatsPanel(UIDrawer* drawer, Player* player)
{
    Panel* panel = drawer->CreatePanel("PlayerStats", 0, 0, 30, 15);
    panel->SetBorder(true, 10);
    auto stats = std::make_unique<StatRenderer>();
 stats->SetStat("HP", std::to_string(player->GetHP()));
    stats->SetKeyColor(10);
    stats->SetValueColor(14);
    panel->SetContentRenderer(std::move(stats));
    return panel;
}
```

---

## 📞 추가 도움말

더 자세한 정보는 다음 문서를 참고하세요:
- **UIDrawer 전체 가이드**: `UIDrawer_README.md`
- **InputBridge 상세**: `include/UI/InputBridge.h`
- **InputManager 상세**: `include/Manager/InputManager.h`

---

**작성일**: 2024  
**작성자**: UIDrawer System Team  
**버전**: 2.0 (InputBridge 추가)
