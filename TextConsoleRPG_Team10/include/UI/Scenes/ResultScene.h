#pragma once
#include "../UIScene.h"

// 게임 결과 타입
enum class EResultType
{
    Victory,    // 승리
    Defeat      // 패배
};

// 게임 결과 Scene (승리/패배 통합)
class ResultScene : public UIScene
{
private:
    EResultType _ResultType;
    bool _InputComplete;

public:
    ResultScene();
    ~ResultScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;

    // 결과 타입 설정
    void SetResultType(EResultType type) { _ResultType = type; }
  EResultType GetResultType() const { return _ResultType; }
};
