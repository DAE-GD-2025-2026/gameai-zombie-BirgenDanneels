#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateThreatDanneelsBirgen.generated.h"

class AAIController;
class UBlackboardComponent;
class ABaseZombie;
class UStudentPerceptorDanneelsBirgen;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_UpdateThreatDanneelsBirgen : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_UpdateThreatDanneelsBirgen();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="Threat")
	float StartFleeRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category="Threat")
	float StopFleeRadius = 1800.f;
	
	UPROPERTY(EditAnywhere, Category="Threat")
	float StartSprintRadius = 900.f;
	
	UPROPERTY(EditAnywhere, Category="Threat")
	float StopSprintRadius = 1300.f;

	static ABaseZombie* FindClosestZombie(const TArray<ABaseZombie*>& Zombies, const FVector& MyLocation, float& OutDistSq);
};