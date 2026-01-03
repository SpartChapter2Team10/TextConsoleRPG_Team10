#include "../../../include/UI/Scenes/ShopScene.h"
#include "../../../include/UI/UIDrawer.h"

ShopScene::ShopScene()
    : UIScene("Shop")
{
}

ShopScene::~ShopScene()
{
}

void ShopScene::Enter()
{
    // TODO: 상점 UI 구현
    _IsActive = true;
}

void ShopScene::Exit()
{
    // TODO: UI 정리
    _IsActive = false;
}

void ShopScene::Update()
{
    // TODO: 상점 로직
}

void ShopScene::Render()
{
    // TODO: 화면 렌더링
}

void ShopScene::HandleInput()
{
    // TODO: 입력 처리
}
