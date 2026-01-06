#pragma once
#include "../UIScene.h"
#include "../../Data/CompanionData.h"
#include <Windows.h>
#include <string>
#include <optional>
#include <vector>

class Panel;

// 동료 영입 Scene
class CompanionRecruitScene : public UIScene
{
private:
    std::optional<CompanionData> _CurrentCompanion;  // 현재 등장한 동료 데이터
    int _CompanionLevel;                             // 동료 레벨 (현재 층수)
    std::string _InputName;                          // 사용자가 입력한 이름
    bool _RecruitAccepted;                           // 영입 수락 여부
    int _SelectedOption;                             // 0: 영입, 1: 거부

    // ===== 대화 시스템 =====
    std::vector<std::string> _DialogueTexts;         // 대화 내용
    std::vector<WORD> _DialogueColors;        // 각 대화의 색상
    int _CurrentDialogueIndex;           // 현재 대화 인덱스
    std::string _RecruitMessage;             // 영입 확정 메시지
    std::string _RefusalMessage;            // 거부 메시지

    // 타워 화살표 업데이트 (StageSelectScene과 동일)
    void UpdateTowerArrow(Panel* towerPanel, int currentFloor);
    
    // 선택 UI 업데이트
    void UpdateSelectionUI();
    
    // 동료 정보 패널 업데이트
    void UpdateCompanionInfoPanel(Panel* infoPanel);
    
    // ===== 대화 시스템 메서드 =====
    void LoadDialogueData();           // RecruitParty.csv에서 대화 로드
    void UpdateDialogueUI();         // 현재 대화를 화면에 표시

public:
    CompanionRecruitScene();
    ~CompanionRecruitScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
