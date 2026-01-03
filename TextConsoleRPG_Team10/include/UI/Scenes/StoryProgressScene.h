#pragma once
#include "../UIScene.h"

// 스토리 진행 Scene (미구현 - 확장용 껍데기)
class StoryProgressScene : public UIScene
{
public:
  StoryProgressScene();
    ~StoryProgressScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
