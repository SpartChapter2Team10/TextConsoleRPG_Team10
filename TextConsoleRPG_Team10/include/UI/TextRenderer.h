#pragma once
#include "IContentRenderer.h"
#include <vector>
#include <string>
#include <Windows.h>
#include <chrono>

// Log 중요도 (PrintManager와 동일)
enum class ELogImportance : int;

// 타이핑 속도 (PrintManager와 동일)
enum class ETypingSpeed : int;

// 개별 줄의 메타데이터
struct TextLine
{
    std::string Text;
    WORD Color = 15;  // 기본 색상 (전경색 + 배경색 조합)
    ELogImportance Importance = static_cast<ELogImportance>(0);  // NONE
    bool HasTypingEffect = false;

    TextLine() = default;
    TextLine(const std::string& text, WORD color = 15)
        : Text(text), Color(color) {
    }
    TextLine(const std::string& text, WORD color, ELogImportance importance)
        : Text(text), Color(color), Importance(importance) {
    }
};

// 기본 텍스트 렌더러
// 여러 줄의 텍스트를 표시 (스크롤 로그 등에 사용)
class TextRenderer : public IContentRenderer
{
private:
    std::vector<TextLine> _Lines;
    WORD _DefaultTextColor = 15;  // 기본 색상 (흰색)
    bool _AutoScroll = true; // 자동 스크롤 (최신 줄 표시)
    int _ScrollOffset = 0;  // 스크롤 위치
    bool _IsDirty = true;

    // 타이핑 효과 관련
    bool _TypingEnabled = false;
    ETypingSpeed _TypingSpeed = static_cast<ETypingSpeed>(1);  // Normal
    int _CurrentTypingLine = -1;  // 현재 타이핑 중인 줄 인덱스
    size_t _CurrentTypingChar = 0;  // 현재 타이핑 중인 문자 위치
    std::chrono::steady_clock::time_point _LastTypingTime;

    // 자동 줄바꿈 관련
    bool _AutoWrap = false;
    int _WrapWidth = 100;  // 줄바꿈 기준 너비 (글자 수)

    // 플리커링 효과 관련
    bool _FlickerEnabled = false;
    float _FlickerInterval = 0.5f;  // 깜빡임 간격 (초)
    float _FlickerTimer = 0.0f;
    bool _FlickerState = true;  // true = 원래 색상, false = 대체 색상
    WORD _FlickerAlternateColor = 0;  // 깜빡일 때 사용할 색상

    // 내부 헬퍼 함수
    std::vector<std::string> WrapText(const std::string& text, int maxWidth);
    int GetTypingInterval() const;
    bool UpdateTypingEffect();

public:
    TextRenderer() = default;

    // ===== 기본 기능 =====

    // 텍스트 추가 (기본)
    void AddLine(const std::string& line);
    void AddLines(const std::vector<std::string>& lines);

    // ===== PrintManager 호환 기능 =====

    // 색상이 있는 줄 추가
    void AddLineWithColor(const std::string& line, WORD color);

    // Log 중요도가 있는 줄 추가
    void AddLogLine(const std::string& line, ELogImportance importance);

    // 타이핑 효과가 있는 줄 추가
    void AddLineWithTyping(const std::string& line, WORD color = 15);

    // 색상별 텍스트 추가 (한 줄에 여러 색상)
    void AddColoredText(const std::string& text, WORD color);
    void FlushColoredLine();  // 색상별 텍스트를 하나의 줄로 완성

    // ===== 설정 =====

    // 텍스트 클리어
    void Clear();

    // 기본 색상 설정 (전경색 + 배경색)
    void SetTextColor(WORD color) { _DefaultTextColor = color; _IsDirty = true; }
    WORD GetTextColor() const { return _DefaultTextColor; }

    // 플리커링 효과 설정
    void EnableFlicker(bool enable, float interval = 0.5f, WORD alternateColor = 0);
    void SetFlickerInterval(float interval) { _FlickerInterval = interval; }
    void SetFlickerAlternateColor(WORD color) { _FlickerAlternateColor = color; }

    // 스크롤 설정
    void SetAutoScroll(bool enable) { _AutoScroll = enable; }
    void ScrollUp() { if (_ScrollOffset > 0) _ScrollOffset--; _IsDirty = true; }
    void ScrollDown() { _ScrollOffset++; _IsDirty = true; }

    // 타이핑 효과 설정
    void EnableTypingEffect(bool enable) { _TypingEnabled = enable; }
    void SetTypingSpeed(ETypingSpeed speed) { _TypingSpeed = speed; }

    // 자동 줄바꿈 설정
    void EnableAutoWrap(bool enable) { _AutoWrap = enable; }
    void SetWrapWidth(int width) { _WrapWidth = width; }

    // ===== 업데이트 (타이핑 효과 + 플리커링) =====

    void Update(float deltaTime) override;  // 타이핑 효과 + 플리커링 업데이트

    // IContentRenderer 구현
    void Render(ScreenBuffer& buffer, const PanelBounds& bounds) override;
    bool IsDirty() const override { return _IsDirty; }
    void SetDirty() override { _IsDirty = true; }

private:
    // 색상별 텍스트 조합용 임시 버퍼
    std::string _ColoredLineBuffer;
    WORD _ColoredLineColor = 15;
};
