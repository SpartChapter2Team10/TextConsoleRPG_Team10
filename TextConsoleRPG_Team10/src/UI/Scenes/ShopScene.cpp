#include "../../../include/UI/Scenes/ShopScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/GameManager.h"
#include "../../../include/Manager/ShopManager.h"
#include "../../../include/Manager/DataManager.h"
#include "../../../include/Common/TextColor.h"

ShopScene::ShopScene()
    : UIScene("Shop")
    , _SelectedItemIndex(0)
    , _IsBuyMode(true)
    , _PlayerInventorySlot(0)
{
}

ShopScene::~ShopScene()
{
}

void ShopScene::Enter()
{
    _Drawer->ClearScreen();
    _Drawer->RemoveAllPanels();
    _Drawer->Activate();
    _IsActive = true;

    // 상점 재개장 (CSV에서 상품 로드)
    ShopManager::GetInstance()->ReopenShop("Items.csv");

    // =============================================================================
    // 패널 레이아웃 (150x45 화면 기준)
    // =============================================================================

    // ===== 상점 타이틀 패널 (상단) =====
    Panel* titlePanel = _Drawer->CreatePanel("Title", 10, 2, 130, 6);
    titlePanel->SetBorder(true, ETextColor::LIGHT_YELLOW);

    auto titleText = std::make_unique<TextRenderer>();
    titleText->AddLine("");
    titleText->AddLine("");
    titleText->AddLineWithColor("          [상점 문구]",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));

    titlePanel->SetContentRenderer(std::move(titleText));
    titlePanel->Redraw();

    // ===== 상점 배경 & NPC 이미지 패널 (좌측) =====
    Panel* imagePanel = _Drawer->CreatePanel("ShopImage", 10, 10, 50, 45);
    imagePanel->SetBorder(true, ETextColor::CYAN);

    auto imageText = std::make_unique<TextRenderer>();
    imageText->AddLine("");
    imageText->AddLine("");
    imageText->AddLine("");
    imageText->AddLine("");
    imageText->AddLine("");
    imageText->AddLine("    [상점 배경 & NPC 이미지]");
    imageText->AddLine("");
    imageText->SetTextColor(MakeColorAttribute(ETextColor::DARK_GRAY, EBackgroundColor::BLACK));

    imagePanel->SetContentRenderer(std::move(imageText));
    imagePanel->Redraw();

    // TODO: 여기에서 상점 이미지 조정
    // 상점 배경과 NPC 이미지를 표시합니다.
    //
    // 구현 방법:
    // Panel* imagePanel = _Drawer->GetPanel("ShopImage");
    // auto artRenderer = std::make_unique<AsciiArtRenderer>();
    // std::string uiPath = DataManager::GetInstance()->GetResourcePath("UI");
    //
    // if (artRenderer->LoadFromFile(uiPath, "Shop.txt")) {
    //     artRenderer->SetAlignment(ArtAlignment::CENTER);
    //     artRenderer->SetColor(ETextColor::LIGHT_YELLOW);
    //     imagePanel->SetContentRenderer(std::move(artRenderer));
    //     imagePanel->Redraw();
    // }

    // ===== 상품 목록 패널 (우측 상단, 3개) =====
    for (int i = 0; i < 3; ++i)
    {
        int yPos = 10 + (i * 15);
        Panel* itemPanel = _Drawer->CreatePanel("Item" + std::to_string(i + 1), 63, yPos, 42, 14);
        itemPanel->SetBorder(true, ETextColor::GREEN);

        auto itemText = std::make_unique<TextRenderer>();
        itemText->AddLine("");
        itemText->AddLineWithColor("  아이템 이미지",
            MakeColorAttribute(ETextColor::CYAN, EBackgroundColor::BLACK));
        itemText->AddLine("");
        itemText->AddLine("  아이템 이미지 | 아이템 비용");
        itemText->AddLine("");
        itemText->AddLine("  설명 및 효과");
        itemText->AddLine("");

        itemPanel->SetContentRenderer(std::move(itemText));
        itemPanel->Redraw();
    }

    // TODO: 상품 목록 동적 업데이트
    // ShopManager에서 판매 중인 상품 목록을 가져와 표시합니다.
    //
    // 구현 방법:
    // auto shopItems = ShopManager::GetInstance()->GetShopItems();
    //
    // for (size_t i = 0; i < 3 && i < shopItems.size(); ++i) {
    //     std::string panelID = "Item" + std::to_string(i + 1);
    //     Panel* itemPanel = _Drawer->GetPanel(panelID);
    //
    //     auto itemText = std::make_unique<TextRenderer>();
    //     itemText->AddLine("");
//
    //     // 선택된 아이템 하이라이트
    //if (_IsBuyMode && i == _SelectedItemIndex) {
    //     itemText->AddLineWithColor("  > " + shopItems[i].name,
    //       MakeColorAttribute(ETextColor::LIGHT_YELLOW, ...));
    //  } else {
    //         itemText->AddLine("    " + shopItems[i].name);
    //     }
    //
    //itemText->AddLine("  가격: " + std::to_string(shopItems[i].price) + " G");
    //     itemText->AddLine("  재고: " + std::to_string(shopItems[i].stock));
  // itemText->AddLine("");
    //
    //     itemPanel->SetContentRenderer(std::move(itemText));
    //     itemPanel->Redraw();
    // }

    // ===== 플레이어 정보 패널 (우측 상단) =====
    Panel* playerPanel = _Drawer->CreatePanel("PlayerInfo", 108, 10, 32, 15);
    playerPanel->SetBorder(true, ETextColor::MAGENTA);

    auto playerText = std::make_unique<TextRenderer>();
    playerText->AddLine("");
    playerText->AddLineWithColor("  [탭]",
        MakeColorAttribute(ETextColor::LIGHT_CYAN, EBackgroundColor::BLACK));
    playerText->AddLine("");
    playerText->AddLine("  = 탭 형식");
    playerText->AddLine("");

    playerPanel->SetContentRenderer(std::move(playerText));
    playerPanel->Redraw();

    // TODO: 플레이어 정보 동적 업데이트
  // 현재 소지 골드 등을 표시합니다.
    //
    // 구현 방법:
    // auto player = GameManager::GetInstance()->GetMainPlayer();
    // Panel* playerPanel = _Drawer->GetPanel("PlayerInfo");
    //
    // auto playerText = std::make_unique<TextRenderer>();
    // playerText->AddLine("");
    // playerText->AddLineWithColor("  [플레이어 정보]",
    //     MakeColorAttribute(ETextColor::LIGHT_CYAN, ...));
    // playerText->AddLine("");
    // playerText->AddLine("  소지금: " + std::to_string(player->GetGold()) + " G");
    // playerText->AddLine("");
    //
    // playerPanel->SetContentRenderer(std::move(playerText));
    // playerPanel->Redraw();

    // ===== 시스템 로그 패널 (하단 좌측) =====
    Panel* logPanel = _Drawer->CreatePanel("SystemLog", 10, 56, 90, 9);
    logPanel->SetBorder(true, ETextColor::LIGHT_CYAN);

    auto logText = std::make_unique<TextRenderer>();
    logText->AddLine("");
    logText->AddLineWithColor("  [ 시스템 로그 출력 창 ]",
        MakeColorAttribute(ETextColor::LIGHT_CYAN, EBackgroundColor::BLACK));
    logText->AddLine("");

    logPanel->SetContentRenderer(std::move(logText));
    logPanel->Redraw();

    // TODO: 거래 로그 표시
    // 구매/판매 결과를 로그로 출력합니다.
    //
    // 구현 방법:
  // Panel* logPanel = _Drawer->GetPanel("SystemLog");
    // auto logText = std::make_unique<TextRenderer>();
    //
    // logText->SetAutoScroll(true);
    // logText->AddLine("상점에 오신 것을 환영합니다!");
  // logText->AddLine("");
    //
    // // 거래 후:
    // // auto [success, message, goldChange, itemName] = ShopManager::GetInstance()->BuyItem(...);
    // // if (success) {
    // //   logText->AddLineWithColor("구매 성공: " + itemName, 
    // // MakeColorAttribute(ETextColor::LIGHT_GREEN, ...));
 // // } else {
    // //   logText->AddLineWithColor("구매 실패: " + message,
    // //   MakeColorAttribute(ETextColor::LIGHT_RED, ...));
    // // }
    //
    // logPanel->SetContentRenderer(std::move(logText));
    // logPanel->Redraw();

 // ===== 인벤토리 & 커맨드 패널 (하단 우측) =====
    Panel* commandPanel = _Drawer->CreatePanel("Command", 103, 56, 37, 9);
    commandPanel->SetBorder(true, ETextColor::WHITE);

    auto commandText = std::make_unique<TextRenderer>();
    commandText->AddLine("");
    commandText->AddLineWithColor("  인벤토리 & 커맨드",
        MakeColorAttribute(ETextColor::LIGHT_YELLOW, EBackgroundColor::BLACK));
    commandText->AddLine("");
    commandText->AddLine("  [↑/↓] 선택");
    commandText->AddLine("  [Enter] 구매");
    commandText->AddLine("  [Tab] 구매/판매 전환");
    commandText->AddLine("  [ESC] 나가기");

    commandPanel->SetContentRenderer(std::move(commandText));
    commandPanel->Redraw();

    // TODO: 커맨드 동적 업데이트
    // 구매/판매 모드에 따라 다른 안내 표시

    _Drawer->Render();
}

void ShopScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _IsActive = false;
}

void ShopScene::Update()
{
    if (_IsActive)
    {
        _Drawer->Update();
        HandleInput();
    }
}

void ShopScene::Render()
{
    // UIDrawer::Update()에서 자동 렌더링
}

void ShopScene::HandleInput()
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
    //     if (_IsBuyMode) {
 //       _SelectedItemIndex--;
    //  if (_SelectedItemIndex < 0) {
    //   _SelectedItemIndex = ShopManager::GetInstance()->GetSellListSize() - 1;
    //         }
 //     } else {
  //         // 판매 모드: 인벤토리 선택
    //       _PlayerInventorySlot--;
    //     if (_PlayerInventorySlot < 0) _PlayerInventorySlot = 9;  // 인벤토리 최대 슬롯
    //     }
//     // 상품 목록 패널 업데이트 (위의 TODO 참고)
 //     _Drawer->Render();
    // }
    // else if (keyCode == VK_DOWN) {  // ↓ 아래로
    //     if (_IsBuyMode) {
    //       _SelectedItemIndex++;
    //      if (_SelectedItemIndex >= ShopManager::GetInstance()->GetSellListSize()) {
    //             _SelectedItemIndex = 0;
    //         }
    // } else {
    //     _PlayerInventorySlot++;
    //         if (_PlayerInventorySlot > 9) _PlayerInventorySlot = 0;
    //     }
    //     // 상품 목록 패널 업데이트
    //     _Drawer->Render();
    // }
    // else if (keyCode == VK_RETURN) {  // Enter - 구매/판매
    //     auto player = GameManager::GetInstance()->GetMainPlayer();
    //
    //     if (_IsBuyMode) {
    // // 구매 처리
    //         auto [success, message, goldChange, itemName] = 
    //    ShopManager::GetInstance()->BuyItem(player.get(), _SelectedItemIndex);
    //
    //         // 로그 패널에 결과 표시 (위의 TODO 참고)
    //         // 상품 목록 패널 업데이트 (재고 변경)
    //  // 플레이어 정보 패널 업데이트 (골드 변경)
    //     }
    //     else {
    //         // 판매 처리
    //         auto [success, message, goldChange, itemName] = 
    //             ShopManager::GetInstance()->SellItem(player.get(), _PlayerInventorySlot);
    //
//     // 로그 패널에 결과 표시
  //         // 플레이어 정보 패널 업데이트
    //     }
    //
    //   _Drawer->Render();
    // }
    // else if (keyCode == VK_TAB) {  // Tab - 구매/판매 전환
    //     _IsBuyMode = !_IsBuyMode;
//     _SelectedItemIndex = 0;
    //     _PlayerInventorySlot = 0;
    //
    //     // 모드 전환 시 UI 업데이트
    //     // 커맨드 패널 업데이트 (안내 문구 변경)
    //     _Drawer->Render();
    // }
    // else if (keyCode == VK_ESCAPE) {  // ESC - 상점 나가기
    //     SceneManager::GetInstance()->ChangeScene(ESceneType::StageSelect);
    // }
}
