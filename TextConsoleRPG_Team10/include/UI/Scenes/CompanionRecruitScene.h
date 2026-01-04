#pragma once
#include "../UIScene.h"
#include <string>

// 동료 영입 Scene
class CompanionRecruitScene : public UIScene
{
private:
    std::string _CompanionName;  // 영입할 동료 이름 (임시)
    std::string _InputName;      // 사용자가 입력한 이름
    bool _RecruitAccepted;       // 영입 수락 여부
    int _SelectedOption; // 0: 영입, 1: 거부

public:
    CompanionRecruitScene();
    ~CompanionRecruitScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
