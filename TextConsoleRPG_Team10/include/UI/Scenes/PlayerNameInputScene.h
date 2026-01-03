#pragma once
#include "../UIScene.h"
#include <string>

// 플레이어 이름 입력 Scene (완전 구현)
class PlayerNameInputScene : public UIScene
{
private:
    std::string _PlayerName;
    bool _InputComplete;

public:
    PlayerNameInputScene();
    ~PlayerNameInputScene() override;

    void Enter() override;
    void Exit() override;
    void Update() override;
    void Render() override;
    void HandleInput() override;

    inline const std::string& GetPlayerName() const { return _PlayerName; }
};
