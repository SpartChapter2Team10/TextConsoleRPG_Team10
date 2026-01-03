#pragma once
#include "../UIScene.h"

// 동료 영입 Scene (미구현 - 확장용 껍데기)
class CompanionRecruitScene : public UIScene
{
public:
    CompanionRecruitScene();
    ~CompanionRecruitScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
