#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ScanAround.generated.h"

UCLASS()
class DANNEELSBIRGENZOMBIERUNTIME_API UBTT_ScanAround: public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_ScanAround();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& root, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds) override;
private:
	float TotalYaw;
};
