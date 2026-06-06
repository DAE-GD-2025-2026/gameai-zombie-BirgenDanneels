#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_UpdateNeeds.generated.h"

UCLASS()
class UBTT_UpdateNeeds: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_UpdateNeeds();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
