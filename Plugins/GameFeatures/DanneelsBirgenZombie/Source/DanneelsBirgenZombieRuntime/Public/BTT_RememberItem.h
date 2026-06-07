#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RememberItem.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_RememberItem : public UBTTaskNode
{
	GENERATED_BODY()
public:
	
	UBTT_RememberItem();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ItemLocationKey;

	UPROPERTY(EditAnywhere)
	bool CanFlipTrash = false;
	
private:
	
	bool IsLocationReachable(APawn* Pawn, const FVector& Location) const;
	bool FindReachableLocationNearItem(APawn* Pawn, const FVector& ItemLocation, FVector& OutLocation) const;
};
