#include "../../../include/UI/Scenes/CharacterSelectScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/GameManager.h"
#include "../../../include/Manager/DataManager.h"
#include "../../../include/Common/TextColor.h"

CharacterSelectScene::CharacterSelectScene()
    : UIScene("CharacterSelect")
    , _CurrentSelection(0)
    , _TotalClasses(4)
{
}

CharacterSelectScene::~CharacterSelectScene()
{
}

void CharacterSelectScene::Enter()
{
    _Drawer->ClearScreen();
    _Drawer->RemoveAllPanels();
    _Drawer->Activate();
    _IsActive = true;

    // =============================================================================
    // 패널 레이아웃 (150x45 화면 기준)
    // =============================================================================

    // ===== 타이틀 패널 (상단) =====
    Panel* titlePanel = _Drawer->CreatePanel("Title", 0, 0, 150, 8);
    titlePanel->SetBorder(true, ETextColor::CYAN);

    auto titleText = std::make_unique<TextRenderer>();
    titleText->AddLine("");
    titleText->AddLine("");
    titleText->AddLineWithColor("        [ 직업 선택 ]",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    titleText->AddLineWithColor("1/4 페이지",
        MakeColorAttribute(ETextColor::WHITE, EBackgroundColor::BLACK));

    titlePanel->SetContentRenderer(std::move(titleText));
    titlePanel->Redraw();

    // ===== 이미지 패널 (왼쪽) =====
    Panel* imagePanel = _Drawer->CreatePanel("ClassImage", 5, 9, 60, 28);
    imagePanel->SetBorder(true, ETextColor::YELLOW);

    // 더미 이미지 (플레이스홀더)
    auto dummyImage = std::make_unique<TextRenderer>();
    dummyImage->AddLine("");
    dummyImage->AddLine("");
    dummyImage->AddLine("");
    dummyImage->AddLine("");
    dummyImage->AddLine("");
    dummyImage->AddLine("");
    dummyImage->AddLine("");
    dummyImage->AddLine("            [직업 별 초상화 이미지]");
    dummyImage->AddLine("");
    dummyImage->AddLine("");
    dummyImage->AddLine("");
    dummyImage->SetTextColor(MakeColorAttribute(ETextColor::DARK_GRAY, EBackgroundColor::BLACK));

    imagePanel->SetContentRenderer(std::move(dummyImage));
    imagePanel->Redraw();

    // TODO: 여기에서 직업별 이미지 조정
  // 사용자 입력에 따라 _CurrentSelection 값이 변경되면 이미지를 업데이트해야 합니다.
    // 
// 구현 방법:
    // Panel* imagePanel = _Drawer->GetPanel("ClassImage");
    // auto artRenderer = std::make_unique<AsciiArtRenderer>();
    // std::string uiPath = DataManager::GetInstance()->GetResourcePath("UI");
    // 
    // // _CurrentSelection에 따라 파일명 결정 (예: "전사.txt", "마법사.txt")
    // if (artRenderer->LoadFromFile(uiPath, "전사.txt")) {
    //     artRenderer->SetAlignment(ArtAlignment::CENTER);
 //     artRenderer->SetColor(ETextColor::LIGHT_YELLOW);
    //     imagePanel->SetContentRenderer(std::move(artRenderer));
    //     imagePanel->Redraw();
    // }

    // ===== 정보 패널 (오른쪽) =====
    Panel* infoPanel = _Drawer->CreatePanel("ClassInfo", 70, 9, 75, 28);
    infoPanel->SetBorder(true, ETextColor::GREEN);

    // 정적 레이블 (기본 구조)
    auto infoText = std::make_unique<TextRenderer>();
    infoText->AddLine("");
    infoText->AddLineWithColor("  직업 클래스 명",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    infoText->AddLine("");
    infoText->AddLineWithColor("  역할 및 특성",
        MakeColorAttribute(ETextColor::CYAN, EBackgroundColor::BLACK));
    infoText->AddLine("  [직업 설명이 여기에 표시됩니다]");
    infoText->AddLine("");
    infoText->AddLineWithColor("  스탯:",
        MakeColorAttribute(ETextColor::WHITE, EBackgroundColor::BLACK));
    infoText->AddLine("    HP:");
    infoText->AddLine("    ATK:");
    infoText->AddLine("    DEF:");
    infoText->AddLine("");
    infoText->AddLine("");
    infoText->AddLineWithColor("  • 고유 스킬1",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    infoText->AddLine("    [스킬 설명]");
    infoText->AddLine("");
    infoText->AddLineWithColor("  • 고유 스킬2",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    infoText->AddLine("    [스킬 설명]");

    infoPanel->SetContentRenderer(std::move(infoText));
    infoPanel->Redraw();

    // TODO: 직업 정보 동적 업데이트
    // 사용자 입력에 따라 _CurrentSelection 값이 변경되면 정보를 업데이트해야 합니다.
    // 
    // 구현 방법:
    // Panel* infoPanel = _Drawer->GetPanel("ClassInfo");
    // auto infoText = std::make_unique<TextRenderer>();
    // 
    // // _CurrentSelection에 따라 직업 정보 표시
 // infoText->AddLineWithColor("  전사", MakeColorAttribute(ETextColor::LIGHT_YELLOW, ...));
    // infoText->AddLine("  강력한 힘과 방어력을 가진 근접 전투의 달인");
    // infoText->AddLine("    HP: 300");
    // infoText->AddLine("    ATK: 40");
    // // ...
    // 
    // infoPanel->SetContentRenderer(std::move(infoText));
    // infoPanel->Redraw();

    // ===== 안내 패널 (하단) =====
    Panel* guidePanel = _Drawer->CreatePanel("Guide", 5, 38, 140, 6);
    guidePanel->SetBorder(true, ETextColor::WHITE);

    auto guideText = std::make_unique<TextRenderer>();
    guideText->AddLine("");
    guideText->AddLineWithColor("  [ 직업 선택 관련 문구 영역 ]",
        MakeColorAttribute(ETextColor::LIGHT_CYAN, EBackgroundColor::BLACK));
    guideText->AddLine("");
    guideText->AddLine("  2. Mage, Defense: 낮음, 높은 공격력과 마법으로 처치합니다. [ ←, →, 선택 ], [←, 다음 ]");

    guidePanel->SetContentRenderer(std::move(guideText));
    guidePanel->Redraw();

    // TODO: 안내 메시지 동적 업데이트
    // _CurrentSelection 값에 따라 안내 문구를 변경할 수 있습니다.
    // 
    // 구현 방법:
    // Panel* guidePanel = _Drawer->GetPanel("Guide");
    // auto guideText = std::make_unique<TextRenderer>();
    // 
    // std::string message = std::to_string(_CurrentSelection + 1) + ". 전사, ...";
    // guideText->AddLine(message);
// 
    // guidePanel->SetContentRenderer(std::move(guideText));
    // guidePanel->Redraw();

    _Drawer->Render();
}

void CharacterSelectScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _IsActive = false;
}

void CharacterSelectScene::Update()
{
    if (_IsActive)
    {
        _Drawer->Update();
        HandleInput();
    }
}

void CharacterSelectScene::Render()
{
    // UIDrawer::Update()에서 자동 렌더링
}

void CharacterSelectScene::HandleInput()
{
    // TODO: 입력 처리 구현
    // 
    // 구현 방법:
    // InputManager* input = InputManager::GetInstance();
    // if (!input->IsKeyPressed()) return;
    // 
    // int keyCode = input->GetKeyCode();
    // 
    // if (keyCode == VK_LEFT) {  // ← 왼쪽 화살표
    //     _CurrentSelection--;
    //if (_CurrentSelection < 0) _CurrentSelection = _TotalClasses - 1;
    //     
    //     // 이미지 패널 업데이트 (위의 TODO 참고)
    //     // 정보 패널 업데이트 (위의 TODO 참고)
    //   // 안내 패널 업데이트 (위의 TODO 참고)
    //     
    //     _Drawer->Render();
    // }
    // else if (keyCode == VK_RIGHT) {  // → 오른쪽 화살표
    //     _CurrentSelection++;
    //     if (_CurrentSelection >= _TotalClasses) _CurrentSelection = 0;
    //     
    //     // 패널들 업데이트
    //     _Drawer->Render();
    // }
    // else if (keyCode == VK_RETURN) {  // Enter - 선택 확인
    //     // GameManager 사용 예시:
    //     // GameManager* gm = GameManager::GetInstance();
    //     // auto player = std::make_shared<Player>("플레이어이름", true);
 //     // gm->SetMainPlayer(player);
    //     
    //     // 다음 씬으로 이동:
 //     // SceneManager::GetInstance()->ChangeScene(ESceneType::StoryProgress);
    // }
    // else if (keyCode == VK_ESCAPE) {  // ESC - 이전 화면
    //     // SceneManager::GetInstance()->ChangeScene(ESceneType::PlayerNameInput);
    // }
}
