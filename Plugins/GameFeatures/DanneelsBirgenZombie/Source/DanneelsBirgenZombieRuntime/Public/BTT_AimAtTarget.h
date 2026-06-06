#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_AimAtTarget.generated.h"

class ABaseZombie;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_AimAtTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_AimAtTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& root, uint8* nodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	float AimRotationSpeed = 8.f;

	UPROPERTY(EditAnywhere)
	float AcceptableAngle = 1.f;
	
};