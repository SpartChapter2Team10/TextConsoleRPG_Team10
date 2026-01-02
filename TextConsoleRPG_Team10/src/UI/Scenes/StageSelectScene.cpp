#include "../../../include/UI/Scenes/StageSelectScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Unit/Player.h"

StageSelectScene::StageSelectScene()
    : UIScene("StageSelect")
    , _SelectedStage(0)
  , _StageSelected(false)
{
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Enter()
{
    _Drawer->Initialize();
    _Drawer->Activate();
    _IsActive = true;
    _StageSelected = false;

    Player* player = SceneManager::GetInstance()->GetPlayer();

    // 타이틀 패널
    Panel* titlePanel = _Drawer->CreatePanel("Title", 0, 0, 106, 5);
    titlePanel->SetBorder(true, 14);
    auto title = std::make_unique<TextRenderer>();
  title->AddLine("=== 스테이지 선택 ===");
    title->AddLine("도전할 층을 선택하세요!");
    title->SetTextColor(14);
    titlePanel->SetContentRenderer(std::move(title));

    // 스테이지 목록 패널
    Panel* stageListPanel = _Drawer->CreatePanel("StageList", 10, 10, 86, 40);
    stageListPanel->SetBorder(true, 11);
    auto stageList = std::make_unique<TextRenderer>();
    stageList->AddLine("=== 에레보스 타워 ===");
stageList->AddLine("");
    stageList->AddLine("[1] 1층 - 어둠의 입구 (추천 레벨: 1-2)");
    stageList->AddLine("      난이도: ★☆☆☆☆");
    stageList->AddLine("");
    stageList->AddLine("[2] 5층 - 뒤틀린 회랑 (추천 레벨: 5-6)");
    stageList->AddLine("    난이도: ★★★☆☆");
    stageList->AddLine("");
    stageList->AddLine("[3] 10층 - 최상층: 에테르노의 방 (추천 레벨: 10)");
    stageList->AddLine("      난이도: ★★★★★");
    stageList->AddLine("");
    stageList->AddLine("※ 현재는 자동 진행 모드입니다.");
 stageList->AddLine("※ 스테이지 선택 기능은 향후 업데이트 예정입니다.");
 stageList->SetTextColor(15);
    stageListPanel->SetContentRenderer(std::move(stageList));

    // 플레이어 정보 패널
  if (player)
    {
        Panel* playerPanel = _Drawer->CreatePanel("PlayerInfo", 10, 52, 40, 10);
     playerPanel->SetBorder(true, 10);
      auto playerInfo = std::make_unique<TextRenderer>();
        playerInfo->AddLine("=== 플레이어 정보 ===");
        playerInfo->AddLine("이름: " + player->GetName());
        playerInfo->AddLine("레벨: " + std::to_string(player->GetLevel()));
        playerInfo->AddLine("HP: " + std::to_string(player->GetCurrentHP()) + "/" + std::to_string(player->GetMaxHP()));
 playerInfo->SetTextColor(10);
        playerPanel->SetContentRenderer(std::move(playerInfo));
    }

    _Drawer->Render();
}

void StageSelectScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _Drawer->Deactivate();
    _IsActive = false;
}

void StageSelectScene::Update()
{
    if (!_StageSelected)
{
      HandleInput();
    }
    else
    {
   // 전투 Scene으로
        SceneManager::GetInstance()->ChangeScene(ESceneType::Battle);
    }
}

void StageSelectScene::Render()
{
    _Drawer->Render();
}

void StageSelectScene::HandleInput()
{
    // 현재는 아무 선택이나 전투로 진행 (자동 진행)
    _Input->GetInput("[Enter]를 눌러 다음 전투로...");
    
    _StageSelected = true;
}
