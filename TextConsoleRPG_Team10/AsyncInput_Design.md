# 비동기 입력 시스템 설계 문서

## 📋 현재 문제점

### InputBridge의 현재 상태
- ✅ **장점**: InputManager를 래핑하여 UI와 통합
- ❌ **단점**: 동기식 입력으로 인한 블로킹
  - 입력 대기 중 애니메이션 정지
  - 입력 대기 중 UI 업데이트 불가
  - 게임 루프 정지

### 코드 분석
```cpp
// 현재 InputBridge::RequestInput
std::string InputBridge::RequestInput(const std::string& prompt)
{
    _IsWaitingInput = true;

    // ❌ 이 줄에서 블로킹됨 (ReadConsoleW가 입력 대기)
    std::string result = InputManager::GetInstance()->GetInput(prompt);
    
    _IsWaitingInput = false;
    return result;
}
```

### 문제 시나리오
```
시간축 →
─────────────────────────────────────────────────
게임 루프:  ■■■■■ [정지...........................] ■■■■■
애니메이션: ████  [정지...........................] ████
입력:     [대기 중.............] ✓
─────────────────────────────────────────────────
                ↑ 이 구간에서 모든 것이 멈춤
```

**원하는 동작:**
```
시간축 →
─────────────────────────────────────────────────
게임 루프:  ████████████████████████████████████
애니메이션: ████████████████████████████████████
입력:           [비동기 대기........] ✓
─────────────────────────────────────────────────
           ↑ 모든 것이 계속 실행됨
```

---

## 🎯 설계 목표

1. **비블로킹 입력**: 입력 대기 중에도 게임 루프 실행
2. **UI 업데이트 유지**: 애니메이션, 타이머 등 계속 작동
3. **기존 코드 호환**: InputManager를 그대로 사용 가능
4. **간단한 API**: 팀원들이 쉽게 사용 가능

---

## 🏗️ 설계 옵션

### 옵션 A: 폴링(Polling) 방식 ⭐ 권장

#### 개념
매 프레임마다 입력 버퍼를 체크하여 입력이 있는지 확인합니다.

#### 장점
- ✅ 구현이 비교적 간단
- ✅ Windows API 활용 (`PeekConsoleInput`)
- ✅ 기존 코드 수정 최소화

#### 단점
- ❌ 매 프레임 체크로 인한 약간의 오버헤드

#### 구현 방법
```cpp
class AsyncInputBridge
{
private:
    std::string _InputBuffer;
    bool _InputReady;
  
public:
    // 매 프레임 호출
    void Update()
    {
     HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
INPUT_RECORD inputRecord;
        DWORD eventsRead;
        
        // 입력 버퍼 체크 (비블로킹)
        while (PeekConsoleInput(hInput, &inputRecord, 1, &eventsRead) && eventsRead > 0)
        {
         if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
            {
           // 키 처리
 char ch = inputRecord.Event.KeyEvent.uChar.AsciiChar;
                
      if (ch == '\r' || ch == '\n')
          {
         _InputReady = true;
  }
                else
         {
            _InputBuffer += ch;
        }
            }
            
        // 이벤트 제거
       ReadConsoleInput(hInput, &inputRecord, 1, &eventsRead);
        }
 }
    
    // 입력이 완료되었는지 확인
    bool IsInputReady() const { return _InputReady; }
    
  // 완료된 입력 가져오기
    std::string GetInput()
    {
        std::string result = _InputBuffer;
  _InputBuffer.clear();
        _InputReady = false;
        return result;
    }
};
```

#### 사용 예시
```cpp
void GameLoop()
{
    UIDrawer* drawer = UIDrawer::GetInstance();
    AsyncInputBridge* input = AsyncInputBridge::GetInstance();
    
 drawer->Activate();
    
    // 입력 요청
    input->StartInput("명령을 입력하세요: ");
    
    while (!input->IsInputReady())
    {
        // ✅ 입력 대기 중에도 계속 실행됨
    input->Update();        // 입력 체크
        drawer->Update();       // 애니메이션 업데이트
        Sleep(16);       // ~60 FPS
    }

    std::string userInput = input->GetInput();
}
```

---

### 옵션 B: 스레드(Thread) 방식

#### 개념
별도 스레드에서 입력을 대기하고, 메인 스레드는 게임 로직 실행.

#### 장점
- ✅ 완전한 비동기
- ✅ 입력 처리와 게임 로직 완전 분리

#### 단점
- ❌ 구현 복잡도 높음
- ❌ 스레드 동기화 필요 (mutex, condition_variable)
- ❌ 디버깅 어려움

#### 구현 방법
```cpp
class ThreadedInputBridge
{
private:
std::thread _InputThread;
    std::mutex _Mutex;
 std::condition_variable _CV;
    std::string _InputBuffer;
    bool _InputReady;
 bool _Running;
    
    void InputThreadFunc()
    {
      while (_Running)
        {
     std::string input;
            std::getline(std::cin, input);
    
            {
         std::lock_guard<std::mutex> lock(_Mutex);
       _InputBuffer = input;
      _InputReady = true;
        }
            _CV.notify_one();
   }
    }
    
public:
    void StartInput()
    {
        _Running = true;
        _InputThread = std::thread(&ThreadedInputBridge::InputThreadFunc, this);
    }
    
    bool IsInputReady()
    {
        std::lock_guard<std::mutex> lock(_Mutex);
        return _InputReady;
    }
    
    std::string GetInput()
    {
   std::unique_lock<std::mutex> lock(_Mutex);
  _CV.wait(lock, [this] { return _InputReady; });
        
      std::string result = _InputBuffer;
        _InputBuffer.clear();
        _InputReady = false;
      return result;
    }
};
```

#### 문제점
- Windows Console API는 스레드 안전하지 않음
- `ReadConsoleW`를 여러 스레드에서 호출 시 문제 발생 가능

---

### 옵션 C: 콜백(Callback) 방식

#### 개념
입력이 완료되면 콜백 함수를 호출합니다.

#### 장점
- ✅ 이벤트 기반 설계
- ✅ 코드 구조 명확

#### 단점
- ❌ 폴링 또는 스레드 기반 구현 필요
- ❌ 콜백 헬(callback hell) 가능성

#### 구현 방법
```cpp
class CallbackInputBridge
{
private:
    std::function<void(std::string)> _Callback;
    
public:
    void RequestInput(const std::string& prompt, std::function<void(std::string)> callback)
    {
        _Callback = callback;
    // 폴링 또는 스레드로 입력 대기...
    }
    
    void OnInputComplete(const std::string& input)
    {
    if (_Callback)
 {
          _Callback(input);
         _Callback = nullptr;
        }
    }
};
```

#### 사용 예시
```cpp
input->RequestInput("명령: ", [](std::string userInput) {
    // 입력 완료 후 실행
  ProcessCommand(userInput);
});

// 게임 루프는 계속 실행
while (running) {
    drawer->Update();
}
```

---

## ✅ 권장 구현: 폴링 방식 (옵션 A)

### 이유
1. **구현 난이도**: 낮음 (Win32 API 직접 사용)
2. **안정성**: 높음 (스레드 동기화 불필요)
3. **호환성**: 기존 코드와 완벽 호환
4. **팀 프로젝트**: 팀원들이 이해하기 쉬움

### 구현 계획

#### 1단계: AsyncInputHandler 클래스 생성
```cpp
// include/UI/AsyncInputHandler.h
class AsyncInputHandler
{
private:
    std::wstring _InputBuffer;
    bool _InputReady;
    bool _IsWaiting;
    
public:
    void StartInput();
    void Update();  // 매 프레임 호출
    bool IsInputReady() const;
    std::string GetInput();
    void ClearInput();
};
```

#### 2단계: InputBridge에 통합
```cpp
// InputBridge에 AsyncInputHandler 추가
class InputBridge
{
private:
    AsyncInputHandler _AsyncHandler;
    
public:
    // 비동기 입력 (새로운 API)
    void StartAsyncInput(const std::string& prompt);
    bool IsAsyncInputReady() const;
    std::string GetAsyncInput();
    
    // 동기식 입력 (기존 API 유지)
    std::string RequestInput(const std::string& prompt);
};
```

#### 3단계: UIDrawer 게임 루프 통합
```cpp
void UIDrawer::Update()
{
    // 입력 핸들러 업데이트
    InputBridge::GetInstance()->UpdateAsyncInput();
    
    // 애니메이션 업데이트
  UpdateAnimations(_DeltaTime);
    
    // 렌더링
  Render();
}
```

---

## 📝 상세 구현 예시

### AsyncInputHandler.h
```cpp
#pragma once
#include <string>
#include <Windows.h>

class AsyncInputHandler
{
private:
    std::wstring _InputBuffer;
    std::string _Prompt;
    bool _InputReady;
    bool _IsWaiting;
    int _CursorPos;
    
public:
    AsyncInputHandler();
    
    // 비동기 입력 시작
    void StartInput(const std::string& prompt);
    
    // 매 프레임 호출 - 입력 체크
    void Update();
    
  // 입력이 완료되었는지 확인
    bool IsInputReady() const { return _InputReady; }
    
    // 입력 대기 중인지 확인
    bool IsWaiting() const { return _IsWaiting; }
    
    // 완료된 입력 가져오기 (UTF-8 변환)
    std::string GetInput();
    
    // 현재 입력 버퍼 가져오기
    std::wstring GetCurrentBuffer() const { return _InputBuffer; }
    
    // 입력 초기화
    void ClearInput();
};
```

### AsyncInputHandler.cpp
```cpp
#include "AsyncInputHandler.h"

AsyncInputHandler::AsyncInputHandler()
    : _InputReady(false)
    , _IsWaiting(false)
    , _CursorPos(0)
{
}

void AsyncInputHandler::StartInput(const std::string& prompt)
{
    _Prompt = prompt;
    _InputBuffer.clear();
    _InputReady = false;
    _IsWaiting = true;
  _CursorPos = 0;
}

void AsyncInputHandler::Update()
{
    if (!_IsWaiting) return;
    
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inputRecord[32];
    DWORD eventsRead = 0;
    
    // 입력 이벤트 확인 (비블로킹)
    if (!PeekConsoleInput(hInput, inputRecord, 32, &eventsRead) || eventsRead == 0)
   return;
    
    // 이벤트 읽기
    ReadConsoleInput(hInput, inputRecord, eventsRead, &eventsRead);
    
    for (DWORD i = 0; i < eventsRead; ++i)
    {
        if (inputRecord[i].EventType != KEY_EVENT)
 continue;
        
        KEY_EVENT_RECORD& keyEvent = inputRecord[i].Event.KeyEvent;
      
        // 키를 누를 때만 처리 (떼는 이벤트 무시)
     if (!keyEvent.bKeyDown)
            continue;
    
        wchar_t ch = keyEvent.uChar.UnicodeChar;
        
        // Enter 키
      if (keyEvent.wVirtualKeyCode == VK_RETURN)
        {
       _InputReady = true;
        _IsWaiting = false;
   return;
        }
        // Backspace 키
        else if (keyEvent.wVirtualKeyCode == VK_BACK)
        {
     if (!_InputBuffer.empty())
 {
      _InputBuffer.pop_back();
  }
        }
        // 일반 문자
        else if (ch >= 32)  // 제어 문자 제외
        {
            _InputBuffer.push_back(ch);
        }
    }
}

std::string AsyncInputHandler::GetInput()
{
    if (!_InputReady)
        return "";
    
    // UTF-8 변환
    if (_InputBuffer.empty())
        return "";
    
    int size_needed = WideCharToMultiByte(
      CP_UTF8, 0,
        _InputBuffer.c_str(), static_cast<int>(_InputBuffer.size()),
        NULL, 0, NULL, NULL
    );
    
    std::string utf8Str(size_needed, 0);
    WideCharToMultiByte(
      CP_UTF8, 0,
      _InputBuffer.c_str(), static_cast<int>(_InputBuffer.size()),
        &utf8Str[0], size_needed, NULL, NULL
    );
    
    ClearInput();
    return utf8Str;
}

void AsyncInputHandler::ClearInput()
{
    _InputBuffer.clear();
    _InputReady = false;
    _IsWaiting = false;
    _CursorPos = 0;
}
```

### 사용 예시
```cpp
void BattleScene::Enter()
{
    UIDrawer* drawer = UIDrawer::GetInstance();
    InputBridge* input = InputBridge::GetInstance();
    
    // UI 구성
    Panel* battlePanel = drawer->CreatePanel("Battle", 0, 0, 106, 40);
    auto battleAnim = std::make_unique<AsciiArtRenderer>();
    battleAnim->LoadAnimationFromFolder("Resources/Animations/BattleIdle", 0.3f);
    battleAnim->StartAnimation();
    battlePanel->SetContentRenderer(std::move(battleAnim));
    
    Panel* inputPanel = drawer->CreatePanel("Input", 0, 40, 106, 10);
    auto inputText = std::make_unique<TextRenderer>();
    inputPanel->SetContentRenderer(std::move(inputText));
    
    drawer->Render();
    
    // 비동기 입력 시작
    input->StartAsyncInput("명령을 입력하세요: ");
    
    // 게임 루프
    while (!input->IsAsyncInputReady())
    {
      // ✅ 애니메이션은 계속 재생됨
        drawer->Update();// 내부에서 input->UpdateAsyncInput() 호출
        
   // ✅ 현재 입력 상태를 UI에 표시
        TextRenderer* inputTextRenderer = 
          dynamic_cast<TextRenderer*>(inputPanel->GetContentRenderer());
        if (inputTextRenderer)
  {
         inputTextRenderer->Clear();
            inputTextRenderer->AddLine("명령: " + input->GetCurrentAsyncBuffer());
  inputPanel->Redraw();
        }
        
        Sleep(16);  // ~60 FPS
  }
    
    // 입력 완료
    std::string command = input->GetAsyncInput();
    ProcessCommand(command);
}
```

---

## 🚧 구현 시 주의사항

### 1. Windows Console 모드 설정
```cpp
void SetupConsoleForAsyncInput()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
DWORD mode;
    GetConsoleMode(hInput, &mode);
  
    // 라인 입력 모드 비활성화 (문자 단위 입력)
  mode &= ~ENABLE_LINE_INPUT;
    
 // 에코 비활성화 (직접 화면에 표시)
    mode &= ~ENABLE_ECHO_INPUT;
    
    // 처리된 입력 모드 활성화 (화살표 키 등 인식)
    mode |= ENABLE_PROCESSED_INPUT;
    
    SetConsoleMode(hInput, mode);
}
```

### 2. UTF-8 한글 처리
- `ReadConsoleW` 사용 (유니코드)
- `WideCharToMultiByte`로 UTF-8 변환

### 3. 입력 버퍼 초기화
```cpp
void FlushConsoleInputBuffer()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hInput);
}
```

### 4. 동기식/비동기 혼용
```cpp
// 동기식 (기존 방식)
std::string name = input->RequestInput("이름: ");  // 블로킹

// 비동기식 (새로운 방식)
input->StartAsyncInput("명령: ");
while (!input->IsAsyncInputReady()) {
    drawer->Update();  // 애니메이션 계속 재생
}
std::string command = input->GetAsyncInput();
```

---

## 📊 성능 고려사항

### 폴링 오버헤드
- `PeekConsoleInput`: 매우 빠름 (~0.001ms)
- 60 FPS 기준: 프레임당 0.001ms → 무시 가능

### 메모리 사용
- `std::wstring _InputBuffer`: 최대 1024자 = ~2KB
- 영향: 미미함

---

## 🎯 결론

### 권장 사항
1. **단기적**: InputBridge에 누락된 메서드 추가 (완료 ✅)
2. **중기적**: AsyncInputHandler 구현 (폴링 방식)
3. **장기적**: 전체 UI 시스템을 이벤트 기반으로 리팩토링

### 우선순위
- **지금 필요한가?**
  - 단순한 턴제 RPG → 동기식으로 충분
  - 실시간 애니메이션 필요 → 비동기식 권장
  
- **구현 시간**
  - 동기식: 이미 완료 ✅
  - 비동기식: 1-2일 소요 예상

### 다음 단계
1. 팀 회의: 비동기 입력이 정말 필요한지 논의
2. 필요하다면: `AsyncInputHandler` 구현 시작
3. 테스트: UIDrawer_TestExample에 비동기 입력 예제 추가

---

## 📞 참고 자료

### Windows Console API
- `PeekConsoleInput`: https://docs.microsoft.com/en-us/windows/console/peekonsoleinput
- `ReadConsoleInput`: https://docs.microsoft.com/en-us/windows/console/readconsoleinput
- `INPUT_RECORD`: https://docs.microsoft.com/en-us/windows/console/input-record-str

### 관련 파일
- `include/UI/InputBridge.h`
- `src/UI/InputBridge.cpp`
- `include/Manager/InputManager.h`
- `src/Manager/InputManager.cpp`

---

**작성일**: 2024  
**작성자**: UIDrawer System Team  
**버전**: 1.0
