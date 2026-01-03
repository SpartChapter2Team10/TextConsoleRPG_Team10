#pragma once
#include "../UIScene.h"

// 스테이지 선택 Scene (미구현 - 확장용 껍데기)
class StageSelectScene : public UIScene
{
public:
    StageSelectScene();
    ~StageSelectScene() override;

    void Enter() override;
    void Exit() override;
  void Update() override;
    void Render() override;
    void HandleInput() override;
};
