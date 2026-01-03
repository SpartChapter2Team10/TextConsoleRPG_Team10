#include "../../../include/UI/Scenes/BattleScene.h"
#include "../../../include/UI/UIDrawer.h"

BattleScene::BattleScene()
    : UIScene("Battle")
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Enter()
{
    // TODO: 전투 UI 구현
    _IsActive = true;
}

void BattleScene::Exit()
{
    // TODO: UI 정리
    _IsActive = false;
}

void BattleScene::Update()
{
    // TODO: 전투 로직
}

void BattleScene::Render()
{
 // TODO: 화면 렌더링
}

void BattleScene::HandleInput()
{
    // TODO: 입력 처리
}
