#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FinishVisit.generated.h"

/**
 * 
 */
UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_FinishVisit: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_FinishVisit();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
