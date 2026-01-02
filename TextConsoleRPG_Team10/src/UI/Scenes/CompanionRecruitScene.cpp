#include "../../../include/UI/Scenes/CompanionRecruitScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/StatRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Unit/Player.h"

CompanionRecruitScene::CompanionRecruitScene()
    : UIScene("CompanionRecruit")
  , _RecruitComplete(false)
{
}

CompanionRecruitScene::~CompanionRecruitScene()
{
}

void CompanionRecruitScene::Enter()
{
    _Drawer->Initialize();
    _Drawer->Activate();
    _IsActive = true;
    _RecruitComplete = false;

    Player* player = SceneManager::GetInstance()->GetPlayer();

    // 타이틀 패널
Panel* titlePanel = _Drawer->CreatePanel("Title", 0, 0, 106, 5);
  titlePanel->SetBorder(true, 14);
    auto title = std::make_unique<TextRenderer>();
    title->AddLine("=== 동료 영입 ===");
    title->AddLine("함께 전투할 동료를 만났습니다!");
    title->SetTextColor(14);
    titlePanel->SetContentRenderer(std::move(title));

    // 동료 정보 패널 (좌측)
    Panel* companionPanel = _Drawer->CreatePanel("Companion", 10, 10, 45, 35);
    companionPanel->SetBorder(true, 10);
    auto companion = std::make_unique<TextRenderer>();
    companion->AddLine("=== 동료: 신비한 방랑자 ===");
    companion->AddLine("");
    companion->AddLine("타워 중턱에서 만난 의문의 인물.");
    companion->AddLine("강력한 힘을 지니고 있는 듯하다.");
    companion->AddLine("");
    companion->AddLine("   /\\_/\\");
    companion->AddLine("  ( o.o )");
    companion->AddLine("   > ^ <");
    companion->AddLine("");
 companion->AddLine("\"함께 가지 않겠나, 용사여?\"");
    companion->AddLine("");
    companion->AddLine("능력치:");
    companion->AddLine("- 공격력 보너스: +5");
    companion->AddLine("- HP 보너스: +20");
 companion->AddLine("");
    companion->AddLine("※ 동료 시스템은 개발 중입니다.");
    companion->SetTextColor(11);
    companionPanel->SetContentRenderer(std::move(companion));

 // 선택 패널 (우측)
    Panel* choicePanel = _Drawer->CreatePanel("Choice", 60, 10, 40, 20);
choicePanel->SetBorder(true, 7);
    auto choice = std::make_unique<TextRenderer>();
    choice->AddLine("동료로 영입하시겠습니까?");
    choice->AddLine("");
    choice->AddLine("[1] 영입한다");
    choice->AddLine(" - 전투에서 도움을 받습니다");
 choice->AddLine("    - 능력치가 상승합니다");
    choice->AddLine("");
    choice->AddLine("[2] 거절한다");
    choice->AddLine("  - 혼자 여정을 계속합니다");
    choice->AddLine("");
    choice->AddLine("※ 현재 버전에서는");
    choice->AddLine("  선택과 무관하게");
    choice->AddLine("  다음 전투로 진행됩니다.");
    choicePanel->SetContentRenderer(std::move(choice));

    // 플레이어 정보
    if (player)
    {
   Panel* playerPanel = _Drawer->CreatePanel("PlayerInfo", 60, 32, 40, 13);
  playerPanel->SetBorder(true, 10);
 auto playerInfo = std::make_unique<StatRenderer>();
   playerInfo->SetStat("=== 현재 상태 ===", "");
        playerInfo->SetStat("이름", player->GetName());
playerInfo->SetStat("레벨", std::to_string(player->GetLevel()));
        playerInfo->SetStat("HP", std::to_string(player->GetCurrentHP()) + "/" + std::to_string(player->GetMaxHP()));
        playerInfo->SetStat("ATK", std::to_string(player->GetAtk()));
        playerInfo->SetKeyColor(11);
        playerInfo->SetValueColor(15);
        playerPanel->SetContentRenderer(std::move(playerInfo));
    }

    _Drawer->Render();
}

void CompanionRecruitScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _Drawer->Deactivate();
    _IsActive = false;
}

void CompanionRecruitScene::Update()
{
 if (!_RecruitComplete)
    {
   HandleInput();
    }
    else
    {
     // 다음 전투로
        SceneManager::GetInstance()->ChangeScene(ESceneType::Battle);
    }
}

void CompanionRecruitScene::Render()
{
  _Drawer->Render();
}

void CompanionRecruitScene::HandleInput()
{
    int choice = _Input->GetIntInput("선택: ", 1, 2);

    Player* player = SceneManager::GetInstance()->GetPlayer();

    if (choice == 1 && player)
    {
        // 동료 영입 (TODO: 향후 동료 시스템 구현 시 활성화)
        player->ModifyMaxHP(20);
        player->ModifyHP(20);
     player->ModifyAtk(5);

        Panel* resultPanel = _Drawer->CreatePanel("Result", 25, 25, 56, 8);
 resultPanel->SetBorder(true, 10);
 auto result = std::make_unique<TextRenderer>();
    result->AddLine("동료를 영입했습니다!");
     result->AddLine("능력치가 상승했습니다!");
        result->AddLine("(HP +20, ATK +5)");
        result->SetTextColor(10);
    resultPanel->SetContentRenderer(std::move(result));
        
  _Drawer->Render();
        Sleep(2000);
    }
    else
    {
        Panel* resultPanel = _Drawer->CreatePanel("Result", 25, 25, 56, 6);
        resultPanel->SetBorder(true, 7);
    auto result = std::make_unique<TextRenderer>();
     result->AddLine("혼자서 여정을 계속합니다.");
        result->SetTextColor(7);
        resultPanel->SetContentRenderer(std::move(result));
        
        _Drawer->Render();
 Sleep(1500);
    }

    _RecruitComplete = true;
}
