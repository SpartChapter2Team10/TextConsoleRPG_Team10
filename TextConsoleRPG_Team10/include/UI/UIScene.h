#pragma once
#include <string>

class UIDrawer;
class InputManager;
class Player;

// UI Scene 베이스 클래스
// 모든 화면(메인메뉴, 전투, 상점 등)의 부모 클래스
class UIScene
{
protected:
    UIDrawer* _Drawer;
    InputManager* _Input;
    bool _IsActive;
    std::string _SceneName;

public:
    UIScene(const std::string& name);
    virtual ~UIScene();

    // Scene 생명주기 (반드시 구현)
    virtual void Enter() = 0;     // Scene 시작 시
    virtual void Exit() = 0;      // Scene 종료 시
    virtual void Update() = 0;    // 매 프레임
    virtual void Render() = 0;    // 화면 출력

    // 입력 처리 (선택적)
    virtual void HandleInput() {}

    // Getter
    inline bool IsActive() const { return _IsActive; }
    inline void SetActive(bool active) { _IsActive = active; }
    inline const std::string& GetName() const { return _SceneName; }
};
