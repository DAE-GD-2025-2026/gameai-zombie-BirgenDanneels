#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EnterHouse.generated.h"

/**
 * 
 */
UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_EnterHouse: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_EnterHouse();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
