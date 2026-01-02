#pragma once
#include "../UIScene.h"

// 게임 오버 Scene
class GameOverScene : public UIScene
{
private:
    bool _RetrySelected;

public:
    GameOverScene();
    ~GameOverScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
