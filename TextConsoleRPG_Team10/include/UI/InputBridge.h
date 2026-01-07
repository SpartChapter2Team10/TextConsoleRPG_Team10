#pragma once
#include "../Singleton.h"
#include <string>
#include <functional>
#include <vector>

class Panel;
class TextRenderer;

// UIDrawer와 InputManager를 연결하는 브릿지
// 비동기 입력을 지원하여 UI 업데이트 중에도 입력 처리 가능
class InputBridge : public Singleton<InputBridge>
{
private:
    Panel* _InputPanel;
    TextRenderer* _InputRenderer;
    std::string _CurrentInput;
    std::string _Prompt;
    bool _IsWaitingInput;

private:
    InputBridge();
    friend class Singleton<InputBridge>;

    InputBridge(const InputBridge&) = delete;
    InputBridge& operator=(const InputBridge&) = delete;

public:
    ~InputBridge();

    // 입력 요청 (동기식 - 기존 InputManager 래핑)
    std::string RequestInput(const std::string& prompt);
    int RequestIntInput(const std::string& prompt, int min, int max);
    char RequestCharInput(const std::string& prompt, const std::string& validChars);
    
    // 누락된 InputManager 메서드 추가
    std::string RequestStringInput(const std::string& prompt, const std::vector<std::string>& validOptions);
    bool RequestYesNoInput(const std::string& prompt);

    // 입력 패널 설정
    void SetInputPanel(Panel* panel);
    
    // 입력 표시 업데이트
    void UpdateInputDisplay(const std::string& text);
    
    inline bool IsWaitingInput() const { return _IsWaitingInput; }
};
