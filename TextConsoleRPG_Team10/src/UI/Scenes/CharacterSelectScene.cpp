#include "../../../include/UI/Scenes/CharacterSelectScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Unit/Player.h"

CharacterSelectScene::CharacterSelectScene()
    : UIScene("CharacterSelect")
    , _SelectedClass(0)
  , _SelectionComplete(false)
{
}

CharacterSelectScene::~CharacterSelectScene()
{
}

void CharacterSelectScene::Enter()
{
    _Drawer->Initialize();
  _Drawer->Activate();
    _IsActive = true;
    _SelectionComplete = false;

    Player* player = SceneManager::GetInstance()->GetPlayer();
 std::string playerName = player ? player->GetName() : "용사";

    // 타이틀 패널
    Panel* titlePanel = _Drawer->CreatePanel("Title", 0, 0, 106, 5);
    titlePanel->SetBorder(true, 14);
    auto title = std::make_unique<TextRenderer>();
    title->AddLine("=== 직업 선택 ===");
    title->AddLine(playerName + "님, 직업을 선택하세요!");
    title->SetTextColor(14);
    titlePanel->SetContentRenderer(std::move(title));

    // 직업 정보 패널 (좌측 - 전사)
    Panel* warriorPanel = _Drawer->CreatePanel("Warrior", 5, 10, 30, 30);
    warriorPanel->SetBorder(true, 12);
    auto warrior = std::make_unique<TextRenderer>();
    warrior->AddLine("[1] 전사 (Warrior)");
    warrior->AddLine("");
    warrior->AddLine("강인한 체력과 높은 방어력을");
    warrior->AddLine("가진 근접 전투의 달인");
    warrior->AddLine("");
    warrior->AddLine("초기 능력치:");
    warrior->AddLine("HP: +50");
    warrior->AddLine("ATK: +5");
    warrior->AddLine("");
    warrior->AddLine("특성:");
    warrior->AddLine("- 높은 생존력");
    warrior->AddLine("- 안정적인 전투");
    warrior->SetTextColor(12);
    warriorPanel->SetContentRenderer(std::move(warrior));

    // 직업 정보 패널 (중앙 - 마법사)
 Panel* magePanel = _Drawer->CreatePanel("Mage", 38, 10, 30, 30);
    magePanel->SetBorder(true, 11);
    auto mage = std::make_unique<TextRenderer>();
    mage->AddLine("[2] 마법사 (Mage)");
    mage->AddLine("");
    mage->AddLine("강력한 마법 공격으로");
    mage->AddLine("적을 압도하는 원거리 딜러");
    mage->AddLine("");
    mage->AddLine("초기 능력치:");
    mage->AddLine("HP: +20");
    mage->AddLine("ATK: +15");
    mage->AddLine("");
  mage->AddLine("특성:");
    mage->AddLine("- 높은 화력");
    mage->AddLine("- 빠른 전투");
    mage->SetTextColor(11);
    magePanel->SetContentRenderer(std::move(mage));

    // 직업 정보 패널 (우측 - 도적)
  Panel* roguePanel = _Drawer->CreatePanel("Rogue", 71, 10, 30, 30);
    roguePanel->SetBorder(true, 10);
    auto rogue = std::make_unique<TextRenderer>();
  rogue->AddLine("[3] 도적 (Rogue)");
    rogue->AddLine("");
    rogue->AddLine("빠른 속도와 치명타로");
    rogue->AddLine("적을 기습하는 암살자");
    rogue->AddLine("");
    rogue->AddLine("초기 능력치:");
  rogue->AddLine("HP: +30");
 rogue->AddLine("ATK: +10");
    rogue->AddLine("");
    rogue->AddLine("특성:");
    rogue->AddLine("- 균형잡힌 능력");
    rogue->AddLine("- 높은 골드 획득");
    rogue->SetTextColor(10);
    roguePanel->SetContentRenderer(std::move(rogue));

    // 안내 메시지
    Panel* promptPanel = _Drawer->CreatePanel("Prompt", 20, 45, 66, 10);
    promptPanel->SetBorder(true, 7);
  auto prompt = std::make_unique<TextRenderer>();
    prompt->AddLine("직업을 선택하세요 (1-3):");
    prompt->AddLine("");
    prompt->AddLine("※ 현재는 기본 직업만 제공됩니다.");
    prompt->AddLine("※ 향후 업데이트에서 직업별 스킬이 추가될 예정입니다.");
    promptPanel->SetContentRenderer(std::move(prompt));

    _Drawer->Render();
}

void CharacterSelectScene::Exit()
{
    _Drawer->RemoveAllPanels();
 _Drawer->Deactivate();
    _IsActive = false;
}

void CharacterSelectScene::Update()
{
    if (!_SelectionComplete)
    {
 HandleInput();
    }
  else
    {
        // 스토리 Scene으로 전환
        SceneManager::GetInstance()->ChangeScene(ESceneType::StoryProgress);
    }
}

void CharacterSelectScene::Render()
{
    _Drawer->Render();
}

void CharacterSelectScene::HandleInput()
{
    _SelectedClass = _Input->GetIntInput("선택: ", 1, 3);

    Player* player = SceneManager::GetInstance()->GetPlayer();
    if (!player) return;

    // 직업별 초기 스탯 조정 (TODO: 향후 직업 시스템으로 대체)
    switch (_SelectedClass)
    {
    case 1: // 전사
      player->ModifyMaxHP(50);
        player->ModifyHP(50);
        player->ModifyAtk(5);
 break;
  case 2: // 마법사
    player->ModifyMaxHP(20);
        player->ModifyHP(20);
        player->ModifyAtk(15);
  break;
    case 3: // 도적
 player->ModifyMaxHP(30);
        player->ModifyHP(30);
        player->ModifyAtk(10);
   break;
    }

    _SelectionComplete = true;
}
