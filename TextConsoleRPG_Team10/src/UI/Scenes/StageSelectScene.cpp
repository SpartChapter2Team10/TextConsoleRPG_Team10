#include "../../../include/UI/Scenes/StageSelectScene.h"
#include "../../../include/UI/UIDrawer.h"

StageSelectScene::StageSelectScene()
  : UIScene("StageSelect")
{
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Enter()
{
    // TODO: 스테이지 선택 UI 구현
    _IsActive = true;
}

void StageSelectScene::Exit()
{
    // TODO: UI 정리
    _IsActive = false;
}

void StageSelectScene::Update()
{
    // TODO: 스테이지 선택 로직
}

void StageSelectScene::Render()
{
    // TODO: 화면 렌더링
}

void StageSelectScene::HandleInput()
{
    // TODO: 입력 처리
}
