#include "../../../include/UI/Scenes/MainMenuScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/Manager/DataManager.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/GameManager.h"
#include <Windows.h>

MainMenuScene::MainMenuScene() 
    : UIScene("MainMenu")
    , _SelectedOption(0)
{
}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::Enter()
{
    _Drawer->ClearScreen();
    _Drawer->RemoveAllPanels();
    _Drawer->Activate();
    _IsActive = true;

    // 타이틀 패널
    Panel* titlePanel = _Drawer->CreatePanel("Title", 0, 0, 106, 20);
    titlePanel->SetBorder(true, 14);
    auto titleArt = std::make_unique<AsciiArtRenderer>();
    
    std::string uiPath = DataManager::GetInstance()->GetResourcePath("UI");
    if (DataManager::GetInstance()->FileExists(uiPath, "Title.txt"))
    {
   titleArt->LoadFromFile(uiPath, "Title.txt");
    }
    else
    {
  titleArt->LoadFromString(
            "╔══════════════════════════════════════════════════════════════════════════╗\n"
            "║       ║\n"
     "║           에레보스 타워: 뒤틀린 성의 종언      ║\n"
      "║        EREBOS TOWER: The End of Twisted Castle ║\n"
    "║     ║\n"
          "╚══════════════════════════════════════════════════════════════════════════╝\n"
   );
    }
    
    titleArt->SetAlignment(ArtAlignment::CENTER);
    titleArt->SetColor(14);
    titlePanel->SetContentRenderer(std::move(titleArt));

  // 메뉴 패널
    Panel* menuPanel = _Drawer->CreatePanel("Menu", 30, 25, 46, 20);
 menuPanel->SetBorder(true, 11);
    auto menu = std::make_unique<TextRenderer>();
    menu->AddLine("");
    menu->AddLine("     === MAIN MENU ===");
menu->AddLine("");
    menu->AddLine("    [1] 새로운 모험 시작");
    menu->AddLine("    [2] 게임 설명");
 menu->AddLine("    [3] 종료");
    menu->AddLine("");
    menu->AddLine("    선택하세요...");
    menu->SetTextColor(15);
    menuPanel->SetContentRenderer(std::move(menu));

    // 하단 정보
  Panel* infoPanel = _Drawer->CreatePanel("Info", 0, 55, 106, 5);
    infoPanel->SetBorder(true, 7);
    auto info = std::make_unique<TextRenderer>();
    info->AddLine("Team 10 | Text Console RPG | 2025");
    info->SetTextColor(7);
    infoPanel->SetContentRenderer(std::move(info));

    _Drawer->Render();
}

void MainMenuScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _IsActive = false;
}

void MainMenuScene::Update()
{
    if (_IsActive)
    {
   HandleInput();
    }
}

void MainMenuScene::Render()
{
    _Drawer->Render();
}

void MainMenuScene::HandleInput()
{
    int choice = _Input->GetIntInput("선택: ", 1, 3);

    switch (choice)
    {
    case 1:
        // 플레이어 이름 입력으로 전환
        SceneManager::GetInstance()->ChangeScene(ESceneType::PlayerNameInput);
  break;
    case 2:
        {
            // 게임 설명
            Panel* helpPanel = _Drawer->CreatePanel("Help", 20, 15, 66, 35);
      helpPanel->SetBorder(true, 10);
       auto help = std::make_unique<TextRenderer>();
     help->AddLine("=== 게임 설명 ===");
            help->AddLine("");
         help->AddLine("에레보스 타워의 최상층에 도달하여");
            help->AddLine("최종 보스를 물리치는 것이 목표입니다!");
            help->AddLine("");
            help->AddLine("기본 시스템:");
          help->AddLine("- 몬스터와 전투하여 경험치와 골드를 획득");
    help->AddLine("- 레벨업으로 능력치 강화");
          help->AddLine("- 상점에서 아이템 구매/판매");
            help->AddLine("- 레벨 10 도달 시 최종 보스 출현");
   help->AddLine("");
     help->AddLine("전투 시스템:");
            help->AddLine("- 자동 전투 (AI가 최적 행동 선택)");
            help->AddLine("- 아이템 자동 사용 (위험 시)");
      help->AddLine("- 턴제 전투");
            help->AddLine("");
            help->AddLine("[Enter]를 눌러 돌아가기...");
      helpPanel->SetContentRenderer(std::move(help));
    _Drawer->Render();
    
          _Input->GetInput("");
            _Drawer->RemovePanel("Help");
            _Drawer->Render();
        }
      break;
  case 3:
 // 게임 종료
  _IsActive = false;
      Exit();
        GameManager::GetInstance()->EndGame();
        break;
    }
}
