#include "../../../include/UI/Scenes/PlayerNameInputScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Unit/Player.h"

PlayerNameInputScene::PlayerNameInputScene()
    : UIScene("PlayerNameInput")
    , _InputComplete(false)
{
}

PlayerNameInputScene::~PlayerNameInputScene()
{
}

void PlayerNameInputScene::Enter()
{
    _Drawer->Initialize();
    _Drawer->Activate();
    _IsActive = true;
    _InputComplete = false;

    // 배경 패널
    Panel* bgPanel = _Drawer->CreatePanel("Background", 0, 0, 106, 65);
    auto bg = std::make_unique<TextRenderer>();
    bg->AddLine("");
  bg->SetTextColor(7);
  bgPanel->SetContentRenderer(std::move(bg));

    // 입력 안내 패널
    Panel* promptPanel = _Drawer->CreatePanel("Prompt", 20, 20, 66, 15);
    promptPanel->SetBorder(true, 10);
  auto prompt = std::make_unique<TextRenderer>();
    prompt->AddLine("=== 캐릭터 생성 ===");
  prompt->AddLine("");
    prompt->AddLine("용감한 모험가여, 당신의 이름은 무엇입니까?");
    prompt->AddLine("");
    prompt->AddLine("이름을 입력하세요:");
    prompt->AddLine("(빈 칸으로 입력 시 'Player'로 설정됩니다)");
    prompt->SetTextColor(14);
    promptPanel->SetContentRenderer(std::move(prompt));

    _Drawer->Render();
}

void PlayerNameInputScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _Drawer->Deactivate();
    _IsActive = false;
}

void PlayerNameInputScene::Update()
{
    if (!_InputComplete)
 {
        HandleInput();
    }
    else
    {
        // 플레이어 생성 완료 메시지
        Panel* completePanel = _Drawer->CreatePanel("Complete", 30, 30, 46, 8);
 completePanel->SetBorder(true, 10);
    auto complete = std::make_unique<TextRenderer>();
        complete->AddLine("캐릭터 생성 완료!");
        complete->AddLine("");
        complete->AddLine("이름: " + _PlayerName);
        complete->AddLine("");
        complete->AddLine("[Enter]를 눌러 계속...");
        complete->SetTextColor(14);
        completePanel->SetContentRenderer(std::move(complete));
        
 _Drawer->Render();
        
        _Input->GetInput("");  // 대기
        
        // 플레이어 생성
    Player* newPlayer = new Player(_PlayerName);
        SceneManager::GetInstance()->SetPlayer(newPlayer);
        
        // 다음 Scene으로 (스토리 또는 전투)
        SceneManager::GetInstance()->ChangeScene(ESceneType::StoryProgress);
    }
}

void PlayerNameInputScene::Render()
{
    _Drawer->Render();
}

void PlayerNameInputScene::HandleInput()
{
    _PlayerName = _Input->GetInput("이름: ");
    
  if (_PlayerName.empty())
    {
        _PlayerName = "Player";
 }
    
    _InputComplete = true;
}
