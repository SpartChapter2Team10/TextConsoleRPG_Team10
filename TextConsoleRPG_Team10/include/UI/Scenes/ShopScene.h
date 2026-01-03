#pragma once
#include "../UIScene.h"

// 상점 Scene
class ShopScene : public UIScene
{
private:
    bool _StayInShop;

public:
  ShopScene();
    ~ShopScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;

    // 상점 메뉴
    void ShowShopMenu();
    void BuyItem();
    void SellItem();
};
