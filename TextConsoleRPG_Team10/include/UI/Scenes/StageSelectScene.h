#pragma once
#include "../UIScene.h"
#include <vector>
#include <string>

// 노드 타입 (스테이지에서 선택 가능한 분기)
enum class ENodeType
{
    Battle,      // 전투
    Shop,        // 상점
    Companion,   // 동료 영입
    Event,       // 랜덤 이벤트
    Boss         // 보스 전투
};

// 스테이지 노드 정보
struct StageNode
{
    ENodeType Type;
    std::string Name;
    int Index;
};

// 스테이지 선택 Scene
class StageSelectScene : public UIScene
{
private:
    std::vector<StageNode> _CurrentNodes;  // 현재 선택 가능한 노드들
    int _SelectedNodeIndex;    // 선택한 노드 인덱스
    int _CurrentStageLevel;          // 현재 스테이지 레벨

public:
    StageSelectScene();
    ~StageSelectScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
