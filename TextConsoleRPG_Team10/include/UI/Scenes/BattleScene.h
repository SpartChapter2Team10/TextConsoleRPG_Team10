#pragma once
#include "../UIScene.h"

// 전투 Scene (미구현 - 확장용 껍데기)
class BattleScene : public UIScene
{
public:
    BattleScene();
    ~BattleScene() override;

  void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
