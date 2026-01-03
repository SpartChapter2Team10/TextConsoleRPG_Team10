#include "../../../include/UI/Scenes/GameOverScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/DataManager.h"
#include "../../../include/Unit/Player.h"
#include <Windows.h>

GameOverScene::GameOverScene()
    : UIScene("GameOver")
    , _RetrySelected(false)
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Enter()
{
    _Drawer->Initialize();
_Drawer->Activate();
    _IsActive = true;
    _RetrySelected = false;

    Player* player = SceneManager::GetInstance()->GetPlayer();
    std::string playerName = player ? player->GetName() : "용사";

    // 배경을 어둡게
    Panel* bgPanel = _Drawer->CreatePanel("Background", 0, 0, 106, 65);
    auto bg = std::make_unique<TextRenderer>();
    bg->SetTextColor(8);  // 어두운 회색
    bgPanel->SetContentRenderer(std::move(bg));

    // Game Over 아트 패널
    Panel* gameOverPanel = _Drawer->CreatePanel("GameOverArt", 20, 10, 66, 20);
    gameOverPanel->SetBorder(true, 4);  // 빨간색
    auto gameOverArt = std::make_unique<AsciiArtRenderer>();

    // Game Over ASCII 아트 로드 시도
    std::string uiPath = DataManager::GetInstance()->GetResourcePath("UI");
    if (DataManager::GetInstance()->FileExists(uiPath, "GameOver.txt"))
    {
 gameOverArt->LoadFromFile(uiPath, "GameOver.txt");
    }
    else
    {
        // 기본 Game Over 텍스트
        gameOverArt->LoadFromString(
   "╔═══════════════════════════════════════╗\n"
          "║     ║\n"
       "║       G A M E   O V E R             ║\n"
            "║       ║\n"
        "╚═══════════════════════════════════════╝\n"
        );
  }
    gameOverArt->SetAlignment(ArtAlignment::CENTER);
    gameOverArt->SetColor(12);  // 빨간색
    gameOverPanel->SetContentRenderer(std::move(gameOverArt));

    // 메시지 패널
    Panel* messagePanel = _Drawer->CreatePanel("Message", 25, 32, 56, 15);
    messagePanel->SetBorder(true, 7);
    auto message = std::make_unique<TextRenderer>();
    message->AddLine(playerName + "의 여정은 여기서 끝났습니다...");
    message->AddLine("");
    message->AddLine("에레보스 타워의 어둠이 다시 한번 승리했습니다.");
    message->AddLine("");
    message->AddLine("하지만 희망은 아직 남아있습니다.");
    message->AddLine("다시 도전하시겠습니까?");
    message->AddLine("");
    message->AddLine("[1] 다시 도전");
    message->AddLine("[2] 메인 메뉴로");
  message->SetTextColor(7);
    messagePanel->SetContentRenderer(std::move(message));

    _Drawer->Render();
    Sleep(2000);  // 2초 대기
}

void GameOverScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _Drawer->Deactivate();
    _IsActive = false;
}

void GameOverScene::Update()
{
    HandleInput();
}

void GameOverScene::Render()
{
    _Drawer->Render();
}

void GameOverScene::HandleInput()
{
    int choice = _Input->GetIntInput("선택: ", 1, 2);

    if (choice == 1)
    {
     // 다시 도전 - 플레이어 이름 입력으로
     SceneManager::GetInstance()->ChangeScene(ESceneType::PlayerNameInput);
    }
    else
    {
   // 메인 메뉴로
        SceneManager::GetInstance()->ChangeScene(ESceneType::MainMenu);
    }
}
