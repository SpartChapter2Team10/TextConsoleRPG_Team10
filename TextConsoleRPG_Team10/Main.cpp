#include "include/Manager/GameManager.h"
#include "include/Manager/DataManager.h"
#include "include/Manager/PrintManager.h"
//#include "SoundPlayer.h"  // SoundPlayer 테스트용 - 필요시 주석 해제
#include <Windows.h>
#include <iostream>

// UIDrawer 테스트 함수 선언
void RunUIDrawerTest();

// 콘솔 창 크기 고정 함수 (system 명령어 사용)
void SetConsoleWindowSize(int width, int height)
{
    // mode con 명령어로 콘솔 크기 설정
    // cols = 가로(너비), lines = 세로(높이)
    char command[256];
    sprintf_s(command, "mode con: cols=%d lines=%d", width, height);
    system(command);

    // 창 크기 조절 비활성화
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow)
    {
        LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
        style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
        SetWindowLong(consoleWindow, GWL_STYLE, style);
        SetWindowPos(consoleWindow, NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
}

int main()
{
    // 1. 콘솔 인코딩 설정 (한글 깨짐 방지)
    SetConsoleOutputCP(65001);

    // 2. 콘솔 창 크기 고정 (가로 106, 세로 60)
    SetConsoleWindowSize(50, 50);

    // 3. 매니저들 초기화 (DataManager -> SoundPlayer 순서 권장)
    if (!DataManager::GetInstance()->Initialize()) {
        PrintManager::GetInstance()->PrintLogLine("DataManager 초기화 실패!", ELogImportance::WARNING);
        return -1;
    }

    // ========================================
    // UIDrawer 테스트 모드
    // ========================================
    // 팀원들이 GameScene을 만들 수 있도록 UI 시스템을 테스트합니다.
    // 거의 모든 UIDrawer 기능을 시연하는 예제입니다.
    // 
    // GameManager와 통합하려면 아래 주석을 해제하고
    // RunUIDrawerTest() 호출을 주석 처리하세요.

    RunUIDrawerTest();

    // ========================================
    // GameManager 모드 (현재 비활성화)
    // ========================================
    // GameManager와 통합 시 아래 주석을 해제하세요
    // GameManager::GetInstance()->StartGame();

        // === SoundPlayer 테스트 코드 (주석 처리) ===
    /*
    if (!SoundPlayer::GetInstance()->Initialize()) {
        PrintManager::GetInstance()->PrintLogLine("SoundPlayer 초기화 실패!", ELogImportance::WARNING);
        return -1;
    }

    PrintManager::GetInstance()->PrintLogLine("시스템 초기화 완료. 테스트를 시작합니다.",ELogImportance::DISPLAY);
    PrintManager::GetInstance()->PrintLogLine("[1] 배경음악 재생 (Loop)");
    PrintManager::GetInstance()->PrintLogLine("[2] 효과음 재생 (Play)");
    PrintManager::GetInstance()->PrintLogLine("[ESC] 종료");

    // 테스트용 배경음악 재생 (파일이 해당 경로에 있어야 합니다)
    // 예: Source/Game/Assets/Audio/BGM_Main.mp3
    SoundPlayer::GetInstance()->PlayLoop(
      "812426__dinisnakamura__battle_bgm_castle_dragon_extended.wav",
        "MainBGM", 0.3f);

    bool bRunning = true;
    while (bRunning) {
        // 키 입력 처리 (비동기 방식 테스트)
        if (GetAsyncKeyState('2') & 0x8000) {
            // 효과음 재생 (짧은 사운드 파일)
    SoundPlayer::GetInstance()->Play("swish_2.wav", 0.8f);
            Sleep(100); // 연속 입력 방지
      }

  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            bRunning = false;
        }

        // 3. ★가장 중요★ 매 프레임 사운드 상태 업데이트
        // 이 함수가 호출되어야 재생 끝난 사운드가 메모리에서 해제됩니다.
        SoundPlayer::GetInstance()->Update();

    // CPU 점유율 과다 방지를 위한 미세한 대기
    Sleep(10);
    }

    // 4. 종료 시 모든 사운드 정지 및 리소스 해제
    SoundPlayer::GetInstance()->StopAll();

    PrintManager::GetInstance()->PrintLogLine("테스트를 종료합니다.");
    */
    // === SoundPlayer 테스트 코드 끝 ===

    return 0;
}
