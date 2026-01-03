#include "../../../include/UI/Scenes/StoryProgressScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/PrintManager.h"
#include "../../../include/Unit/Player.h"
#include <Windows.h>

StoryProgressScene::StoryProgressScene()
    : UIScene("StoryProgress")
    , _CurrentStoryIndex(0)
    , _StoryComplete(false)
{
}

StoryProgressScene::~StoryProgressScene()
{
}

void StoryProgressScene::Enter()
{
    _Drawer->Initialize();
    _Drawer->Activate();
    _IsActive = true;
    _CurrentStoryIndex = 0;
    _StoryComplete = false;

    Player* player = SceneManager::GetInstance()->GetPlayer();
    std::string playerName = player ? player->GetName() : "용사";

  // 스토리 패널
    Panel* storyPanel = _Drawer->CreatePanel("Story", 10, 10, 86, 45);
    storyPanel->SetBorder(true, 11);
    auto story = std::make_unique<TextRenderer>();
    
    story->AddLine("=== 프롤로그 ===");
    story->AddLine("");
    story->AddLine("어둠이 세상을 집어삼킨 지 오래...");
 story->AddLine("");
    story->AddLine("전설 속 '에레보스 타워'가 나타났다.");
    story->AddLine("그 탑의 정상에는 세상을 구원할 힘이 잠들어 있다고 한다.");
    story->AddLine("");
    story->AddLine("수많은 용사들이 도전했지만, 아무도 돌아오지 못했다.");
    story->AddLine("");
    story->AddLine("그러나 " + playerName + "은(는) 다짐했다.");
    story->AddLine("\"나는 반드시 탑의 정상에 도달하리라!\"");
    story->AddLine("");
    story->AddLine("이렇게 " + playerName + "의 여정이 시작되었다...");
story->AddLine("");
    story->AddLine("");
    story->AddLine("[Enter]를 눌러 계속...");
    
    story->SetTextColor(15);
    story->SetAutoScroll(false);
    storyPanel->SetContentRenderer(std::move(story));

    _Drawer->Render();
}

void StoryProgressScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _Drawer->Deactivate();
    _IsActive = false;
}

void StoryProgressScene::Update()
{
    if (!_StoryComplete)
    {
     HandleInput();
    }
}

void StoryProgressScene::Render()
{
    _Drawer->Render();
}

void StoryProgressScene::HandleInput()
{
    _Input->GetInput("");  // 아무 키 대기
    
    _StoryComplete = true;
    
    // 전투 Scene으로 전환
    SceneManager::GetInstance()->ChangeScene(ESceneType::Battle);
}
