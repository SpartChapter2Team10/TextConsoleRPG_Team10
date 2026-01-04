#pragma once
#include "../UIScene.h"

// 직업 선택 Scene
class CharacterSelectScene : public UIScene
{
private:
    int _CurrentSelection;  // 현재 선택 중인 직업 인덱스
    int _TotalClasses;    // 전체 직업 수

public:
    CharacterSelectScene();
    ~CharacterSelectScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
