#include "../../include/Manager/SceneManager.h"
#include "../../include/UI/UIScene.h"
#include "../../include/Manager/PrintManager.h"

SceneManager::SceneManager()
    : _CurrentScene(nullptr)
    , _CurrentSceneType(ESceneType::MainMenu)
    , _GamePlayer(nullptr)
{
}

SceneManager::~SceneManager()
{
    if (_CurrentScene && _CurrentScene->IsActive())
    {
        _CurrentScene->Exit();
    }
}

void SceneManager::RegisterScene(ESceneType type, std::unique_ptr<UIScene> scene)
{
    _Scenes[type] = std::move(scene);
}

void SceneManager::ChangeScene(ESceneType type)
{
    // 이전 Scene 종료
    if (_CurrentScene && _CurrentScene->IsActive())
    {
        _CurrentScene->Exit();
    }

    // Scene 존재 확인
    auto it = _Scenes.find(type);
    if (it == _Scenes.end())
    {
        PrintManager::GetInstance()->PrintLogLine(
            "SceneManager: Scene not found!",
            ELogImportance::WARNING
        );
        return;
    }

    // 새 Scene 시작
    _CurrentScene = it->second.get();
    _CurrentSceneType = type;
    _CurrentScene->Enter();
}

void SceneManager::Update()
{
    if (_CurrentScene && _CurrentScene->IsActive())
    {
        _CurrentScene->Update();
    }
}

void SceneManager::Render()
{
    if (_CurrentScene && _CurrentScene->IsActive())
    {
        _CurrentScene->Render();
    }
}
