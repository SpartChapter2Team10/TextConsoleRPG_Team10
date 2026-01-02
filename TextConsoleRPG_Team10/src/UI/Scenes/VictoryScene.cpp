#include "../../../include/UI/Scenes/VictoryScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/UI/StatRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/DataManager.h"
#include "../../../include/Unit/Player.h"
#include <Windows.h>

VictoryScene::VictoryScene()
    : UIScene("Victory")
    , _ContinueToMenu(false)
{
}

VictoryScene::~VictoryScene()
{
}

void VictoryScene::Enter()
{
    _Drawer->Initialize();
    _Drawer->Activate();
    _IsActive = true;
    _ContinueToMenu = false;

    Player* player = SceneManager::GetInstance()->GetPlayer();
    std::string playerName = player ? player->GetName() : "용사";

 // 배경을 밝게
    Panel* bgPanel = _Drawer->CreatePanel("Background", 0, 0, 106, 65);
    auto bg = std::make_unique<TextRenderer>();
    bg->SetTextColor(15);  // 밝은 색
 bgPanel->SetContentRenderer(std::move(bg));

    // Victory 아트 패널
    Panel* victoryPanel = _Drawer->CreatePanel("VictoryArt", 20, 5, 66, 20);
    victoryPanel->SetBorder(true, 14);  // 노란색
    auto victoryArt = std::make_unique<AsciiArtRenderer>();

    // Victory ASCII 아트 로드 시도
    std::string uiPath = DataManager::GetInstance()->GetResourcePath("UI");
    if (DataManager::GetInstance()->FileExists(uiPath, "Victory.txt"))
    {
        victoryArt->LoadFromFile(uiPath, "Victory.txt");
    }
    else
    {
        // 기본 Victory 텍스트
        victoryArt->LoadFromString(
            "╔═══════════════════════════════════════╗\n"
            "║         ║\n"
          "║      V I C T O R Y ! ! !             ║\n"
"║ ║\n"
       "╚═══════════════════════════════════════╝\n"
  );
    }
    victoryArt->SetAlignment(ArtAlignment::CENTER);
    victoryArt->SetColor(14);  // 노란색
    victoryPanel->SetContentRenderer(std::move(victoryArt));

  // 메시지 패널
    Panel* messagePanel = _Drawer->CreatePanel("Message", 10, 27, 86, 20);
 messagePanel->SetBorder(true, 10);
    auto message = std::make_unique<TextRenderer>();
    message->AddLine("축하합니다! " + playerName + "님!");
    message->AddLine("");
    message->AddLine("당신은 에레보스 타워의 정상에 도달했습니다!");
    message->AddLine("최종 보스 '에테르노'를 물리치고 세상에 평화를 되찾았습니다.");
    message->AddLine("");
    message->AddLine("긴 여정 동안 수많은 시련을 극복하고");
    message->AddLine("마침내 전설의 용사가 되었습니다!");
    message->AddLine("");
    message->AddLine("이 거대한 탑의 정상은 너무 고요합니다...");
    message->AddLine("수상할 정도로 말이죠...");
    message->SetTextColor(15);
    messagePanel->SetContentRenderer(std::move(message));

    // 최종 스탯 패널
    if (player)
    {
        Panel* statsPanel = _Drawer->CreatePanel("FinalStats", 30, 50, 46, 12);
        statsPanel->SetBorder(true, 11);
   auto stats = std::make_unique<StatRenderer>();
        stats->SetStat("=== 최종 스탯 ===", "");
        stats->SetStat("이름", player->GetName());
        stats->SetStat("최종 레벨", std::to_string(player->GetLevel()));
        stats->SetStat("HP", std::to_string(player->GetMaxHP()));
        stats->SetStat("ATK", std::to_string(player->GetAtk()));
        stats->SetStat("소지 골드", std::to_string(player->GetGold()) + " G");
    stats->SetKeyColor(11);
        stats->SetValueColor(14);
    statsPanel->SetContentRenderer(std::move(stats));
    }

    _Drawer->Render();
    Sleep(3000);  // 3초 대기
}

void VictoryScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _Drawer->Deactivate();
    _IsActive = false;
}

void VictoryScene::Update()
{
    HandleInput();
}

void VictoryScene::Render()
{
    _Drawer->Render();
}

void VictoryScene::HandleInput()
{
    _Input->GetInput("[Enter]를 눌러 메인 메뉴로...");
    
    // 메인 메뉴로
    SceneManager::GetInstance()->ChangeScene(ESceneType::MainMenu);
}
