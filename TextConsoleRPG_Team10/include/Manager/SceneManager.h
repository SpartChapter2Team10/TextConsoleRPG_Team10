#pragma once
#include "../Singleton.h"
#include <memory>
#include <map>
#include <string>

class UIScene;
class Player;

// Scene 타입 정의
enum class ESceneType
{
    MainMenu,           // 메인 메뉴
    PlayerNameInput,// 이름 입력
    CharacterSelect,    // 직업 선택
 StoryProgress,      // 스토리 진행
    StageSelect,  // 스테이지 선택
    Battle,      // 전투
    Shop,          // 상점
    CompanionRecruit,   // 동료 영입
    GameOver,   // 게임 오버
  Victory   // 승리
};

class SceneManager : public Singleton<SceneManager>
{
private:
 std::map<ESceneType, std::unique_ptr<UIScene>> _Scenes;
    UIScene* _CurrentScene;
    ESceneType _CurrentSceneType;
    Player* _GamePlayer;  // 게임 전역 플레이어

private:
    SceneManager();
    friend class Singleton<SceneManager>;

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    ~SceneManager();

    // Scene 등록
    void RegisterScene(ESceneType type, std::unique_ptr<UIScene> scene);

    // Scene 전환
    void ChangeScene(ESceneType type);

    // 업데이트 (게임 루프에서 호출)
    void Update();
    void Render();

    // 플레이어 설정
    void SetPlayer(Player* player) { _GamePlayer = player; }
    Player* GetPlayer() { return _GamePlayer; }

    // 현재 Scene
    UIScene* GetCurrentScene() { return _CurrentScene; }
    ESceneType GetCurrentSceneType() { return _CurrentSceneType; }
};
