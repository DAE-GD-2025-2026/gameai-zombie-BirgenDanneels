#pragma once

#include "BehaviorTree/BTService.h"
#include "BTS_CombatDecision.generated.h"

class ABaseZombie;
class ABaseItem;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_CombatDecision : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CombatDecision();

protected:
	virtual void TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& root, uint8* nodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector WeaponKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector HasLineOfSightKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ShouldShootKey;

	UPROPERTY(EditAnywhere)
	float MaxShootRange = 1400.f;

	UPROPERTY(EditAnywhere)
	float ShotgunClusterRadius = 450.f;

	UPROPERTY(EditAnywhere)
	int ShotgunMinZombieCount = 3;

private:
	ABaseZombie* FindBestTarget(const APawn* Pawn) const;
	ABaseItem* ChooseBestWeapon(APawn* Pawn, ABaseZombie* Target, int CloseZombieCount) const;

	bool HasLOS(const APawn* Pawn, const ABaseZombie* Target) const;
	int CountZombiesNearTarget(const APawn* Pawn, const ABaseZombie* Target, float Radius) const;
};