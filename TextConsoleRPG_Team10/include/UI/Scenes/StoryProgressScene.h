#pragma once
#include "../UIScene.h"

// 스토리 진행 Scene
class StoryProgressScene : public UIScene
{
private:
    int _CurrentStoryIndex;  // 현재 스토리 단계
    bool _TextComplete;  // 텍스트 출력 완료 여부

public:
    StoryProgressScene();
    ~StoryProgressScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
