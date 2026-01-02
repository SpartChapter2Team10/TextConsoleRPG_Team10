#pragma once
#include "../UIScene.h"

// 승리 Scene
class VictoryScene : public UIScene
{
private:
    bool _ContinueToMenu;

public:
    VictoryScene();
    ~VictoryScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
