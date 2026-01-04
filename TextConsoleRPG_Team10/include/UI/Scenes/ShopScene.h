#pragma once
#include "../UIScene.h"

// 상점 Scene
class ShopScene : public UIScene
{
private:
    int _SelectedItemIndex;  // 선택한 상품 인덱스
    bool _IsBuyMode;        // true: 구매 모드, false: 판매 모드
    int _PlayerInventorySlot;  // 판매 시 선택한 인벤토리 슬롯

public:
    ShopScene();
    ~ShopScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
