#include "../../../include/UI/Scenes/BattleScene.h"
#include "../../../include/UI/UIDrawer.h"
#include "../../../include/UI/Panel.h"
#include "../../../include/UI/TextRenderer.h"
#include "../../../include/UI/StatRenderer.h"
#include "../../../include/UI/AsciiArtRenderer.h"
#include "../../../include/Manager/SceneManager.h"
#include "../../../include/Manager/DataManager.h"
#include "../../../include/Manager/BattleManager.h"
#include "../../../include/Unit/Player.h"
#include "../../../include/Unit/NormalMonster.h"
#include "../../../include/Unit/Boss.h"
#include "../../../include/Unit/IMonster.h"
#include "../../../include/Item/HealPotion.h"
#include "../../../include/Item/AttackUp.h"
#include <Windows.h>

BattleScene::BattleScene()
    : UIScene("Battle")
    , _Player(nullptr)
    , _BattleEnd(false)
    , _PlayerWin(false)
    , _TurnCount(0)
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Enter()
{
    _Drawer->Initialize();
    _Drawer->Activate();
    _IsActive = true;
    _BattleEnd = false;
    _PlayerWin = false;
    _TurnCount = 0;

    _Player = SceneManager::GetInstance()->GetPlayer();
    if (!_Player)
    {
  _IsActive = false;
        return;
}

    // 몬스터 생성
    DataManager* dm = DataManager::GetInstance();
    auto [stage, monsterName] = dm->GetRandomStageAndMonster();
    
    if (stage.empty() || monsterName.empty())
    {
        _IsActive = false;
     return;
    }

    // 보스전 확인
    if (_Player->GetLevel() >= 10)
    {
 _Monster = std::make_unique<Boss>(_Player->GetLevel());
    }
  else
    {
        _Monster = std::make_unique<NormalMonster>(_Player->GetLevel(), stage, monsterName);
    }

    // UI 구성
    // 플레이어 스탯 패널 (좌측)
    Panel* playerPanel = _Drawer->CreatePanel("PlayerStats", 0, 0, 30, 20);
    playerPanel->SetBorder(true, 10);  // 초록색
    auto playerStats = std::make_unique<StatRenderer>();
    playerStats->SetStat("=== 플레이어 ===", "");
    playerStats->SetStat("이름", _Player->GetName());
    playerStats->SetStat("레벨", std::to_string(_Player->GetLevel()));
    playerStats->SetStat("HP", std::to_string(_Player->GetCurrentHP()) + "/" + std::to_string(_Player->GetMaxHP()));
    playerStats->SetStat("ATK", std::to_string(_Player->GetTotalAtk()));
playerStats->SetKeyColor(11);
    playerStats->SetValueColor(15);
    playerPanel->SetContentRenderer(std::move(playerStats));

    // 몬스터 아트 패널 (중앙)
  Panel* monsterArt = _Drawer->CreatePanel("MonsterArt", 30, 0, 46, 30);
    monsterArt->SetBorder(true, 12);  // 빨간색
    auto art = std::make_unique<AsciiArtRenderer>();
    
    // 애니메이션 폴더 확인
    std::string animPath = dm->GetResourcePath("Animations") + "/" + monsterName;  // GetAnimationsPath() → GetResourcePath("Animations")
    if (dm->DirectoryExists(animPath))
    {
        art->LoadAnimationFromFolder(animPath, 0.3f);
        art->StartAnimation();
    }
    else
    {
        // 정적 이미지 또는 기본 텍스트
        std::string monstersPath = dm->GetResourcePath("Monsters");  // GetMonstersPath() → GetResourcePath("Monsters")
        if (dm->FileExists(monstersPath, monsterName + ".txt"))
        {
            art->LoadFromFile(monstersPath, monsterName + ".txt");
     }
        else
        {
            art->LoadFromString("  [" + monsterName + "]\n\n    /\\_/\\\n   ( o.o )\n    > ^ <");
        }
    }
    art->SetAlignment(ArtAlignment::CENTER);
    monsterArt->SetContentRenderer(std::move(art));

    // 몬스터 스탯 패널 (우측)
    Panel* monsterPanel = _Drawer->CreatePanel("MonsterStats", 76, 0, 30, 20);
    monsterPanel->SetBorder(true, 12);
    auto monsterStats = std::make_unique<StatRenderer>();
    monsterStats->SetStat("=== 적 ===", "");
    monsterStats->SetStat("이름", _Monster->GetName());
    monsterStats->SetStat("레벨", std::to_string(_Monster->GetLevel()));
    monsterStats->SetStat("HP", std::to_string(_Monster->GetCurrentHP()) + "/" + std::to_string(_Monster->GetMaxHP()));
    monsterStats->SetKeyColor(12);
    monsterStats->SetValueColor(15);
    monsterPanel->SetContentRenderer(std::move(monsterStats));

    // 전투 로그 패널 (하단)
    Panel* logPanel = _Drawer->CreatePanel("BattleLog", 0, 40, 106, 25);
    logPanel->SetBorder(true, 7);
    auto log = std::make_unique<TextRenderer>();
    log->AddLine("=== 전투 시작! ===");
    log->AddLine(_Monster->GetStage());
    log->AddLine(_Monster->GetName() + "이(가) 나타났다!");
    log->AddLine("");
    log->SetAutoScroll(true);
    log->SetTextColor(15);
    logPanel->SetContentRenderer(std::move(log));

  _Drawer->Render();
    Sleep(1500);
}

void BattleScene::Exit()
{
    _Drawer->RemoveAllPanels();
    _Drawer->Deactivate();
    _IsActive = false;
}

void BattleScene::Update()
{
    if (_BattleEnd)
    {
   // 전투 종료 처리
        Sleep(2000);
        
        if (_PlayerWin)
   {
   // 레벨 10 미만이면 상점 또는 다음 전투
if (_Player->GetLevel() < 10)
       {
    SceneManager::GetInstance()->ChangeScene(ESceneType::Shop);
        }
  else
   {
   // 승리 Scene
        SceneManager::GetInstance()->ChangeScene(ESceneType::Victory);
            }
        }
        else
        {
 // 패배 Scene
            SceneManager::GetInstance()->ChangeScene(ESceneType::GameOver);
  }
 return;
    }

    // 전투 진행
    ProcessPlayerTurn();
    CheckBattleEnd();
    if (_BattleEnd) return;

    ProcessMonsterTurn();
    CheckBattleEnd();
    
    _Player->ProcessRoundEnd();
    _TurnCount++;
}

void BattleScene::Render()
{
    _Drawer->Render();
}

void BattleScene::HandleInput()
{
    // 자동 전투이므로 입력 없음
}

void BattleScene::ProcessPlayerTurn()
{
    Panel* logPanel = _Drawer->GetPanel("BattleLog");
    TextRenderer* log = logPanel ? dynamic_cast<TextRenderer*>(logPanel->GetContentRenderer()) : nullptr;

    // 아이템 사용 로직 (BattleManager와 동일)
    if (_Monster->GetCurrentHP() < _Player->GetTotalAtk())
{
        // 공격으로 처치 가능
        int beforeHP = _Monster->GetCurrentHP();
   _Player->Attack(_Monster.get());
        int damage = beforeHP - _Monster->GetCurrentHP();

        if (log)
      {
            log->AddLine(_Player->GetAttackNarration());
            log->AddLine("피해: " + std::to_string(damage));
        }
    }
    else if (_Player->GetCurrentHP() <= _Player->GetMaxHP() / 4)
    {
        // 회복 포션 사용
     int slot = _Player->GetInventory().FindFirstSlotIndexOfType<HealPotion>();
      if (slot != -1)
     {
         if (log) log->AddLine(_Player->GetName() + "이(가) 회복 포션을 사용했다!");
  _Player->UseItem(slot);
        }
   else
        {
  // 포션 없으면 공격
      int beforeHP = _Monster->GetCurrentHP();
            _Player->Attack(_Monster.get());
     int damage = beforeHP - _Monster->GetCurrentHP();
            if (log)
  {
            log->AddLine(_Player->GetAttackNarration());
       log->AddLine("피해: " + std::to_string(damage));
            }
        }
    }
    else if (_Monster->GetCurrentHP() > _Player->GetTotalAtk())
    {
     // 공격력 포션 사용
        int slot = _Player->GetInventory().FindFirstSlotIndexOfType<AttackUp>();
        if (slot != -1)
   {
            if (log) log->AddLine(_Player->GetName() + "이(가) 공격력 포션을 사용했다!");
            _Player->UseItem(slot);
        }
 
        // 공격
        int beforeHP = _Monster->GetCurrentHP();
        _Player->Attack(_Monster.get());
        int damage = beforeHP - _Monster->GetCurrentHP();
        if (log)
        {
            log->AddLine(_Player->GetAttackNarration());
          log->AddLine("피해: " + std::to_string(damage));
        }
  }
    else
    {
      // 일반 공격
        int beforeHP = _Monster->GetCurrentHP();
     _Player->Attack(_Monster.get());
        int damage = beforeHP - _Monster->GetCurrentHP();
        if (log)
        {
       log->AddLine(_Player->GetAttackNarration());
 log->AddLine("피해: " + std::to_string(damage));
        }
    }

    UpdateUI();
    _Drawer->Render();
    Sleep(800);
}

void BattleScene::ProcessMonsterTurn()
{
    Panel* logPanel = _Drawer->GetPanel("BattleLog");
    TextRenderer* log = logPanel ? dynamic_cast<TextRenderer*>(logPanel->GetContentRenderer()) : nullptr;

    int beforeHP = _Player->GetCurrentHP();
    _Monster->Attack(_Player);
    int damage = beforeHP - _Player->GetCurrentHP();

 if (log)
{
    log->AddLine(_Monster->GetAttackNarration());
        log->AddLine("피해: " + std::to_string(damage));
    }

 UpdateUI();
    _Drawer->Render();
    Sleep(800);
}

void BattleScene::UpdateUI()
{
 // 플레이어 스탯 업데이트
    Panel* playerPanel = _Drawer->GetPanel("PlayerStats");
    if (playerPanel)
{
        StatRenderer* stats = dynamic_cast<StatRenderer*>(playerPanel->GetContentRenderer());
        if (stats)
        {
      stats->SetStat("HP", std::to_string(_Player->GetCurrentHP()) + "/" + std::to_string(_Player->GetMaxHP()));
  stats->SetStat("ATK", std::to_string(_Player->GetTotalAtk()));
            playerPanel->Redraw();
        }
  }

    // 몬스터 스탯 업데이트
    Panel* monsterPanel = _Drawer->GetPanel("MonsterStats");
  if (monsterPanel)
    {
      StatRenderer* stats = dynamic_cast<StatRenderer*>(monsterPanel->GetContentRenderer());
        if (stats)
        {
            stats->SetStat("HP", std::to_string(_Monster->GetCurrentHP()) + "/" + std::to_string(_Monster->GetMaxHP()));
            monsterPanel->Redraw();
        }
    }
}

void BattleScene::CheckBattleEnd()
{
    Panel* logPanel = _Drawer->GetPanel("BattleLog");
    TextRenderer* log = logPanel ? dynamic_cast<TextRenderer*>(logPanel->GetContentRenderer()) : nullptr;

    if (_Monster->IsDead())
    {
        _BattleEnd = true;
        _PlayerWin = true;
        
        if (log)
  {
       log->AddLine("");
  log->AddLine("=== 승리! ===");
     log->AddLine(_Player->GetName() + "이(가) " + _Monster->GetName() + "를 쓰러뜨렸다!");
      }

 // 보상 지급
        BattleManager::GetInstance()->CalculateReward(_Player, _Monster.get());
        _Player->ResetBuffs();
     
        _Drawer->Render();
    }
    else if (_Player->IsDead())
    {
  _BattleEnd = true;
_PlayerWin = false;
        
  if (log)
        {
         log->AddLine("");
      log->AddLine("=== 패배... ===");
    log->AddLine(_Player->GetName() + "이(가) 쓰러졌다...");
        }

        _Player->ResetBuffs();
        _Drawer->Render();
    }
}
