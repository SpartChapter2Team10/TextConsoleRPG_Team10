#include "../../../include/UI/Scenes/StoryProgressScene.h"
#include "../../../include/UI/UIDrawer.h"

StoryProgressScene::StoryProgressScene()
    : UIScene("StoryProgress")
{
}

StoryProgressScene::~StoryProgressScene()
{
}

void StoryProgressScene::Enter()
{
    // TODO: 스토리 UI 구현
    _IsActive = true;
}

void StoryProgressScene::Exit()
{
  // TODO: UI 정리
    _IsActive = false;
}

void StoryProgressScene::Update()
{
    // TODO: 스토리 진행 로직
}

void StoryProgressScene::Render()
{
    // TODO: 화면 렌더링
}

void StoryProgressScene::HandleInput()
{
    // TODO: 입력 처리
}
