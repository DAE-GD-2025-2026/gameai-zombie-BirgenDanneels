#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EnterHouseDanneelsBirgen.generated.h"

/**
 * 
 */
UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_EnterHouseDanneelsBirgen: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_EnterHouseDanneelsBirgen();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
