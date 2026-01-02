#pragma once
#include "../UIScene.h"
#include <memory>

class Player;
class IMonster;

// 전투 Scene
class BattleScene : public UIScene
{
private:
    Player* _Player;
    std::unique_ptr<IMonster> _Monster;
    bool _BattleEnd;
    bool _PlayerWin;
    int _TurnCount;

public:
    BattleScene();
    ~BattleScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;

    // 전투 로직
    void ProcessPlayerTurn();
    void ProcessMonsterTurn();
    void UpdateUI();
    void CheckBattleEnd();

    // 전투 결과
  inline bool IsBattleEnd() const { return _BattleEnd; }
  inline bool IsPlayerWin() const { return _PlayerWin; }
};
