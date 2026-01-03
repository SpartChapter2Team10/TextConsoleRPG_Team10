# InputBridge 및 비동기 입력 FAQ

## 질문 1: 비동기 입력이 필요한가?

### 답변: 상황에 따라 다릅니다

#### ✅ 동기식 입력으로 충분한 경우 (현재 InputBridge)
```
턴제 RPG의 일반적인 흐름:

1. 화면 표시 (플레이어 정보, 적 정보)
2. 플레이어 입력 대기 ← 이때 화면이 정지해도 OK
3. 입력 처리 (공격, 스킬 등)
4. 결과 표시 (애니메이션 재생)
5. 적 턴
6. 반복...
```

**이런 게임은 동기식으로 충분:**
- 전통적인 턴제 RPG (포켓몬, 파이널 판타지 스타일)
- 입력 대기 중 화면 변화가 없는 게임
- 간단한 메뉴 기반 게임

#### ❌ 비동기 입력이 필요한 경우
```
실시간 요소가 있는 게임:

┌─────────────────────────────────────┐
│  전투 중... (계속 애니메이션)      │ ← 멈추지 않음
│  타이머: 05초    │ ← 계속 감소
├─────────────────────────────────────┤
│  > 명령 입력: _            │ ← 동시에 입력 대기
└─────────────────────────────────────┘
```

**이런 게임은 비동기 필요:**
- ATB(Active Time Battle) 시스템
- 제한 시간이 있는 입력
- 입력 대기 중에도 배경 애니메이션 재생
- 실시간 전략 게임

---

## 질문 2: InputBridge가 InputManager의 모든 기능을 사용하는가?

### 답변: ✅ 이제 모든 기능을 지원합니다 (업데이트 완료)

#### Before (이전)
| InputManager 메서드 | InputBridge 지원 |
|---------------------|-----------------|
| `GetInput` | ✅ `RequestInput` |
| `GetIntInput` | ✅ `RequestIntInput` |
| `GetCharInput` | ✅ `RequestCharInput` |
| `GetStringInput` | ❌ **없음** |
| `GetYesNoInput` | ❌ **없음** |

#### After (현재) ⭐ NEW
| InputManager 메서드 | InputBridge 지원 |
|---------------------|-----------------|
| `GetInput` | ✅ `RequestInput` |
| `GetIntInput` | ✅ `RequestIntInput` |
| `GetCharInput` | ✅ `RequestCharInput` |
| `GetStringInput` | ✅ **`RequestStringInput`** |
| `GetYesNoInput` | ✅ **`RequestYesNoInput`** |

### 사용 예시

#### GetStringInput / RequestStringInput
```cpp
// InputManager 직접 사용
std::vector<std::string> jobs = {"전사", "마법사", "궁수"};
std::string job = InputManager::GetInstance()->GetStringInput("직업: ", jobs);

// InputBridge 사용
std::string job = InputBridge::GetInstance()->RequestStringInput("직업: ", jobs);
```

**동작:**
- "전사", "마법사", "궁수" 중 하나만 허용
- 다른 값 입력 시 "유효하지 않은 입력입니다" 메시지 + 재입력

#### GetYesNoInput / RequestYesNoInput
```cpp
// InputManager 직접 사용
bool confirm = InputManager::GetInstance()->GetYesNoInput("계속하시겠습니까? ");

// InputBridge 사용
bool confirm = InputBridge::GetInstance()->RequestYesNoInput("계속하시겠습니까? ");
```

**동작:**
- "yes", "Yes", "YES" → `true`
- "no", "No", "NO" → `false`
- 다른 값 입력 시 "유효하지 않은 입력입니다" 메시지 + 재입력

---

## 질문 3: PrintManager에는 브릿지가 없는 이유?

### 답변: PrintManager는 출력만 담당하므로 브릿지가 불필요합니다

#### PrintManager vs InputManager 차이

| 특성 | PrintManager | InputManager |
|------|--------------|--------------|
| **방향** | 단방향 (프로그램 → 콘솔) | 양방향 (콘솔 ↔ 프로그램) |
| **블로킹** | ❌ 없음 (즉시 반환) | ✅ 있음 (입력 대기) |
| **UI 통합 필요성** | ❌ 낮음 | ✅ 높음 |
| **게임 루프 영향** | ❌ 없음 | ✅ 정지시킴 |

### PrintManager는 브릿지가 필요 없는 이유

#### 1. 출력은 블로킹되지 않음
```cpp
// PrintManager - 즉시 반환
PrintManager::GetInstance()->PrintLogLine("Hello");
// 여기서 게임이 멈추지 않음 ✅

// InputManager - 입력 대기
std::string name = InputManager::GetInstance()->GetInput("이름: ");
// 입력이 완료될 때까지 여기서 멈춤 ❌
```

#### 2. UIDrawer와 PrintManager는 독립적
```cpp
// 레거시 모드
PrintManager::GetInstance()->PrintLogLine("일반 출력");

// UI 모드로 전환
UIDrawer::GetInstance()->Activate();
Panel* panel = drawer->CreatePanel(...);
drawer->Render();

// 다시 레거시 모드
UIDrawer::GetInstance()->Deactivate();
PrintManager::GetInstance()->PrintLogLine("다시 일반 출력");
```

**PrintManager와 UIDrawer는 각자의 영역에서 작동:**
- PrintManager: 레거시 모드 (단순 텍스트 출력)
- UIDrawer: UI 모드 (패널, 렌더러)
- 서로 충돌하지 않음 ✅

#### 3. InputManager는 UI와 통합 필요
```cpp
// InputManager는 콘솔에서 직접 읽음
std::string input = InputManager::GetInstance()->GetInput("입력: ");
// 문제: UI 패널과 충돌 가능 ❌

// InputBridge는 UI 패널에 표시
InputBridge* bridge = InputBridge::GetInstance();
bridge->SetInputPanel(inputPanel);  // UI 패널 설정
std::string input = bridge->RequestInput("입력: ");  // 패널에 표시
// UI와 통합됨 ✅
```

### PrintManager의 역할

PrintManager는 **로깅 시스템**입니다:
```cpp
// 일반 출력
PrintManager::GetInstance()->PrintLog("메시지");

// 경고 출력
PrintManager::GetInstance()->PrintLogLine("경고!", ELogImportance::WARNING);

// 중요 출력
PrintManager::GetInstance()->PrintLogLine("중요!", ELogImportance::DISPLAY);
```

**UIDrawer에서는 TextRenderer가 이 역할을 대체:**
```cpp
auto textRenderer = std::make_unique<TextRenderer>();
textRenderer->AddLine("메시지");
textRenderer->SetTextColor(14);  // 색상 지정
panel->SetContentRenderer(std::move(textRenderer));
```

---

## 🎯 결론 및 권장 사항

### InputBridge 사용 가이드

#### 언제 InputManager를 직접 사용?
```cpp
// 간단한 레거시 모드 입력
InputManager* input = InputManager::GetInstance();
std::string name = input->GetInput("이름: ");
```

**사용 시기:**
- UIDrawer를 사용하지 않는 간단한 화면
- 메뉴, 상점 등 정적인 화면

#### 언제 InputBridge를 사용?
```cpp
// UIDrawer와 통합된 입력
InputBridge* bridge = InputBridge::GetInstance();
bridge->SetInputPanel(inputPanel);
std::string name = bridge->RequestInput("이름: ");
```

**사용 시기:**
- UIDrawer를 사용하는 화면
- 패널과 입력을 통합해야 하는 경우
- 입력 상태를 UI에 표시하고 싶은 경우

### PrintManager vs TextRenderer

#### PrintManager (레거시)
```cpp
PrintManager::GetInstance()->PrintLogLine("전투 시작!");
```

**사용 시기:**
- 간단한 콘솔 출력
- 디버그 로그
- UIDrawer 없이 동작하는 부분

#### TextRenderer (UIDrawer)
```cpp
auto textRenderer = std::make_unique<TextRenderer>();
textRenderer->AddLine("전투 시작!");
textRenderer->SetTextColor(12);
panel->SetContentRenderer(std::move(textRenderer));
```

**사용 시기:**
- UIDrawer 화면
- 패널 기반 UI
- 색상, 정렬 등 고급 기능 필요

### 비동기 입력 구현 여부

#### ✅ 지금 구현해야 하는가?
**대부분의 경우: 아니요**

현재 동기식 InputBridge로 충분:
- 턴제 RPG
- 메뉴 기반 게임
- 입력 대기 중 화면 정지 OK

#### ⚠️ 언제 구현해야 하는가?
**다음 기능이 필요할 때만:**
- 입력 대기 중 애니메이션 재생
- 제한 시간이 있는 입력
- 실시간 UI 업데이트

**그때 `AsyncInput_Design.md`를 참고하여 구현하세요.**

---

## 📚 관련 문서

| 문서 | 내용 |
|------|------|
| `UIDrawer_README.md` | UIDrawer 전체 가이드 |
| `UIDrawer_TestExample_README.md` | 테스트 예제 사용법 |
| `AsyncInput_Design.md` | 비동기 입력 시스템 설계 |
| `include/UI/InputBridge.h` | InputBridge 헤더 |
| `include/Manager/InputManager.h` | InputManager 헤더 |

---

## ✅ 업데이트 내역

### 2024-XX-XX
- ✅ InputBridge에 `RequestStringInput` 추가
- ✅ InputBridge에 `RequestYesNoInput` 추가
- ✅ `AsyncInput_Design.md` 작성 (비동기 입력 설계)
- ✅ README 업데이트 (InputBridge 한계 명시)

---

**작성일**: 2024  
**작성자**: UIDrawer System Team  
**버전**: 1.1
