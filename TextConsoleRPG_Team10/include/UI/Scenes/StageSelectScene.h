#pragma once
#include "../UIScene.h"

// 스테이지 선택 Scene (향후 확장용)
class StageSelectScene : public UIScene
{
private:
  int _SelectedStage;
    bool _StageSelected;

public:
    StageSelectScene();
~StageSelectScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
