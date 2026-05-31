#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Heal.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_Heal : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_Heal();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
