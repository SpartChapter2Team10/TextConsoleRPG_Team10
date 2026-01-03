#pragma once
#include "../Singleton.h"
#include <string>
#include <vector>

class InputManager : public Singleton<InputManager>
{
public:
    // ===== 블로킹 입력 메서드 =====
    
    // 단순 문자열 입력
    std::string GetInput(const std::string& Prompt);
    // 정수 입력, 지정된 범위내의 값 받기
    int GetIntInput(const std::string& Prompt, int Min, int Max);
    // 정해진 옵션들 중 하나 입력받기
    std::string GetStringInput(const std::string& Prompt, const std::vector<std::string>& ValidOptions);
    // 정해둔 문자들 중에서 입력받기
    char GetCharInput(const std::string& Prompt, const std::string& ValidChars);
    // Yes or No 입력 받기
    bool GetYesNoInput(const std::string& Prompt);
    
    // ===== 논블로킹 입력 메서드 =====
    
    // 키가 눌렸는지 확인 (논블로킹)
    // 반환값: true - 키 입력 있음, false - 키 입력 없음
    bool IsKeyPressed() const;
    
    // 눌린 키 코드 가져오기 (논블로킹)
    // 반환값: 키 코드 (키가 없으면 0 반환)
    int GetKeyCode();
    
    // 특정 키가 눌렸는지 확인 및 소비 (논블로킹)
    // keyCode: 확인할 키 코드 (예: 27 = ESC, 32 = SPACE)
    // 반환값: 해당 키가 눌렸으면 true, 아니면 false
    bool IsKeyDown(int keyCode);
    
    // 특정 문자 키가 눌렸는지 확인 및 소비 (대소문자 구분 없음)
    // ch: 확인할 문자
    // 반환값: 해당 문자 키가 눌렸으면 true, 아니면 false
    bool IsCharPressed(char ch);
    
private:
    // 입력 버퍼 비우기
    void ClearInputBuffer();
    // UTF-8 입력 받기
    std::string GetUTFInput();
};