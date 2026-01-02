#pragma once
#include "../UIScene.h"

// 직업 선택 Scene
class CharacterSelectScene : public UIScene
{
private:
    int _SelectedClass;
    bool _SelectionComplete;

public:
    CharacterSelectScene();
    ~CharacterSelectScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
