#include "../../../include/UI/Scenes/CompanionRecruitScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/GameManager.h"
#include "../../../include/Manager/DataManager.h"
#include "../../../include/Common/TextColor.h"
#include <Windows.h>

CompanionRecruitScene::CompanionRecruitScene()
    : UIScene("CompanionRecruit")
    , _CompanionName("???")
    , _InputName("")
    , _RecruitAccepted(false)
    , _SelectedOption(0)
{
}

CompanionRecruitScene::~CompanionRecruitScene()
{
}

void CompanionRecruitScene::Enter()
{
    _Drawer->ClearScreen();
    _Drawer->RemoveAllPanels();
    _Drawer->Activate();
    _IsActive = true;
    _RecruitAccepted = false;
    _SelectedOption = 0;
    _InputName.clear();

    // =============================================================================
    // 패널 레이아웃 (150x45 화면 기준)
    // =============================================================================

    // ===== 타이틀 패널 (상단) =====
    Panel* titlePanel = _Drawer->CreatePanel("Title", 10, 2, 130, 6);
    titlePanel->SetBorder(true, ETextColor::LIGHT_CYAN);

    auto titleText = std::make_unique<TextRenderer>();
    titleText->AddLine("");
    titleText->AddLine("");
    titleText->AddLineWithColor("        [동료 발견! 스테이지 관련 문구] - 전교권 김독 입구",
        MakeColorAttribute(ETextColor::LIGHT_CYAN, EBackgroundColor::BLACK));

    titlePanel->SetContentRenderer(std::move(titleText));
    titlePanel->Redraw();

    // ===== 동료 캐릭터 이미지 패널 (좌측) =====
    Panel* characterPanel = _Drawer->CreatePanel("Character", 10, 10, 65, 30);
    characterPanel->SetBorder(true, ETextColor::YELLOW);

    auto characterText = std::make_unique<TextRenderer>();
    characterText->AddLine("");
    characterText->AddLine("");
    characterText->AddLine("");
    characterText->AddLine("        [정식 캐릭터 이미지]");
    characterText->AddLine("        • 성급 드가 이미지");
    characterText->AddLine("   • 합류 커특은 미표 이미지 등");
    characterText->AddLine("");
    characterText->SetTextColor(MakeColorAttribute(ETextColor::DARK_GRAY, EBackgroundColor::BLACK));

    characterPanel->SetContentRenderer(std::move(characterText));
    characterPanel->Redraw();

    // TODO: 여기에서 동료 이미지 조정
    // 동료 직업에 따라 다른 ASCII Art 표시

    // ===== 동료 정보 패널 (우측 상단) =====
    Panel* infoPanel = _Drawer->CreatePanel("CompanionInfo", 78, 10, 62, 30);
    infoPanel->SetBorder(true, ETextColor::GREEN);

    auto infoText = std::make_unique<TextRenderer>();
    infoText->AddLine("");
    infoText->AddLineWithColor("  [동료 이미지]",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    infoText->AddLine("");
    infoText->AddLine("  - 대충 살아");
    infoText->AddLine("    그냥");
    infoText->AddLine("");

    infoPanel->SetContentRenderer(std::move(infoText));
    infoPanel->Redraw();

    // TODO: 동료 정보 동적 업데이트

    // ===== 플레이어 정보 패널 (하단) =====
    Panel* playerPanel = _Drawer->CreatePanel("PlayerInfo", 10, 41, 130, 4);
    playerPanel->SetBorder(true, ETextColor::CYAN);

    auto playerText = std::make_unique<TextRenderer>();
    playerText->AddLine("");
    playerText->AddLine("  이름 : 가령 | 직업 : 전사   HP 100/200 ATK / DEF");

    playerPanel->SetContentRenderer(std::move(playerText));
    playerPanel->Redraw();

    // TODO: 플레이어 정보 동적 업데이트

    // ===== 시스템 로그 패널 (하단 좌측) =====
    Panel* logPanel = _Drawer->CreatePanel("SystemLog", 10, 46, 90, 9);
    logPanel->SetBorder(true, ETextColor::LIGHT_CYAN);

    auto logText = std::make_unique<TextRenderer>();
    logText->AddLine("");
    logText->AddLineWithColor("  [ 시스템 로그 출력 창 ]",
        MakeColorAttribute(ETextColor::LIGHT_CYAN, EBackgroundColor::BLACK));
    logText->AddLine("");

    logPanel->SetContentRenderer(std::move(logText));
    logPanel->Redraw();

    // ===== 인벤토리 & 커맨드 패널 (하단 우측) =====
    Panel* commandPanel = _Drawer->CreatePanel("Command", 103, 46, 37, 9);
    commandPanel->SetBorder(true, ETextColor::WHITE);

    auto commandText = std::make_unique<TextRenderer>();
    commandText->AddLine("");
    commandText->AddLineWithColor("  인벤토리 & 커맨드",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    commandText->AddLine("");
    commandText->AddLine("  [←/→] 선택");
    commandText->AddLine("  [Enter] 확인");
    commandText->AddLine("  [ESC] 거부");

    commandPanel->SetContentRenderer(std::move(commandText));
    commandPanel->Redraw();

    _Drawer->Render();
}

void CompanionRecruitScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _IsActive = false;
}

void CompanionRecruitScene::Update()
{
    if (_IsActive)
    {
        _Drawer->Update();
        HandleInput();
    }
}

void CompanionRecruitScene::Render()
{
    // UIDrawer::Update()에서 자동 렌더링
}

void CompanionRecruitScene::HandleInput()
{
    // TODO: 입력 처리 구현
    // VK_LEFT/RIGHT: 영입/거부 선택 전환
    // VK_RETURN: 선택 확인 (영입 시 이름 입력)
    // VK_ESCAPE: 거부
}
