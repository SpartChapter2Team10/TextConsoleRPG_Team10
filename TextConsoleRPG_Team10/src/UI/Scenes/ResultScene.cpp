#include "../../../include/UI/Scenes/ResultScene.h"
#include "../../../include/UI/UIDrawer.h"

ResultScene::ResultScene()
    : UIScene("Result")
    , _ResultType(EResultType::Victory)
    , _InputComplete(false)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Enter()
{
    // TODO: 승리/패배에 따른 UI 구성
    _IsActive = true;
    _InputComplete = false;
}

void ResultScene::Exit()
{
    // TODO: UI 정리
    _IsActive = false;
}

void ResultScene::Update()
{
    // TODO: 결과 화면 로직
    // - 승리 시: 보상 표시, 메인 메뉴 복귀
    // - 패배 시: 재도전 또는 메인 메뉴
}

void ResultScene::Render()
{
    // TODO: 화면 렌더링
}

void ResultScene::HandleInput()
{
  // TODO: 입력 처리
    // - 메인 메뉴로
    // - 재도전 (패배 시)
}
