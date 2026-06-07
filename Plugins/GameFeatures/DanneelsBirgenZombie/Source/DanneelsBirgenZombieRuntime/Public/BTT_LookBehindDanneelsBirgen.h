#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LookBehindDanneelsBirgen.generated.h"

UENUM()
enum class ELookBehindState : uint8
{
	LookingBehind,
	RotatingBack
};

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_LookBehindDanneelsBirgen : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_LookBehindDanneelsBirgen();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& root, uint8* nodeMemory, EBTNodeResult::Type taskResult) override;

	UPROPERTY(EditAnywhere, Category="Look Behind")
	float RotationSpeed = 3.f;

	UPROPERTY(EditAnywhere, Category="Look Behind")
	float HoldTime = 0.75f;

private:
	bool RotatePawnTowards(APawn* Pawn, float TargetYaw, float DeltaSeconds) const;
	float GetMovementYaw(APawn* Pawn, bool LookBehind) const;
	
	float HoldEndTime = 0.f;

	ELookBehindState State;
};