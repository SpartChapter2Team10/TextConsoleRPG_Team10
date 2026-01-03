#pragma once
#include "../UIScene.h"

// 동료 영입 Scene (향후 확장용)
class CompanionRecruitScene : public UIScene
{
private:
    bool _RecruitComplete;

public:
    CompanionRecruitScene();
    ~CompanionRecruitScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
