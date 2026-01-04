#pragma once
#include "../UIScene.h"

// 전투 Scene
class BattleScene : public UIScene
{
private:
    int _CurrentTurn;  // 현재 턴 수
    bool _PlayerTurn;     // true: 플레이어 턴, false: 적 턴
    int _SelectedAction;  // 선택한 행동 (0: 공격, 1: 스킬, 2: 아이템)
    int _SelectedTarget;   // 선택한 대상 인덱스
    bool _BattleEnd;   // 전투 종료 여부

public:
    BattleScene();
    ~BattleScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;
};
