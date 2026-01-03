#include "../../../include/UI/Scenes/CharacterSelectScene.h"
#include "../../../include/UI/UIDrawer.h"

CharacterSelectScene::CharacterSelectScene()
    : UIScene("CharacterSelect")
{
}

CharacterSelectScene::~CharacterSelectScene()
{
}

void CharacterSelectScene::Enter()
{
    // TODO: 직업 선택 UI 구현
    _IsActive = true;
}

void CharacterSelectScene::Exit()
{
    // TODO: UI 정리
    _IsActive = false;
}

void CharacterSelectScene::Update()
{
    // TODO: 직업 선택 로직
}

void CharacterSelectScene::Render()
{
    // TODO: 화면 렌더링
}

void CharacterSelectScene::HandleInput()
{
    // TODO: 입력 처리
}
