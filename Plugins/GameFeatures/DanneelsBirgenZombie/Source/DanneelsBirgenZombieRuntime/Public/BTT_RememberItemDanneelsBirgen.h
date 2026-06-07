#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RememberItemDanneelsBirgen.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_RememberItemDanneelsBirgen : public UBTTaskNode
{
	GENERATED_BODY()
public:
	
	UBTT_RememberItemDanneelsBirgen();
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
