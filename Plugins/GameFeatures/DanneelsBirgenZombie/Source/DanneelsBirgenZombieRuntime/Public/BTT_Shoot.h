#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Shoot.generated.h"

class ABaseItem;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_Shoot: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_Shoot();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
protected:
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector WeaponKey;
	
private:
	
	int GetItemSlot(const TArray<ABaseItem*>& Inventory, ABaseItem* SlotItem) const;
};
