#include "../../../include/UI/Scenes/CompanionRecruitScene.h"
#include "../../../include/UI/UIDrawer.h"

CompanionRecruitScene::CompanionRecruitScene()
    : UIScene("CompanionRecruit")
{
}

CompanionRecruitScene::~CompanionRecruitScene()
{
}

void CompanionRecruitScene::Enter()
{
    // TODO: 동료 영입 UI 구현
    _IsActive = true;
}

void CompanionRecruitScene::Exit()
{
    // TODO: UI 정리
    _IsActive = false;
}

void CompanionRecruitScene::Update()
{
 // TODO: 동료 영입 로직
}

void CompanionRecruitScene::Render()
{
    // TODO: 화면 렌더링
}

void CompanionRecruitScene::HandleInput()
{
    // TODO: 입력 처리
}
