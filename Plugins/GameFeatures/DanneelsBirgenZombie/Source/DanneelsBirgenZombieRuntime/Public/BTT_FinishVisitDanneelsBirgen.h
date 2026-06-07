#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FinishVisitDanneelsBirgen.generated.h"

/**
 * 
 */
UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_FinishVisitDanneelsBirgen: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_FinishVisitDanneelsBirgen();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
