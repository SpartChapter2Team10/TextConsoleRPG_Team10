#include "../../include/UI/InputBridge.h"
#include "../../include/UI/Panel.h"
#include "../../include/UI/TextRenderer.h"
#include "../../include/Manager/InputManager.h"

InputBridge::InputBridge()
    : _InputPanel(nullptr)
    , _InputRenderer(nullptr)
    , _IsWaitingInput(false)
{
}

InputBridge::~InputBridge()
{
}

void InputBridge::SetInputPanel(Panel* panel)
{
    _InputPanel = panel;
    if (_InputPanel)
    {
        _InputRenderer = dynamic_cast<TextRenderer*>(_InputPanel->GetContentRenderer());
 }
}

void InputBridge::UpdateInputDisplay(const std::string& text)
{
    if (_InputRenderer)
    {
  _InputRenderer->Clear();  // ClearLines → Clear
 _InputRenderer->AddLine(_Prompt);
_InputRenderer->AddLine("> " + text);
  if (_InputPanel)
    {
          _InputPanel->Redraw();
        }
    }
}

std::string InputBridge::RequestInput(const std::string& prompt)
{
    _Prompt = prompt;
    _IsWaitingInput = true;
    
    UpdateInputDisplay("");
    
    // 동기식 입력 (기존 InputManager 사용)
 std::string result = InputManager::GetInstance()->GetInput(prompt);
    
    _IsWaitingInput = false;
    return result;
}

int InputBridge::RequestIntInput(const std::string& prompt, int min, int max)
{
    _Prompt = prompt;
    _IsWaitingInput = true;
    
    UpdateInputDisplay("");
    
    int result = InputManager::GetInstance()->GetIntInput(prompt, min, max);
    
    _IsWaitingInput = false;
    return result;
}

char InputBridge::RequestCharInput(const std::string& prompt, const std::string& validChars)
{
    _Prompt = prompt;
    _IsWaitingInput = true;
    
    UpdateInputDisplay("");
    
    char result = InputManager::GetInstance()->GetCharInput(prompt, validChars);
    
    _IsWaitingInput = false;
    return result;
}
