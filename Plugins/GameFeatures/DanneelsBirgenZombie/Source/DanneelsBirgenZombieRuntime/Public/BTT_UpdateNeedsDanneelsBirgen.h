#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_UpdateNeedsDanneelsBirgen.generated.h"

UCLASS()
class UBTT_UpdateNeedsDanneelsBirgen: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_UpdateNeedsDanneelsBirgen();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	
};
