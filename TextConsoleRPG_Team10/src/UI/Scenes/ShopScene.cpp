#include "../../../include/UI/Scenes/ShopScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/ShopManager.h"
#include "../../../include/Manager/InputManager.h"
#include "../../../include/Unit/Player.h"
#include "../../../include/Item/Inventory.h"
#include "../../../include/Item/IItem.h"

ShopScene::ShopScene()
    : UIScene("Shop")
    , _StayInShop(true)
{
}

ShopScene::~ShopScene()
{
}

void ShopScene::Enter()
{
 _Drawer->Initialize();
    _Drawer->Activate();
    _IsActive = true;
    _StayInShop = true;

    ShopManager::GetInstance()->ReopenShop();

  // 상점 타이틀
    Panel* titlePanel = _Drawer->CreatePanel("ShopTitle", 0, 0, 106, 5);
  titlePanel->SetBorder(true, 14);
    auto title = std::make_unique<TextRenderer>();
    title->AddLine("=== 상점 ===");
    title->AddLine("어서오세요! 모험에 필요한 물품을 판매합니다.");
title->SetTextColor(14);
    titlePanel->SetContentRenderer(std::move(title));

    _Drawer->Render();
}

void ShopScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _Drawer->Deactivate();
    _IsActive = false;
}

void ShopScene::Update()
{
    if (_StayInShop)
 {
        ShowShopMenu();
    }
    else
    {
   // 다음 전투로
        SceneManager::GetInstance()->ChangeScene(ESceneType::Battle);
    }
}

void ShopScene::Render()
{
    _Drawer->Render();
}

void ShopScene::HandleInput()
{
    // ShowShopMenu에서 처리
}

void ShopScene::ShowShopMenu()
{
    Player* player = SceneManager::GetInstance()->GetPlayer();
    if (!player) return;

    // 상점 아이템 목록
    Panel* itemListPanel = _Drawer->CreatePanel("ItemList", 0, 5, 70, 35);
    itemListPanel->SetBorder(true, 11);
 auto itemList = std::make_unique<TextRenderer>();
    
    // ShopManager에서 아이템 목록 가져와 표시
    ShopManager* shop = ShopManager::GetInstance();
    itemList->AddLine("=== 판매 아이템 ===");
    itemList->AddLine("");
    // TODO: ShopManager::PrintShop() 내용을 TextRenderer로 변환
    itemList->AddLine("(아이템 목록은 ShopManager 참조)");
    
    itemListPanel->SetContentRenderer(std::move(itemList));

    // 플레이어 정보
    Panel* playerInfoPanel = _Drawer->CreatePanel("PlayerInfo", 70, 5, 36, 15);
    playerInfoPanel->SetBorder(true, 10);
    auto playerInfo = std::make_unique<TextRenderer>();
    playerInfo->AddLine("=== 플레이어 정보 ===");
    playerInfo->AddLine("");
 playerInfo->AddLine("이름: " + player->GetName());
    playerInfo->AddLine("레벨: " + std::to_string(player->GetLevel()));
    playerInfo->AddLine("소지금: " + std::to_string(player->GetGold()) + " G");
    playerInfo->SetTextColor(10);
    playerInfoPanel->SetContentRenderer(std::move(playerInfo));

  // 메뉴
    Panel* menuPanel = _Drawer->CreatePanel("Menu", 0, 40, 106, 10);
    menuPanel->SetBorder(true, 7);
    auto menu = std::make_unique<TextRenderer>();
    menu->AddLine("[1] 아이템 구매");
    menu->AddLine("[2] 아이템 판매");
    menu->AddLine("[3] 상점 나가기");
    menu->AddLine("");
    menu->AddLine("선택하세요:");
    menuPanel->SetContentRenderer(std::move(menu));

    _Drawer->Render();

    int choice = _Input->GetIntInput("선택: ", 1, 3);

    switch (choice)
    {
    case 1:
        BuyItem();
        break;
    case 2:
      SellItem();
        break;
    case 3:
        _StayInShop = false;
        break;
    }

    // 메뉴 패널들 제거 (다시 그리기 위해)
    _Drawer->RemovePanel("ItemList");
    _Drawer->RemovePanel("PlayerInfo");
    _Drawer->RemovePanel("Menu");
}

void ShopScene::BuyItem()
{
    Player* player = SceneManager::GetInstance()->GetPlayer();
  ShopManager* shop = ShopManager::GetInstance();

    int itemChoice = _Input->GetIntInput(
  "구매할 아이템 번호 (취소: 0): ", 
        0, 
        static_cast<int>(shop->GetSellListSize())
    );

    if (itemChoice > 0)
    {
      shop->BuyItem(player, itemChoice - 1);
    }
}

void ShopScene::SellItem()
{
    Player* player = SceneManager::GetInstance()->GetPlayer();
    ShopManager* shop = ShopManager::GetInstance();
    Inventory& inventory = player->GetInventory();

    // 인벤토리 표시
    Panel* inventoryPanel = _Drawer->CreatePanel("Inventory", 10, 15, 86, 20);
    inventoryPanel->SetBorder(true, 11);
    auto invText = std::make_unique<TextRenderer>();
    invText->AddLine("=== 내 인벤토리 ===");
    invText->AddLine("");

    bool hasItems = false;
    for (int i = 0; i < 10; ++i)
    {
        IItem* item = inventory.GetItemAtSlot(i);
        int amount = inventory.GetSlotAmount(i);

 if (item && amount > 0)
  {
          hasItems = true;
 int sellPrice = static_cast<int>(item->GetPrice() * 0.6);
    std::string msg = std::to_string(i + 1) + ". " + item->GetName() + " x" +
       std::to_string(amount) + " (판매가: " + std::to_string(sellPrice) + " G)";
    invText->AddLine(msg);
        }
    }

    if (!hasItems)
    {
invText->AddLine("판매할 아이템이 없습니다.");
    }

    inventoryPanel->SetContentRenderer(std::move(invText));
    _Drawer->Render();

    if (hasItems)
    {
        int slotChoice = _Input->GetIntInput(
    "판매할 아이템 슬롯 번호 (취소: 0): ", 
       0, 
            10
      );

        if (slotChoice > 0)
        {
            shop->SellItem(player, slotChoice - 1);
 }
    }
    else
    {
        _Input->GetInput("[Enter]를 눌러 계속...");
    }

    _Drawer->RemovePanel("Inventory");
}
