#pragma once
#include "../Singleton.h"
#include <memory>
#include <random>

static std::mt19937 gen(std::random_device{}());

class Player;

class GameManager : public Singleton<GameManager>
{
private:
    std::shared_ptr<Player> _MainPlayer;  // 레거시 호환용
    bool _IsGameOver = false;
    bool _IsRunning = false;

private:
    GameManager() = default;
    friend class Singleton<GameManager>;

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

public:
    // ===== Scene 기반 게임 시스템 =====
    void Initialize();   // 씬 등록
    void StartGame();    // 메인 루프 시작
    void EndGame();      // 게임 종료
    
  // ===== Getter/Setter =====
    inline bool IsRunning() const { return _IsRunning; }
    inline bool IsGameOver() const { return _IsGameOver; }
};
