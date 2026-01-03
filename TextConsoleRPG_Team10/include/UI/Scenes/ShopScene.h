#pragma once
#include "../UIScene.h"

// 상점 Scene (미구현 - 확장용 껍데기)
class ShopScene : public UIScene
{
public:
    ShopScene();
    ~ShopScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
