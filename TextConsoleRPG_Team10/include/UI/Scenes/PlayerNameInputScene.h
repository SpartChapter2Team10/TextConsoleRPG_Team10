#pragma once
#include "../UIScene.h"
#include <string>

// 플레이어 이름 입력 Scene
class PlayerNameInputScene : public UIScene
{
private:
    std::string _PlayerName;

    // 확인 화면 표시
    void ShowConfirmation();

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
