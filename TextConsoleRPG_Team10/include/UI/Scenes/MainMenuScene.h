#pragma once
#include "../UIScene.h"

// 메인 메뉴 Scene (완전 구현)
class MainMenuScene : public UIScene
{
private:
    int _SelectedOption;

public:
    MainMenuScene();
    ~MainMenuScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
