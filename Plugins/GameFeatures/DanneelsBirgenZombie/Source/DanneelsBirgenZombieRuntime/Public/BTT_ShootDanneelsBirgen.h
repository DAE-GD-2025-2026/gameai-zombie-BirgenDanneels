#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ShootDanneelsBirgen.generated.h"

class ABaseItem;

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_ShootDanneelsBirgen : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ShootDanneelsBirgen();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector WeaponKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ShouldShootKey;

private:
	int GetItemSlot(const TArray<ABaseItem*>& Inventory, ABaseItem* SlotItem) const;
};