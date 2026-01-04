#include "../../../include/UI/Scenes/BattleScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/GameManager.h"
#include "../../../include/Common/TextColor.h"

BattleScene::BattleScene()
    : UIScene("Battle")
    , _CurrentTurn(1)
    , _PlayerTurn(true)
    , _SelectedAction(0)
    , _SelectedTarget(0)
    , _BattleEnd(false)
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Enter()
{
    _Drawer->ClearScreen();
    _Drawer->RemoveAllPanels();
    _Drawer->Activate();
    _IsActive = true;

    // =============================================================================
    // 패널 레이아웃 (150x45 화면 기준)
    // =============================================================================

    // ===== 전투 정보 패널 (상단) =====
    Panel* infoPanel = _Drawer->CreatePanel("BattleInfo", 10, 1, 130, 6);
    infoPanel->SetBorder(true, ETextColor::CYAN);

    auto infoText = std::make_unique<TextRenderer>();
    infoText->AddLine("");
    infoText->AddLineWithColor("        [스테이지 명] - [몬스터 이름]",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    infoText->AddLineWithColor("   남은 HP/총 HP",
        MakeColorAttribute(ETextColor::WHITE, EBackgroundColor::BLACK));
    infoText->AddLine("");

    infoPanel->SetContentRenderer(std::move(infoText));
    infoPanel->Redraw();

    // TODO: 전투 정보 동적 업데이트
    // BattleManager 또는 GameManager에서 현재 전투 정보를 가져와 표시합니다.

    // ===== 아군 파티 패널 (왼쪽 4개) =====
    Panel* party1 = _Drawer->CreatePanel("Party1", 10, 8, 25, 15);
    party1->SetBorder(true, ETextColor::GREEN);
    auto party1Text = std::make_unique<TextRenderer>();
    party1Text->AddLine("");
    party1Text->AddLine("   플레이 파티 1");
    party1Text->AddLine("");
    party1->SetContentRenderer(std::move(party1Text));
    party1->Redraw();

    Panel* party2 = _Drawer->CreatePanel("Party2", 38, 8, 25, 15);
    party2->SetBorder(true, ETextColor::GREEN);
    auto party2Text = std::make_unique<TextRenderer>();
    party2Text->AddLine("");
    party2Text->AddLine("   플레이 파티 2");
    party2Text->AddLine("");
    party2->SetContentRenderer(std::move(party2Text));
    party2->Redraw();

    Panel* party3 = _Drawer->CreatePanel("Party3", 10, 24, 25, 15);
    party3->SetBorder(true, ETextColor::GREEN);
    auto party3Text = std::make_unique<TextRenderer>();
    party3Text->AddLine("");
    party3Text->AddLine("   플레이 파티 3");
    party3Text->AddLine("");
    party3->SetContentRenderer(std::move(party3Text));
    party3->Redraw();

    Panel* party4 = _Drawer->CreatePanel("Party4", 38, 24, 25, 15);
    party4->SetBorder(true, ETextColor::GREEN);
    auto party4Text = std::make_unique<TextRenderer>();
    party4Text->AddLine("");
    party4Text->AddLine("   플레이 파티 4");
    party4Text->AddLine("");
    party4->SetContentRenderer(std::move(party4Text));
    party4->Redraw();

    // TODO: 아군 파티 정보 동적 업데이트
    // GameManager::GetInstance()->GetParty()로 파티원 정보를 가져와 표시합니다.

    // ===== 애니메이션 영역 (중앙) =====
    Panel* animPanel = _Drawer->CreatePanel("Animation", 66, 8, 50, 31);
    animPanel->SetBorder(true, ETextColor::YELLOW);
    auto animText = std::make_unique<TextRenderer>();
    animText->AddLine("");
    animText->AddLine("");
    animText->AddLine("    [애니메이션 아스키아트");
    animText->AddLine("       재생 영역]");
    animText->AddLine("");
    animText->SetTextColor(MakeColorAttribute(ETextColor::DARK_GRAY, EBackgroundColor::BLACK));
    animPanel->SetContentRenderer(std::move(animText));
    animPanel->Redraw();

    // TODO: 여기에서 전투 애니메이션 재생

    // ===== 적 이미지 패널 (오른쪽) =====
    Panel* enemyPanel = _Drawer->CreatePanel("Enemy", 119, 8, 21, 31);
    enemyPanel->SetBorder(true, ETextColor::RED);
    auto enemyText = std::make_unique<TextRenderer>();
    enemyText->AddLine("");
    enemyText->AddLine("");
    enemyText->AddLine("  [몬스터");
    enemyText->AddLine("   이미지]");
    enemyText->AddLine("");
    enemyText->SetTextColor(MakeColorAttribute(ETextColor::DARK_GRAY, EBackgroundColor::BLACK));
    enemyPanel->SetContentRenderer(std::move(enemyText));
    enemyPanel->Redraw();

    // TODO: 적 이미지 동적 업데이트

    // ===== 파티원 정보 패널 (하단 4개) =====
    for (int i = 0; i < 4; ++i)
    {
        int xPos = 10 + (i * 32);
        Panel* memberPanel = _Drawer->CreatePanel("Member" + std::to_string(i + 1), xPos, 40, 30, 4);
        memberPanel->SetBorder(true, ETextColor::CYAN);
        auto memberText = std::make_unique<TextRenderer>();
        memberText->AddLine("");
        memberText->AddLine("  이름 : 유저 | 직업 : 전사");
        memberText->AddLine("  HP 100/200  ATK / DEF");
        memberPanel->SetContentRenderer(std::move(memberText));
        memberPanel->Redraw();
    }

    // TODO: 파티원 상세 정보 동적 업데이트

      // ===== 시스템 로그 패널 (하단 중앙) =====
    Panel* logPanel = _Drawer->CreatePanel("SystemLog", 10, 45, 90, 10);
    logPanel->SetBorder(true, ETextColor::LIGHT_CYAN);
    auto logText = std::make_unique<TextRenderer>();
    logText->AddLine("");
    logText->AddLineWithColor("  [ 시스템 로그 출력 창 ]",
        MakeColorAttribute(ETextColor::LIGHT_CYAN, EBackgroundColor::BLACK));
    logText->AddLine("");
    logPanel->SetContentRenderer(std::move(logText));
    logPanel->Redraw();

    // TODO: 시스템 로그 동적 업데이트

    // ===== 인벤토리 & 커맨드 패널 (하단 우측) =====
    Panel* commandPanel = _Drawer->CreatePanel("Command", 103, 45, 37, 10);
    commandPanel->SetBorder(true, ETextColor::WHITE);
    auto commandText = std::make_unique<TextRenderer>();
    commandText->AddLine("");
    commandText->AddLineWithColor("  인벤토리 & 커맨드",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    commandText->AddLine("");
    commandPanel->SetContentRenderer(std::move(commandText));
    commandPanel->Redraw();

    // TODO: 인벤토리 & 커맨드 동적 업데이트

    _Drawer->Render();
}

void BattleScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _IsActive = false;
}

void BattleScene::Update()
{
    if (_IsActive)
    {
        _Drawer->Update();
        // TODO: 전투 로직 업데이트
          // BattleManager를 사용하여 전투 진행 상황을 처리합니다.
        HandleInput();
    }
}

void BattleScene::Render()
{
    // UIDrawer::Update()에서 자동 렌더링
}

void BattleScene::HandleInput()
{
    // TODO: 입력 처리 구현
    // VK_UP/VK_DOWN으로 행동 선택
    // VK_RETURN으로 행동 확정
    // 공격, 스킬, 아이템, 도망 처리
}
