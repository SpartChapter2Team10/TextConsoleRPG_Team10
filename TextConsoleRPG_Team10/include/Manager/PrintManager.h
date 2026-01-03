#pragma once
#include "../Singleton.h"
#include "../Common/TextColor.h"  // Enum 정의를 별도 헤더로 분리
#include <string>

class PrintManager : public Singleton<PrintManager>
{
private:
    // 현재 타이핑 속도
    ETypingSpeed _CurrentSpeed = ETypingSpeed::Normal;
    // 줄 당 글자 제한 수(= 줄 길이)
    int _LineLimit = 106;
    // 현재 줄에 출력된 글자 수
    int _CurrentCharCnt = 0;

public:
    // 로그 출력 및 일반 출력 함수
    void PrintLog(const std::string& Msg, ELogImportance Importance = ELogImportance::NONE);
    // 로그 출력 및 일반 출력 함수 + 개행
    void PrintLogLine(const std::string& Msg, ELogImportance Importance = ELogImportance::NONE);
    
    // 타이핑 효과를 적용한 출력 함수
    void PrintWithTyping(const std::string& Msg);
    // 타이핑 효과를 적용한 출력 함수 + 개행
    void PrintWithTypingLine(const std::string& Msg);

    void PrintColorText(const std::string& Msg, ETextColor Color);
    
    // 콘솔 텍스트 색상 변경 함수
    void ChangeTextColor(ETextColor NewTextColor = ETextColor::WHITE);
    // 현재 콘솔 텍스트 색상 반환
    const ETextColor GetCurrentTextColor();
    // 개행
    void EndLine();

    // 한 줄당 글자 제한 수 설정 함수
    void SetLineLimit(int Limit);
    // 한 줄당 글자 제한 수 반환 함수
    const int& GetLineLimit() const;
    // 타이핑 효과의 속도 설정 함수
    void SetTypingSpeed(ETypingSpeed NewSpeed);
    int GetIntervalTime() const;
};