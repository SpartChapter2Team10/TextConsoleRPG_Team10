#pragma once
#include "../Singleton.h"
#include <memory>
#include <string>

class Player;
class IMonster;
class ICharacter;

// ===== 전투 결과 구조체 =====
// Scene에서 전투 종료 후 결과를 조회하기 위한 데이터
struct BattleResult
{
    bool Victory = false;  // 승리 여부
    bool IsCompleted = false;       // 전투 종료 여부
  int ExpGained = 0;         // 획득 경험치
    int GoldGained = 0;         // 획득 골드
    std::string ItemName = "";      // 획득 아이템 이름 (없으면 빈 문자열)
};

// ===== 전투 타입 =====
enum class EBattleType
{
    None,
    Normal,     // 일반 몬스터
    Boss        // 보스
};

class BattleManager : public Singleton<BattleManager>
{
private:
    // 현재 전투 상태
    std::unique_ptr<IMonster> _CurrentMonster;
    EBattleType _BattleType = EBattleType::None;
    bool _IsBattleActive = false;
    BattleResult _Result;

public:
    // ===== Scene 친화적 인터페이스 =====
    
    /// <summary>
    /// 전투 시작 (몬스터 생성만 담당)
    /// - GameManager에서 메인 플레이어 레벨 가져옴
    /// - Normal: DataManager에서 랜덤 몬스터 생성
    /// - Boss: 보스 몬스터 생성
    /// </summary>
    /// <param name="type">전투 타입 (Normal/Boss)</param>
    /// <returns>성공 시 true, 실패 시 false</returns>
    bool StartBattle(EBattleType type);
 
    /// <summary>
    /// 전투 종료 처리
    /// - 승리 시: 보상 처리 (CalculateReward 호출)
    /// - 패배 시: 보상 없음
    /// - 모든 파티원 버프 초기화
    /// - 사망한 동료 제거 (GameManager::RemoveDeadCompanions)
    /// - 전투 상태 초기화
    /// </summary>
    void EndBattle();
    
    /// <summary>
    /// 1턴 실행 (Scene의 Update에서 호출)
    /// - 플레이어 턴: ProcessTurn(Player, Monster)
    /// - 몬스터 사망 체크 → 승리 처리
    /// - 몬스터 턴: ProcessAttack(Monster, Player)
    /// - 플레이어 사망 체크 → 패배 처리
    /// - 라운드 종료: 파티 전체 ProcessRoundEnd()
 /// </summary>
    /// <returns>전투 계속 시 true, 종료 시 false</returns>
    bool ProcessBattleTurn();
    
    // ===== 전투 상태 조회 =====
    
    /// <summary>전투 진행 중 여부</summary>
    inline bool IsBattleActive() const { return _IsBattleActive; }
    
    /// <summary>현재 전투 타입</summary>
    inline EBattleType GetBattleType() const { return _BattleType; }
    
    /// <summary>현재 전투 중인 몬스터 (nullptr 가능)</summary>
    inline IMonster* GetCurrentMonster() const { return _CurrentMonster.get(); }
    
    /// <summary>전투 결과 (전투 종료 후 유효)</summary>
    inline const BattleResult& GetBattleResult() const { return _Result; }
    
    // ===== 내부 로직 (팀원이 구현 예정) =====
    
    /// <summary>
    /// 턴 처리 (아이템 사용 + 공격)
    /// - Player: 체력/공격력 포션 자동 사용 판단 → ProcessAttack 호출
    /// - Monster: 즉시 ProcessAttack 호출
    /// </summary>
    void ProcessTurn(ICharacter* Atk, ICharacter* Def);
    
    /// <summary>
    /// 공격 처리 (피해 계산 + 로그 출력)
    /// - Attack() 호출
    /// - 피해량 계산
    /// - PrintManager로 전투 로그 출력
    /// </summary>
    void ProcessAttack(ICharacter* Atk, ICharacter* Def);
    
    /// <summary>
    /// 보상 계산 및 분배 (내부 호출용)
    /// - 경험치: 파티 전체 동일 수치 획득
    /// - 골드: 메인 플레이어만 획득
    /// - 아이템: 메인 플레이어 인벤토리에 추가
    /// - BattleResult 업데이트
    /// </summary>
    void CalculateReward(Player* P, IMonster* M);
    
    // ===== 레거시 인터페이스 (하위 호환용 - 제거 예정) =====
    
/// <summary>레거시: 자동 전투 (Scene 없이 동작)</summary>
    bool StartAutoBattle(Player* P);
    
    /// <summary>레거시: 보스 전투 (Scene 없이 동작)</summary>
    bool StartBossBattle(Player* P);
};
