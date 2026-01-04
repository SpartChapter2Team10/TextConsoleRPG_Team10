#include "../../../include/UI/Scenes/StageSelectScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/GameManager.h"
#include "../../../include/Manager/DataManager.h"
#include "../../../include/Common/TextColor.h"

StageSelectScene::StageSelectScene()
    : UIScene("StageSelect")
    , _SelectedNodeIndex(0)
    , _CurrentStageLevel(1)
{
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Enter()
{
    _Drawer->ClearScreen();
    _Drawer->RemoveAllPanels();
    _Drawer->Activate();
    _IsActive = true;
    _SelectedNodeIndex = 0;

    // 임시: 노드 생성 (실제로는 StageManager나 GameManager에서 가져와야 함)
    _CurrentNodes.clear();
    _CurrentNodes.push_back({ ENodeType::Battle, "전투", 0 });
    _CurrentNodes.push_back({ ENodeType::Shop, "상점", 1 });
    _CurrentNodes.push_back({ ENodeType::Companion, "동료 영입", 2 });

    // =============================================================================
    // 패널 레이아웃 (150x45 화면 기준)
    // =============================================================================

    // ===== 타이틀 패널 (상단) =====
    Panel* titlePanel = _Drawer->CreatePanel("Title", 10, 2, 130, 6);
    titlePanel->SetBorder(true, ETextColor::LIGHT_CYAN);

    auto titleText = std::make_unique<TextRenderer>();
    titleText->AddLine("");
    titleText->AddLine("");
    titleText->AddLineWithColor("     [층수 _ 스테이지 진입 관련 문구]",
        MakeColorAttribute(ETextColor::LIGHT_CYAN, EBackgroundColor::BLACK));

    titlePanel->SetContentRenderer(std::move(titleText));
    titlePanel->Redraw();

    // TODO: 타이틀 동적 업데이트
    // _CurrentStageLevel에 따라 "1층", "2층" 등 표시

    // ===== 진행 안내 패널 (상단 중앙) =====
    Panel* guidePanel = _Drawer->CreatePanel("Guide", 10, 10, 130, 6);
    guidePanel->SetBorder(true, ETextColor::YELLOW);

    auto guideText = std::make_unique<TextRenderer>();
    guideText->AddLine("");
    guideText->AddLineWithColor("  [진두 현황 문구] - 진두 총 n번, 일반 몹 n마리, 이벤트는 n마리 이하",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));

    guidePanel->SetContentRenderer(std::move(guideText));
    guidePanel->Redraw();

    // TODO: 진행 현황 동적 업데이트
    // 현재까지 진행한 전투 수, 처치한 몹 수 등 표시

    // ===== 스테이지 진입 분할 노드 (중앙) =====
    Panel* nodePanel = _Drawer->CreatePanel("Nodes", 10, 18, 130, 25);
    nodePanel->SetBorder(true, ETextColor::CYAN);

    auto nodeText = std::make_unique<TextRenderer>();
    nodeText->AddLine("");
    nodeText->AddLine("");
    nodeText->AddLine("");
    nodeText->AddLine("     [스테이지 진입 분할 노드]");
    nodeText->AddLine("");
    nodeText->SetTextColor(MakeColorAttribute(ETextColor::DARK_GRAY, EBackgroundColor::BLACK));

    nodePanel->SetContentRenderer(std::move(nodeText));
    nodePanel->Redraw();

    // TODO: 노드 목록 동적 표시
    // _CurrentNodes를 순회하며 선택 가능한 노드들을 표시합니다.
    //
    // 구현 방법:
    // Panel* nodePanel = _Drawer->GetPanel("Nodes");
  // auto nodeText = std::make_unique<TextRenderer>();
    //
    // nodeText->AddLine("");
    // nodeText->AddLine("  선택 가능한 경로:");
    // nodeText->AddLine("");
    //
    // for (size_t i = 0; i < _CurrentNodes.size(); ++i) {
    //     std::string nodeIcon = "";
    //     ETextColor nodeColor = ETextColor::WHITE;
    //
    //     switch (_CurrentNodes[i].Type) {
    //     case ENodeType::Battle:
  //         nodeIcon = "⚔";
    //         nodeColor = ETextColor::LIGHT_RED;
    //         break;
    //     case ENodeType::Shop:
    //    nodeIcon = "🛒";
    //nodeColor = ETextColor::LIGHT_YELLOW;
    //break;
    //     case ENodeType::Companion:
    //         nodeIcon = "👤";
    //         nodeColor = ETextColor::LIGHT_GREEN;
    //       break;
    //   case ENodeType::Boss:
    //  nodeIcon = "💀";
    // nodeColor = ETextColor::LIGHT_MAGENTA;
    //         break;
    //     }
    //
 //  std::string prefix = (i == _SelectedNodeIndex) ? "> " : "  ";
    //     std::string nodeLine = prefix + nodeIcon + " " + _CurrentNodes[i].Name;
    //
  //     if (i == _SelectedNodeIndex) {
    //       nodeText->AddLineWithColor(nodeLine,
  //     MakeColorAttribute(ETextColor::LIGHT_YELLOW, ...));
    //     } else {
    //         nodeText->AddLineWithColor(nodeLine,
    //             MakeColorAttribute(nodeColor, ...));
  //     }
    //     nodeText->AddLine("");
    // }
    //
    // nodePanel->SetContentRenderer(std::move(nodeText));
    // nodePanel->Redraw();

    // ===== 진입 방식 및 키 설명 (하단) =====
    Panel* controlPanel = _Drawer->CreatePanel("Control", 10, 44, 90, 6);
    controlPanel->SetBorder(true, ETextColor::LIGHT_CYAN);

    auto controlText = std::make_unique<TextRenderer>();
    controlText->AddLine("");
    controlText->AddLineWithColor("  [진입 방식 및 키 설명]",
        MakeColorAttribute(ETextColor::LIGHT_CYAN, EBackgroundColor::BLACK));
    controlText->AddLine("");
    controlText->AddLine("  [↑/↓] 선택   [Enter] 진입   [ESC] 메인 메뉴");

    controlPanel->SetContentRenderer(std::move(controlText));
    controlPanel->Redraw();

    // ===== 인벤토리 & 커맨드 패널 (하단 우측) =====
    Panel* commandPanel = _Drawer->CreatePanel("Command", 103, 44, 37, 6);
    commandPanel->SetBorder(true, ETextColor::WHITE);

    auto commandText = std::make_unique<TextRenderer>();
    commandText->AddLine("");
    commandText->AddLineWithColor("  인벤토리 & 커맨드",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    commandText->AddLine("");

    commandPanel->SetContentRenderer(std::move(commandText));
    commandPanel->Redraw();

    _Drawer->Render();
}

void StageSelectScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _CurrentNodes.clear();
    _IsActive = false;
}

void StageSelectScene::Update()
{
    if (_IsActive)
    {
        _Drawer->Update();
        HandleInput();
    }
}

void StageSelectScene::Render()
{
    // UIDrawer::Update()에서 자동 렌더링
}

void StageSelectScene::HandleInput()
{
    // TODO: 입력 처리 구현
    //
    // 구현 방법:
    // InputManager* input = InputManager::GetInstance();
  // if (!input->IsKeyPressed()) return;
   //
// int keyCode = input->GetKeyCode();
    //
    // if (keyCode == VK_UP) {  // ↑ 위로
    //     _SelectedNodeIndex--;
    //     if (_SelectedNodeIndex < 0) {
    //    _SelectedNodeIndex = static_cast<int>(_CurrentNodes.size()) - 1;
    //     }
    //
    //     // 노드 패널 업데이트 (위의 TODO 참고)
    //     _Drawer->Render();
    // }
    // else if (keyCode == VK_DOWN) {  // ↓ 아래로
    //     _SelectedNodeIndex++;
    //     if (_SelectedNodeIndex >= static_cast<int>(_CurrentNodes.size())) {
    //    _SelectedNodeIndex = 0;
    //  }
    //
    //     // 노드 패널 업데이트
    //     _Drawer->Render();
    // }
    // else if (keyCode == VK_RETURN) {  // Enter - 노드 진입
    //     if (_SelectedNodeIndex >= 0 && _SelectedNodeIndex < _CurrentNodes.size()) {
    //         ENodeType selectedType = _CurrentNodes[_SelectedNodeIndex].Type;
    //
    //    _IsActive = false;
    //         Exit();
    //
    //         // 노드 타입에 따라 씬 전환
    //       switch (selectedType) {
    //         case ENodeType::Battle:
    //             // BattleManager::GetInstance()->StartBattle(...);
    //             SceneManager::GetInstance()->ChangeScene(ESceneType::Battle);
    //           break;
    //         case ENodeType::Shop:
    //   SceneManager::GetInstance()->ChangeScene(ESceneType::Shop);
    //   break;
    //  case ENodeType::Companion:
    // SceneManager::GetInstance()->ChangeScene(ESceneType::CompanionRecruit);
    //          break;
    //         case ENodeType::Boss:
    //      // BattleManager::GetInstance()->StartBossBattle(...);
//     SceneManager::GetInstance()->ChangeScene(ESceneType::Battle);
    //        break;
    //         case ENodeType::Event:
  // // EventManager::GetInstance()->TriggerRandomEvent();
    //     break;
    //         }
    //     }
    // }
    // else if (keyCode == VK_ESCAPE) {  // ESC - 메인 메뉴
    //     _IsActive = false;
    //     Exit();
    //     SceneManager::GetInstance()->ChangeScene(ESceneType::MainMenu);
    // }
}
