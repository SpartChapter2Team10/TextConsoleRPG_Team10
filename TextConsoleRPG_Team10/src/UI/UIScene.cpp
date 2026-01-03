#include "../../include/UI/UIScene.h"
#include "../../include/UI/UIDrawer.h"
#include "../../include/Manager/InputManager.h"

UIScene::UIScene(const std::string& name)
    : _SceneName(name)
    , _IsActive(false)
{
    _Drawer = UIDrawer::GetInstance();
    _Input = InputManager::GetInstance();
}

UIScene::~UIScene()
{
}
