#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateThreat.generated.h"

class AAIController;
class UBlackboardComponent;
class ABaseZombie;
class UStudentPerceptor;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTS_UpdateThreat : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_UpdateThreat();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="Threat")
	float DangerRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category="Threat")
	float StartSprintRadius = 900.f;
	
	UPROPERTY(EditAnywhere, Category="Threat")
	float StopSprintRadius = 1300.f;

	static ABaseZombie* FindClosestZombie(const TSet<TObjectPtr<ABaseZombie>>& Zombies, const FVector& MyLocation, float& OutDistSq);
};