#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PickUpLoot.generated.h"

class ABaseItem;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_PickUpLoot: public UBTTaskNode
{	
	GENERATED_BODY()
public:
	UBTT_PickUpLoot();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
private:
	 int GetFreeInventorySlot(const TArray<ABaseItem*>& Inventory) const;
};
