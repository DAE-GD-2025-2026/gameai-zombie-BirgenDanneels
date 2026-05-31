#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Eat.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_Eat : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Eat();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
