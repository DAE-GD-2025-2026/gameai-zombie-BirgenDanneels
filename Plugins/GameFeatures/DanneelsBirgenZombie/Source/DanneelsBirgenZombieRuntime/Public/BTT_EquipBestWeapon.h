#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EquipBestWeapon.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_EquipBestWeapon: public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
