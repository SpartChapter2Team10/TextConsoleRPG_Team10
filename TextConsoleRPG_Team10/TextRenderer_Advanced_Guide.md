# TextRenderer 고급 기능 가이드

## 📌 개요

TextRenderer가 **PrintManager의 모든 기능을 지원**하도록 대폭 확장되었습니다!  
이제 UIDrawer 모드에서도 PrintManager의 고급 기능을 모두 사용할 수 있습니다.

---

## 🆚 PrintManager vs TextRenderer 기능 비교

### Before (이전)

| 기능 | PrintManager | TextRenderer | 상태 |
|------|--------------|--------------|------|
| 기본 출력 | ✅ | ✅ | 지원 |
| 색상 변경 | ✅ | ⚠️ 전체만 | 부분 지원 |
| 타이핑 효과 | ✅ | ❌ | **누락** |
| Log 중요도 | ✅ | ❌ | **누락** |
| 자동 줄바꿈 | ✅ | ❌ | **누락** |

### After (현재) ⭐ NEW!

| 기능 | PrintManager | TextRenderer | 상태 |
|------|--------------|--------------|------|
| 기본 출력 | ✅ `PrintLog` | ✅ `AddLine` | ✅ 완벽 지원 |
| 색상 변경 | ✅ `ChangeTextColor` | ✅ `SetTextColor` | ✅ 완벽 지원 |
| **줄별 색상** | ❌ 없음 | ✅ `AddLineWithColor` | ⭐ **더 강력** |
| **타이핑 효과** | ✅ `PrintWithTyping` | ✅ `AddLineWithTyping` | ✅ 완벽 지원 |
| **Log 중요도** | ✅ `ELogImportance` | ✅ `AddLogLine` | ✅ 완벽 지원 |
| **자동 줄바꿈** | ✅ `LineLimit` | ✅ `EnableAutoWrap` | ✅ 완벽 지원 |
| **타이핑 속도** | ✅ `SetTypingSpeed` | ✅ `SetTypingSpeed` | ✅ 완벽 지원 |

---

## 🎯 새로운 기능 상세

### 1. Log 중요도 표시 (`AddLogLine`)

PrintManager의 `ELogImportance`를 그대로 지원합니다.

#### 사용 예시
```cpp
auto textRenderer = std::make_unique<TextRenderer>();

// 일반 로그
textRenderer->AddLogLine("일반 메시지", ELogImportance::NONE);
// 출력: "일반 메시지" (기본 색상)

// DISPLAY (중요 정보)
textRenderer->AddLogLine("중요한 정보!", ELogImportance::DISPLAY);
// 출력: "[DISPLAY] 중요한 정보!" (노란색)

// WARNING (경고)
textRenderer->AddLogLine("경고! 위험합니다!", ELogImportance::WARNING);
// 출력: "[WARNING] 경고! 위험합니다!" (빨간색)
```

#### PrintManager와 비교
```cpp
// PrintManager (레거시 모드)
PrintManager::GetInstance()->PrintLogLine("경고!", ELogImportance::WARNING);

// TextRenderer (UIDrawer 모드)
textRenderer->AddLogLine("경고!", ELogImportance::WARNING);

// ✅ 동일한 결과!
```

---

### 2. 줄별 색상 지정 (`AddLineWithColor`)

**PrintManager보다 더 강력합니다!** 각 줄마다 다른 색상을 지정할 수 있습니다.

#### 사용 예시
```cpp
auto textRenderer = std::make_unique<TextRenderer>();

textRenderer->AddLineWithColor("빨간색 경고", static_cast<WORD>(ETextColor::LIGHT_RED));
textRenderer->AddLineWithColor("초록색 성공", static_cast<WORD>(ETextColor::LIGHT_GREEN));
textRenderer->AddLineWithColor("파란색 정보", static_cast<WORD>(ETextColor::LIGHT_BLUE));
```

#### PrintManager의 한계
```cpp
// PrintManager는 전체 색상만 변경 가능
PrintManager::GetInstance()->ChangeTextColor(ETextColor::RED);
PrintManager::GetInstance()->PrintLogLine("빨간색");
PrintManager::GetInstance()->ChangeTextColor(ETextColor::GREEN);
PrintManager::GetInstance()->PrintLogLine("초록색");
// → 매번 색상 변경 필요 ❌

// TextRenderer는 한 번에 설정
textRenderer->AddLineWithColor("빨간색", WORD(ETextColor::RED));
textRenderer->AddLineWithColor("초록색", WORD(ETextColor::GREEN));
// → 간편하고 직관적 ✅
```

---

### 3. 타이핑 효과 (`AddLineWithTyping`)

PrintManager의 `PrintWithTyping`을 UIDrawer에서 사용할 수 있습니다!

#### 설정 및 사용
```cpp
auto textRenderer = std::make_unique<TextRenderer>();

// 1. 타이핑 효과 활성화
textRenderer->EnableTypingEffect(true);

// 2. 타이핑 속도 설정
textRenderer->SetTypingSpeed(ETypingSpeed::Normal);
// Slow (100ms), Normal (50ms), Fast (20ms)

// 3. 타이핑될 텍스트 추가
textRenderer->AddLineWithTyping("안녕하세요!", 14);  // 노란색
textRenderer->AddLineWithTyping("타이핑 효과 데모입니다.", 11);  // 하늘색

// 4. 패널 설정
panel->SetContentRenderer(std::move(textRenderer));

// 5. 게임 루프에서 업데이트
while (running)
{
    drawer->Update();  // ✅ 타이핑 효과 자동 재생!
    Sleep(16);  // ~60 FPS
}
```

#### 작동 원리
```
시간축 →
──────────────────────────────────────────────
텍스트: "안녕하세요!"

0.0s: ""
0.05s: "안"
0.1s: "안녕"
0.15s: "안녕하"
0.2s: "안녕하세"
0.25s: "안녕하세요"
0.3s: "안녕하세요!"
──────────────────────────────────────────────
```

#### PrintManager와 비교
```cpp
// PrintManager (블로킹)
PrintManager::GetInstance()->PrintWithTyping("안녕하세요!");
// ❌ 타이핑이 완료될 때까지 프로그램 정지

// TextRenderer (비블로킹)
textRenderer->AddLineWithTyping("안녕하세요!");
while (running) {
    drawer->Update();  // ✅ 타이핑 중에도 게임 계속 실행
}
```

---

### 4. 자동 줄바꿈 (`EnableAutoWrap`)

PrintManager의 `LineLimit` 기능을 지원합니다.

#### 사용 예시
```cpp
auto textRenderer = std::make_unique<TextRenderer>();

// 1. 자동 줄바꿈 활성화
textRenderer->EnableAutoWrap(true);

// 2. 줄바꿈 기준 너비 설정 (글자 수)
textRenderer->SetWrapWidth(50);  // 50글자마다 줄바꿈

// 3. 긴 텍스트 추가
textRenderer->AddLine("이것은 아주 긴 텍스트입니다. 자동 줄바꿈 기능을 사용하면 지정된 너비를 초과하는 텍스트가 자동으로 다음 줄로 넘어갑니다!");

// 출력:
// "이것은 아주 긴 텍스트입니다. 자동 줄바꿈 기능을 사용하면 지정된"
// "너비를 초과하는 텍스트가 자동으로 다음 줄로 넘어갑니다!"
```

#### PrintManager와 비교
```cpp
// PrintManager
PrintManager::GetInstance()->SetLineLimit(50);
PrintManager::GetInstance()->PrintLog("긴 텍스트...");
// ✅ 자동 줄바꿈

// TextRenderer
textRenderer->EnableAutoWrap(true);
textRenderer->SetWrapWidth(50);
textRenderer->AddLine("긴 텍스트...");
// ✅ 동일한 자동 줄바꿈
```

#### UTF-8 한글 처리
```cpp
// 한글은 자동으로 2칸으로 계산됨
textRenderer->SetWrapWidth(20);
textRenderer->AddLine("한글은 두 칸씩 차지합니다");

// 출력:
// "한글은 두 칸씩"  (10글자 = 20칸)
// "차지합니다"     (5글자 = 10칸)
```

---

## 🎨 실전 사용 예시

### 예시 1: 전투 로그 (색상 + 중요도)
```cpp
auto battleLog = std::make_unique<TextRenderer>();

// 전투 시작
battleLog->AddLogLine("전투 시작!", ELogImportance::DISPLAY);

// 플레이어 행동 (초록색)
battleLog->AddLineWithColor("플레이어의 공격!", static_cast<WORD>(ETextColor::LIGHT_GREEN));
battleLog->AddLineWithColor("몬스터에게 50 데미지!", static_cast<WORD>(ETextColor::LIGHT_GREEN));

// 몬스터 행동 (빨간색)
battleLog->AddLineWithColor("몬스터의 반격!", static_cast<WORD>(ETextColor::LIGHT_RED));
battleLog->AddLineWithColor("플레이어에게 30 데미지!", static_cast<WORD>(ETextColor::LIGHT_RED));

// 경고
battleLog->AddLogLine("HP가 낮습니다!", ELogImportance::WARNING);
```

---

### 예시 2: 스토리 대화 (타이핑 효과)
```cpp
auto dialogue = std::make_unique<TextRenderer>();

// 타이핑 효과 설정
dialogue->EnableTypingEffect(true);
dialogue->SetTypingSpeed(ETypingSpeed::Normal);

// 대화 추가
dialogue->AddLineWithTyping("NPC: 안녕하세요, 용사님!", 14);
dialogue->AddLineWithTyping("", 15);
dialogue->AddLineWithTyping("NPC: 이 마을에 괴물이 나타났습니다.", 11);
dialogue->AddLineWithTyping("NPC: 부탁드립니다, 도와주세요!", 11);

// 게임 루프
while (!dialogueComplete)
{
    drawer->Update();  // 타이핑 효과 재생
    
    // ESC로 스킵
    if (_kbhit() && _getch() == 27)
 break;
   
    Sleep(16);
}
```

---

### 예시 3: 공지사항 (자동 줄바꿈 + 중요도)
```cpp
auto notice = std::make_unique<TextRenderer>();

// 자동 줄바꿈 활성화
notice->EnableAutoWrap(true);
notice->SetWrapWidth(80);

// 제목
notice->AddLogLine("중요 공지", ELogImportance::DISPLAY);
notice->AddLine("");

// 본문 (자동 줄바꿈)
notice->AddLine("안녕하세요 용사님들! 오늘은 게임 업데이트 내용을 안내해 드리겠습니다. 먼저, 새로운 던전이 추가되었습니다. 또한 아이템 시스템이 개선되었으며, 여러 버그가 수정되었습니다. 즐거운 게임 되세요!");

// 경고
notice->AddLine("");
notice->AddLogLine("서버 점검: 오늘 오후 2시", ELogImportance::WARNING);
```

---

## 📊 기능 요약표

### AddLine 계열 메서드

| 메서드 | 기능 | PrintManager 대응 |
|--------|------|-------------------|
| `AddLine(text)` | 기본 텍스트 추가 | `PrintLog` |
| `AddLineWithColor(text, color)` | 색상 지정 텍스트 추가 | `PrintColorText` |
| `AddLogLine(text, importance)` | Log 중요도 텍스트 추가 | `PrintLogLine` |
| `AddLineWithTyping(text, color)` | 타이핑 효과 텍스트 추가 | `PrintWithTyping` |

### 설정 메서드

| 메서드 | 기능 | PrintManager 대응 |
|--------|------|-------------------|
| `SetTextColor(color)` | 기본 색상 설정 | `ChangeTextColor` |
| `EnableTypingEffect(bool)` | 타이핑 효과 활성화 | - |
| `SetTypingSpeed(speed)` | 타이핑 속도 설정 | `SetTypingSpeed` |
| `EnableAutoWrap(bool)` | 자동 줄바꿈 활성화 | - |
| `SetWrapWidth(width)` | 줄바꿈 기준 너비 | `SetLineLimit` |
| `SetAutoScroll(bool)` | 자동 스크롤 설정 | - |

---

## 🎮 테스트 예제

### 예제 7: TextRenderer 고급 기능
UIDrawer_TestExample.cpp의 `TestScene_AdvancedTextRenderer()` 참고

**시연 내용:**
- Log 중요도 (NONE, DISPLAY, WARNING)
- 줄별 색상 지정
- 자동 줄바꿈

### 예제 8: 타이핑 효과 실시간 데모
UIDrawer_TestExample.cpp의 `TestScene_TypingEffectDemo()` 참고

**시연 내용:**
- 타이핑 효과 실시간 재생
- 타이핑 속도 조절
- 게임 루프와 통합

---

## 💡 베스트 프랙티스

### 1. 전투 로그에는 색상 + 중요도
```cpp
auto battleLog = std::make_unique<TextRenderer>();
battleLog->SetAutoScroll(true);  // 최신 로그 표시

// 일반 행동
battleLog->AddLineWithColor("플레이어 공격", WORD(ETextColor::LIGHT_GREEN));

// 중요 이벤트
battleLog->AddLogLine("크리티컬 히트!", ELogImportance::DISPLAY);

// 위험 경고
battleLog->AddLogLine("HP 부족!", ELogImportance::WARNING);
```

### 2. 대화/스토리에는 타이핑 효과
```cpp
auto dialogue = std::make_unique<TextRenderer>();
dialogue->EnableTypingEffect(true);
dialogue->SetTypingSpeed(ETypingSpeed::Normal);

dialogue->AddLineWithTyping("NPC: 환영합니다!", 14);
dialogue->AddLineWithTyping("...", 7);
dialogue->AddLineWithTyping("NPC: 무엇을 도와드릴까요?", 14);
```

### 3. 긴 설명에는 자동 줄바꿈
```cpp
auto description = std::make_unique<TextRenderer>();
description->EnableAutoWrap(true);
description->SetWrapWidth(패널너비 - 4);  // 패딩 고려

description->AddLine("긴 아이템 설명 텍스트...");
```

---

## ⚙️ 고급 설정

### 타이핑 속도 비교
```cpp
// 느림 (100ms/글자) - 감정적인 장면
textRenderer->SetTypingSpeed(ETypingSpeed::Slow);

// 보통 (50ms/글자) - 일반 대화
textRenderer->SetTypingSpeed(ETypingSpeed::Normal);

// 빠름 (20ms/글자) - 전투 로그
textRenderer->SetTypingSpeed(ETypingSpeed::Fast);
```

### 스크롤 제어
```cpp
// 자동 스크롤 (기본값)
textRenderer->SetAutoScroll(true);
// → 항상 최신 줄 표시 (로그에 적합)

// 수동 스크롤
textRenderer->SetAutoScroll(false);
textRenderer->ScrollUp();    // 위로 스크롤
textRenderer->ScrollDown();  // 아래로 스크롤
// → 대화 기록 탐색에 적합
```

---

## 🆚 PrintManager를 언제 사용해야 하나?

### PrintManager 사용 권장
- 간단한 콘솔 출력
- 디버그 로그
- UIDrawer 없이 동작하는 부분
- 빠른 프로토타이핑

### TextRenderer 사용 권장
- UIDrawer 화면
- 패널 기반 UI
- 색상/효과가 필요한 경우
- 게임 플레이 중 UI

---

## 📚 관련 문서

- **UIDrawer_README.md** - UIDrawer 전체 가이드
- **UIDrawer_TestExample_README.md** - 테스트 예제 사용법
- **include/UI/TextRenderer.h** - TextRenderer 헤더
- **include/Manager/PrintManager.h** - PrintManager 헤더

---

## ✅ 마이그레이션 가이드

### PrintManager → TextRenderer 전환

#### Before (PrintManager)
```cpp
PrintManager* pm = PrintManager::GetInstance();
pm->SetLineLimit(80);
pm->ChangeTextColor(ETextColor::YELLOW);
pm->PrintLogLine("중요!", ELogImportance::DISPLAY);
pm->PrintWithTyping("안녕하세요");
```

#### After (TextRenderer)
```cpp
auto textRenderer = std::make_unique<TextRenderer>();
textRenderer->EnableAutoWrap(true);
textRenderer->SetWrapWidth(80);
textRenderer->SetTextColor(static_cast<WORD>(ETextColor::YELLOW));
textRenderer->AddLogLine("중요!", ELogImportance::DISPLAY);
textRenderer->EnableTypingEffect(true);
textRenderer->AddLineWithTyping("안녕하세요");

panel->SetContentRenderer(std::move(textRenderer));
drawer->Update();  // 타이핑 효과 재생
```

---

**작성일**: 2024  
**작성자**: UIDrawer System Team  
**버전**: 2.0 (PrintManager 완벽 호환)
