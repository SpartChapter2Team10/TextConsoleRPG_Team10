#pragma once
#include "../UIScene.h"

// 스토리 진행 Scene
class StoryProgressScene : public UIScene
{
private:
    int _CurrentStoryIndex;
    bool _StoryComplete;

public:
    StoryProgressScene();
    ~StoryProgressScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
