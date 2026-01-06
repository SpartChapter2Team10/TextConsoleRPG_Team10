#pragma once
#include "../UIScene.h"

class MainMenuScene : public UIScene
{
private:
    bool _ExitRequested = false;  // ⭐ 게임 종료 플래그 추가

public:
    MainMenuScene();
    ~MainMenuScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
 
    // ⭐ 게임 종료 요청 확인
    inline bool IsExitRequested() const { return _ExitRequested; }
};
