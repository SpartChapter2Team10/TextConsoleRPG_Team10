#pragma once
#include "../UIScene.h"

// 직업 선택 Scene (미구현 - 확장용 껍데기)
class CharacterSelectScene : public UIScene
{
public:
    CharacterSelectScene();
    ~CharacterSelectScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
