#pragma once
#include <Windows.h>  // WORD 타입을 위해 필요

// 타이핑 속도 지정에 사용
enum class ETypingSpeed : int
{
    NONE = -1,
    Slow = 0,
    Normal,
    Fast,
    MAX
};

// 텍스트 색상 변경에 사용
enum class ETextColor : int
{
    NONE = -1,
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    YELLOW,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_YELLOW,
    WHITE,
    MAX
};

// 배경색 (전경색과 동일한 색상 사용)
enum class EBackgroundColor : int
{
    NONE = -1,
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    YELLOW,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_YELLOW,
    WHITE,
    MAX
};

// Log 출력 시 사용
enum class ELogImportance : int
{
    NONE = 0,  // 일반 출력
    DISPLAY,   // [DISPLAY] ~ 출력
    WARNING,   // [WARNING] ~ 출력
    MAX
};

// 색상 조합 헬퍼 함수
inline WORD MakeColorAttribute(ETextColor foreground, EBackgroundColor background = EBackgroundColor::BLACK)
{
    return static_cast<WORD>(foreground) | (static_cast<WORD>(background) << 4);
}

// 오버로드 (WORD 직접 사용)
inline WORD MakeColorAttribute(WORD foreground, WORD background)
{
    return (foreground & 0x0F) | ((background & 0x0F) << 4);
}
