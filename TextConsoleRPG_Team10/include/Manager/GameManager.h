#pragma once
#include "../Singleton.h"

class Player;

class GameManager : public Singleton<GameManager>
{
private:
    Player* _MainPlayer;

public:
    void StartGame();
    void RunMainLoop();
};
